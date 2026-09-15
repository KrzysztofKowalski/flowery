# Notes

Working notes on the port: what the original pipeline did, and how the two
implementations were shown to agree. Things here were established by reading
`legacy/` and by measuring the sample files; they are not obvious from either
source tree on its own.

## The legacy pipeline

`legacy/cx.sh` is the actual curve generator. Everything else
(`run.sh`, `preparecommands.sh`, `subprepare.sh`, `runner.sh`, `launch.sh`)
exists to fan it out over millions of parameter combinations with GNU
`parallel`; `samples.sh` calls it directly for the 614 files in
`legacy/samples`.

`cx.sh` feeds gnuplot a parametric plot, with `a=1` and `s=0` unless extra
arguments say otherwise:

```
n1p={0,1}*2*pi*n1  ...  s1p={0,1}*2*pi*s1  ...
z(t)=a1*exp(n1p*t+s1p) + a2*exp(n2p*t+s2p) + a3*exp(n3p*t+s3p)
set size ratio -1
set terminal svg size 777,777
set samples N
plot [t=0:1] real(z(t)), imag(z(t))
```

Three things fell out of this that the port has to match exactly:

1. **The sampling convention.** gnuplot evaluates `t = i/(N-1)` for `i` in
   `0..N-1`, so both endpoints are included and the first and last points
   coincide. Getting this wrong (`t = i/N`, the natural choice) leaves the
   loop open by one step — it was the first bug found, and `test.sh` now
   pins it.
2. **`set size ratio -1`** means equal x and y scales, i.e. the curve keeps
   its aspect ratio and is centred in the canvas. That is what
   `build_screen_points()` reproduces, in window points instead of 777×777.
3. **Colour**: gnuplot wrote `stroke='rgb(148,   0, 211)'`, hence the purple
   in `main.c` and in the SVGs written by `s`.

## The sample files

`legacy/samples` holds 614 SVGs, but only about 497 distinct parameter sets:
**117 of them are duplicates whose names end in ` 2`** (a leftover from
fetching the directory twice, e.g. `spiro-0001-0001-1998-2001 2.svg`). Any
tooling that globs these paths has to cope with spaces in the names — the
test binary reads the directory itself for exactly that reason.

gnuplot writes the whole curve as a single
`<path stroke='rgb(148, 0, 211)' d='M x,y L x,y ... '/>` element, wrapped
over many lines. The point markers in `<defs>` also contain `M`/`L`, so the
parser has to anchor on the `stroke` attribute rather than on the first
`<path`.

**20 files contain fewer points than the sample count in their own name** —
for instance `spiro-0001-0001-99999-99999.svg` has 27468 of 99999 points
(that one aliases to a slow wheel: with `n3 = samples` the fast wheel nearly
cancels, so the curve crawls and many neighbouring points land on the same
terminal coordinate). Whatever gnuplot dropped there, the remaining points
no longer line up one-to-one with the curve, so `test.sh` reports these files
rather than comparing them. The other 594 compare point by point.

## How agreement was measured

The SVGs are in screen coordinates; the curve is not. Comparing them needs
the transform between the two, which is gnuplot's plot window position, its
uniform scale, and the y flip. Rather than guess gnuplot's margins, the test
fits `u = ox + s*x`, `v = oy - s*y` by least squares over all points at once
and looks at the residuals — a wrong curve cannot be rescued by the fit.
Result: 0.043px average, 0.094px worst point, over 594 files. The residual
is essentially gnuplot's own coordinate rounding (it writes one decimal).

A per-axis normalisation instead of the similarity fit reports a deviation of
1.0 and looks like a total mismatch — that is an artefact of the comparison,
not of the curve. The y flip is what gives it away: without it the same fit
gives 332px average against 0.05px with it.

## Key repeat

Held keys repeat, but deliberately not through the OS. SDL3 only generates
repeat events while text input is active (`SDL_StartTextInput`), at the user's
configured rate — and, more to the point, it would repeat *every* key, so a
held `s` would write an SVG every few milliseconds. Instead `main.c` keeps its
own table of held keys (`hold_key` / `release_key` / `key_repeats`) and
re-fires only the keys that sweep a value: the arrows and `[` `]`, after
300 ms and then every 40 ms. Everything that toggles or saves something is
deliberately left out of that set. It is `SDL_EVENT_KEY_UP` that removes an
entry, so the table is also cleared on `SDL_EVENT_WINDOW_FOCUS_LOST` — no
key-up ever arrives once the window is unfocused and the key would otherwise
repeat forever.

Verified by injecting events with `SDL_PushEvent`: holding Up for ~1 s took
`n[0]` from 480 to 507, while holding `s` for the same second wrote exactly
one file.

## Performance

Measured on the development machine (Haswell i7-4850HQ, 2.3 GHz, AVX2 + FMA but
no AVX-512; gcc 16.2.1, glibc 2.44). Two setups were used, and they disagree
enough that both are recorded here:

* **offscreen** (`SDL_VIDEODRIVER=offscreen`) reproduces far better between
  runs and is the one to quote for anything below the draw.
* **on the real display** timings are contaminated by shader compilation,
  buffer allocation and vsync on the first frames: an early probe reported
  25 ms for work that a warmed-up standalone probe measures at **0.36 ms**. Do
  not trust a single-digit frame count from a freshly started window. Warm up
  (the numbers below use 120 frames) and take the best of several.

An earlier version of this section said the offscreen setup "uses SDL's
*software* renderer". It does not, at least not with SDL 3.4: `SDL_GetRendererName()`
reports `opengl` under the offscreen driver too, and `glGetString(GL_RENDERER)`
gives the same `Mesa Intel(R) Iris(R) Pro Graphics P5200` on both. The two
setups differ in back-buffer size and in whether the frame goes through the
compositor (900x900 px against 1800x1800 px), not in the renderer. The earlier
"reproduces to the millisecond" claim is also optimistic: the same row measured
57.5 ms and 61.3 ms in two runs of the same binary, so anything under ~10% is
noise.

### The sampling kernel

`flowery_points` was 110 ns per sample. It is now 16.4 ns, a **6.7x** speedup,
with the curve still matching gnuplot to the same 0.043 px as before.

| change | ns/sample | speedup |
|---|---|---|
| original (`-O2`, no `-march`) | 109 | — |
| `-O3 -march=native` alone | 104 | 1.06x |
| angle hoisted out of the loop | 89 | 1.2x |
| `-ffast-math -fopenmp-simd` → `libmvec` sincos | 17 | 6.4x |
| hand-written AVX2 sin/cos | 16.1 | 6.8x |
| four-term reduction, for the 400x wider range below | **16.4** | **6.7x** |
| order-3 recurrence, resync every 256 (not used) | 8.8 | 12.3x |

Two routes to a vector sin/cos were rejected before this one:

* **`-ffast-math` + `libmvec`.** gcc will emit calls to glibc's 4-wide
  `_ZGVdN4vvv_sincos`, but only under the *whole* `-ffast-math`;
  `-fno-math-errno -fno-trapping-math -fopenmp-simd` is not enough and silently
  stays scalar, and `-funsafe-math-optimizations` alone is not enough either.
  `-ffast-math` also licenses reassociation, which is the one thing the 1e-12
  unit tests in `tests/` would notice. It did in fact pass 388/388 when tried,
  but that is luck rather than a guarantee, so the flag is not used.
* **Calling `_ZGVdN4vvv_sincos` by its mangled name.** It is an IFUNC, the
  output pointers have a 32-byte alignment requirement the ABI does not make
  obvious, and getting it wrong segfaults. Not worth the fragility.

So `flowery.cpp` carries its own `sincos4`: a four-lane Cody-Waite reduction
followed by two polynomials and a quadrant fixup. Two traps in there cost real
time and are worth not re-discovering:

* **The reduction constants.** fdlibm's `pio2_1 / pio2_2 / pio2_3` are a
  *split* of pi/2 — each is the correctly rounded remainder after the previous
  ones. Its `pio2_1t / pio2_2t / pio2_3t` look like further terms and are
  actually *alternatives* used by its two-step refinement. Subtracting all six
  takes pi/2 away three times over; the symptom is a curve that is visibly
  wrong, not slightly off.
* **The polynomial.** Truncated Taylor is not good enough over `|r| <= pi/4`.
  Stopping at `r^11` costs ~7e-12 at `r = pi/4`, four orders of magnitude worse
  than double rounding, and it showed up as a 2.8e-10 wobble in the curve. The
  shipped coefficients are fdlibm's minimax ones.

Measured against libm over 1.6M arguments spanning +/-1e6: worst relative error
**2.3e-16**, about 1 ULP, and exact at 0, pi/2, pi and 2pi. The whole kernel
against a scalar reference over 1000003 samples differs by at most 8.9e-16,
which is why the 1e-12 unit tests and the gnuplot comparison both come out
unchanged.

The reduction is exact only while `fn * kPio2_i` is exactly representable, so
the vector path needs a bound — and the bound it had, `TAU * (|n| + |s|) <= 1e6`
per wheel, was a cliff rather than a slope. At N=1048576 the same curve costs
**16.4 ms** at `n = 159154` and **109.8 ms** at `n = 159155`: **6.7x** for one
press of an arrow key. Holding Shift+Up adds ten teeth every 40 ms, so ten
minutes of holding reaches it, and `./flowery 200000 1 1` reaches it in one
command. Above the cliff the per-sample cost is flat, so this is a step from
one code path to another and not a gradual degradation.

The split is now derived from pi/2 instead of copied from fdlibm: four terms of
25 / 24 / 25 significant bits plus a full-width tail, all four subtracted
unconditionally. That costs **+5.8%** on the kernel (16.4 -> 17.3 ns/sample)
and makes `fn * term` exact up to `fn < 2^28`; the four terms sum to pi/2 to
2e-41, so the limit moves from 1e6 to 4e8 — a wheel with 63 million teeth, 400x
further out. Past 4e8 `flowery_points` still falls back to libm.

The extra term is worth its 5.8% because the fallback is worse than it looks:
glibc takes its own multi-precision branch once the angle passes about 1e8, so
past the new limit the scalar path is not 6.8x slower than the vector one but
**13x** — 98 ns/sample below 1e8, 190 at 3e8, 225 at 1e9. A wheel big enough to
leave the vector path is slow twice over.

Verified against libm rather than asserted, over 40M arguments (8M per range,
plus the quadrant boundaries, which are the reduction's worst case — `fn*pi/2`
cancels exactly there, so `|r|` peeks past pi/4 and the polynomial is evaluated
at the edge of its interval):

| range | worst relative error | worst ULP |
|---|---|---|
| [0, 1e6] (the old limit) | 2.22e-16 | 1.0 |
| [1e6, 1e7] | 2.24e-16 | 2.0 |
| [1e7, 1e8] | 2.32e-16 | 2.0 |
| [1e8, 4e8] (the new limit) | 2.48e-16 | 2.0 |
| near k*(pi/2), k < 2e6 | 2.22e-16 | 1.0 |
| near k*(pi/2), k ~ 1e8 | 2.22e-16 | 1.0 |

Not one argument in 40M was more than 2 ULP from libm. The whole kernel against
a scalar reference over 200003 samples still differs by at most 8.9e-16, the
same figure as before.

The price is that the curve is no longer *bit*-identical to the old one: the
two reductions round differently in the last bit, so 5-17% of points move, by
at most 1 ULP. That is the same size as the difference either version has
against libm (9.2e-16), and both are equally close to it, so this is a wash
rather than a loss. The externally visible numbers do not move: 388/388 unit
tests, 594/614 files, 0.043 px average and 0.094 px worst point, before and
after.

### The bounding box and the screen mapping

`flowery_bbox` was 1.94 ms at N=1048576 and is 1.02 ms: `_mm256_min_pd` /
`_mm256_max_pd` four points at a time, then a horizontal reduce. min/max are
exact, so this is bit-identical to the scalar scan.

`build_screen_points` is vectorised the same way, four points at a time, and the
two `double -> float` conversions plus an interleave produce four `SDL_FPoint`s
per iteration. The operations are the same ones in the same order as the scalar
loop, so the results are bit-for-bit identical.

### Not recomputing what has not changed

`render_frame` used to recompute the curve, lay it out and allocate three fresh
buffers **every frame**, even when nothing had moved. The buffers now persist
and grow on demand, and two dirty flags separate "the curve changed" (a wheel,
a phase, the sample count, the animation) from "the window changed". An idle
frame now does no curve maths at all: 0.0 ms where it used to be ~131 ms at
N=1048576.

The mallocs turned out to be a red herring on their own — 0.01 ms plus ~7 ms of
page faults at N=1048576 — but they are gone anyway.

### Where a frame actually goes

The complaint that started this was "some values are fast, others are very
slow". A sweep over the wheel values a user can reach, 900x900 offscreen, best
of 7 after 40 warm frames. `arc` is the length of the drawn polyline in window
points — how much path the rasteriser has to lay down, before any overdraw —
and `drawn` the number of vertices left after thinning.

| wheels | samples | kernel ms (before → after) | bbox | thin | drawn | arc pts | draw ms |
|---|---|---|---|---|---|---|---|
| 5/7/12 | 2001 | 0.033 → 0.035 | 0.001 | 0.004 | 1993 | 12992 | 0.18 |
| 5/7/12 | 1048576 | 17.4 → 18.3 | 0.94 | 2.17 | 25621 | 12993 | 1.70 |
| 480/648/816 | 2001 | 0.033 → 0.035 | 0.001 | 0.004 | 2001 | 647985 | 1.90 |
| 480/648/816 | 1048576 | 17.3 → 18.3 | 0.95 | 2.26 | 802681 | 808550 | 61.3 |
| 12000/15000/20000 | 2001 | 0.033 → 0.038 | 0.001 | 0.004 | 2001 | 1560000 | 0.14 |
| 12000/15000/20000 | 1048576 | 17.2 → 18.3 | 0.94 | 2.48 | 1045026 | 21390707 | minutes |
| 159155/159156/159157 | 2001 | 0.232 → 0.035 | 0.001 | 0.004 | 1999 | 725075 | 2.04 |
| 159155/159156/159157 | 1048576 | 107.2 → 18.3 | 0.95 | 2.43 | 1047364 | 179685506 | minutes |
| 2e8/2.4e8/3e8 | 2001 | 1.31 → 1.32 | 0.001 | 0.003 | 2 | 0 | 0.00 |
| 2e8/2.4e8/3e8 | 1048576 | 532 → 560 | 1.03 | 2.53 | 1048576 | 231645461 | minutes |

Two things fall out of it.

**Above a few tens of thousands of samples the draw is the frame, not the
curve.** At N=2001 with the default wheels the kernel is 0.033 ms and the draw
is 1.90 ms; at N=1048576 the kernel is 17-18 ms and the draw is 61 ms. Even at
the default sample count the curve maths is 1.7% of the frame.

**The draw scales with the curve's arc length on screen, not with `samples`,
and not with the vertex count either.** The cleanest pair in the table is
5/7/12 against 480/648/816 at N=2001: both draw 2001 vertices at 900x900, and
the second takes 10.6x longer (1.90 ms against 0.18 ms) because its polyline is
50x longer (647985 points against 12992). Nothing else differs. The same
relationship holds at the top of the range: at N=1048576, 5/7/12 and the
default wheels draw 25621 and 802681 vertices for 12993 and 808550 points of
arc, and take 1.70 ms and 61.3 ms.

`arc` is roughly `2*pi * sum(|n_k| * a_k)`, the curve's own length in world
units, scaled into the window; the wheel values are the whole of it. At
N=1048576 the default 480/648/816 draws 808550 points of arc and
159155/159156/159157 draws 179685506 — 222x. The ratio of the teeth is 245x;
the gap is how much the three wheels cancel against each other in the two
cases, which scales with `n` too. That is the "some values are fast, others are very slow"
report, and it is linear in `n` with no cliff in it. The cliff was the kernel's
and is the one thing that was fixed; this one is the curve being genuinely
longer.

The sweep also shows a trap in reading any of these rows. Note that at N=2001
the bottom row costs *nothing* while its neighbour at N=1048576 is the most
expensive in the table. With `samples = 2001` the sampling step is 1/2000, so a
wheel whose teeth divide 2000 lands on the same handful of phases at every
sample: 2e8, 2.4e8 and 3e8 are all multiples of 2000, the curve collapses to
two distinct points, and the frame is free. `12000/15000/20000` at N=2001 is
half of this — 12000 and 20000 are multiples of 2000, 15000 is not — and its
`arc` of 1560000 is the polyline zigzagging the width of the window 2000 times.
Any benchmark of this program that picks round numbers at the default sample
count is likely measuring an aliased curve, so the sweep above uses the
`legacy/` values and N=1048576 for anything it wants to conclude from.

On the real display (1800x1800 px, 120 warm frames, vsync off so the work is
visible rather than hidden behind the swap):

| wheels | samples | kernel ms | drawn | draw ms | present ms |
|---|---|---|---|---|---|
| 5/7/12 | 2001 | 0.03 | 1993 | 0.68 | 0.39 |
| 480/648/816 | 2001 | 0.03 | 2001 | 22.97 | 16.29 |
| 480/648/816 | 1048576 | 18.27 | 802681 | 103.87 | 31.20 |
| 159155/159156/159157 | 2001 | 0.04 | 1999 | 24.21 | 18.22 |

Note the second row against the first: at the *default* sample count, with the
default wheels, the draw alone is 23.0 ms — over the 16.7 ms a 60 Hz frame
allows, before any of the curve maths. A per-segment and per-pixel model fitted
to synthetic polylines on the same setup puts it at roughly **70-100 ns per
segment plus 15-25 ns per device pixel**, and at N=2001 the pixels are 99% of
it (2001 segments, 1.3M device pixels of arc).

### Drawing

At N=1048576 with the software renderer the curve is 16 ms and the draw is
~78 ms, so the draw is what is left. Two changes, both measured on the real
display with a warmed-up standalone probe:

* **Rainbow batching, 2.5x.** The rainbow mode set a colour and called
  `SDL_RenderLine` once per segment — a million draw calls at the top of the
  range. `SDL_RenderLines` over a run of segments is about twice as cheap per
  segment as one call each, and the hue rises monotonically with the sample
  index, so equal quantised hues are already contiguous. 256 runs now replace
  the million calls: 212.8 ms -> 85.3 ms at N=1048576, 53.3 -> 20.5 ms at
  N=262144, 0.45 -> 0.25 ms at N=2001.
* **Thinning the drawn polyline, ~1.3x at N=1048576.** A vertex is only kept
  once it is 0.5 points from the last kept one. It is *not* lossless and the
  numbers are small: at N=1048576 it drops 1048576 vertices to 802681 and
  changes 904 of 1080000 pixels (0.08%), because a dense curve really does have
  hundreds of thousands of pixels of arc length, and a single pixel at a
  crossing can flip. At N=2001 and below it keeps everything. The saved SVG is
  never thinned.

For reference, `SDL_RenderLines` costs about 55 ns per segment at 2880x1800 on
the Intel Iris Pro, once warm; at 1800x1800 the synthetic numbers above put it
at 70-100 ns per segment plus 15-25 ns per device pixel.

**The thinning threshold was re-examined and left where it is.** Since the
per-segment cost is real, raising `MIN_SEGMENT_PTS` from 0.5 was the obvious
next move, and it does buy something: on the default wheels at N=1048576, on
the real display, 0.5 -> 1.0 pt drops 802681 vertices to 607408 and the draw
from 104.8 ms to 88.2 ms, 2.0 pt to 321934 vertices and 61.0 ms, 4.0 pt to
183457 and 48.6 ms. The `arc` barely moves (808550 -> 808505 pt), so what is
saved is all per-segment.

What it costs is measured by rendering to BMP and diffing, and that turned out
to be the fiddly part. The first attempt reported *zero* differing pixels for
every threshold. That is a broken result rather than a good one — the images
were uniformly black, so every pair compared equal — and the cause was never
pinned down: the same readback in a later probe, before or after the present,
with vsync on or off, returns the real frame in both setups. An obscured or
not-yet-presented window on the real display is the plausible culprit. The
lesson is the cheap one: count the lit pixels in each image before trusting a
diff, because "no differences" and "no image" look the same in a diff.

Read correctly, at scale 1 (900x900 px, default wheels, N=1048576, 0.5 pt
against 1.0 pt): 1702 of 810000 pixels change, 0.21% of the canvas but 4.2% of
the 40768 pixels the curve actually lights, and they are near-balanced — 781
gained, 921 lost — so this is single-pixel flips along the edge of the stroke
as the path shifts by a fraction of a pixel, not geometry going missing. At
scale 2 it is the same 0.21% of the canvas. The one curve that does worse is
5/7/12 at N=1048576, whose thinning at 0.5 pt is already at the floor: it drops
25621 vertices to 12900 and loses 9432 of its 57000 lit pixels, 16% — there the
curve genuinely thins.

So: 1.3x on the draw of the densest curve, for 4% of the curve's pixels on the
common ones and 16% on the ones that are already cheap, and no change at all to
any sample count at or below 262144. Given that 105 ms becomes 88 ms and the
frame is seven times over budget either way, that is not a trade worth making
by default, and 0.5 pt — one device pixel on this machine's 2x display — stays.
A sagitta test in place of the distance test would thin the straight runs much
harder for the same deviation and is the change actually worth making if the
draw ever needs to come down; it is a bigger change than this sweep justified.

The status line reports `samples=N (drawing M)` so it is visible when the two
differ.

### Flags

`-O3 -march=native -fno-math-errno -fno-trapping-math`, and `-std=c++20`.
`-march=native` is what enables every vector path; the sources build without it
and fall back to scalar code that produces the same numbers (checked by running
the suite both ways, 594/614 and 0.043 px either way). `-fno-math-errno` and
`-fno-trapping-math` only drop the obligation to set errno and raise traps on
domain errors, which nothing here reads — unlike `-ffast-math` they do not
permit reassociation.

## Display scaling

Both SDL2 and SDL3 need `SDL_WINDOW_HIGH_PIXEL_DENSITY` for a native-resolution
back buffer on a scaled display: the window is measured in points, the back
buffer in pixels. Measured on the development machine (Omarchy: Arch +
Hyprland, on MacBook hardware, internal panel 2880x1800 at scale 2):

```
without HIGH_PIXEL_DENSITY   window 710x427 pts | renderer  710x427 px | scale 1.00
with HIGH_PIXEL_DENSITY      window 710x427 pts | renderer 1420x854 px | scale 2.00
```

Without the flag the compositor stretches a 1× image over a 2× screen, so
everything the app draws is soft. With it, and with drawing done in points
under `SDL_SetRenderScale()`, the curve is laid out identically but rasterised
at the full native resolution. `SDL_GetWindowDisplayScale()` is the source of
the scale (SDL 3.2+).

Fullscreen reaches the panel's native resolution too — same window, `f`
pressed, sizes logged per frame:

```
windowed    900x900 pts  -> 1800x1800 px   (scale 2.00)
tiled       687x850 pts  -> 1374x1700 px   (Hyprland sized it)
fullscreen 1440x900 pts  -> 2880x1800 px   (scale 2.00)
```

1440x900 is the logical size of the panel at scale 2, so 2880x1800 is its
native pixel count: fullscreen is genuinely 1:1, not upscaled. The resize
between those lines also shows the window following the compositor, which is
what `SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED` is there for.

Note that `SDL_SetWindowFullscreen()` needs the event loop to be pumping
before the compositor's new size arrives; a probe that fullscreens and
immediately measures still sees the old size.
