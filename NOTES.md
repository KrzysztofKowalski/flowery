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
   in `main.cpp` and in the SVGs written by `s`.

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
held `s` would write an SVG every few milliseconds. Instead `main.cpp` keeps its
own table of held keys (`hold_key` / `release_key` / `key_repeats`) and
re-fires only the keys that sweep a value: the arrows, `[` `]` and the stroke
width on `,` `.`, after 300 ms and then every 40 ms. Everything that toggles or
saves something is deliberately left out of that set. It is `SDL_EVENT_KEY_UP` that removes an
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
visible rather than hidden behind the swap). This table and the model under it
are the **pre-fix** baseline — "Drawing in pixels" further down takes the `draw`
column almost to nothing — but everything here about the kernel, the thinning
and the shape of the sweep still stands:

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

That split — a small per-segment term and a large per-device-pixel one — is the
diagnosis the fix was built on: remove the per-pixel term and the draw stops
dominating the frame. See "Drawing in pixels".

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

### Drawing in pixels

The change that mattered. SDL3 only hands `SDL_RenderLines` to the driver's line
API when the render scale is 1; at any other scale it takes
`RenderLinesWithRectsF` -> `RenderLineBresenham` -> `RenderPointsWithRects`,
which emits **one quad per line pixel** — four vertices and six indices each.
The app drew in window points under `SDL_SetRenderScale(scale, scale)`, so on a
2x display every frame went down that software path: 2001 segments and 1.3M
device pixels of arc at the default sample count, one quad apiece. The renderer
was hardware the whole time — `opengl`, Mesa on the Iris Pro. The lines were not.

What changed:

* `update_sizes()` leaves the renderer at scale 1.
* `build_screen_points()` takes a `zoom` argument and multiplies the layout by
  it, so the curve still lands on physical pixels. The screen passes the display
  scale; `save_svg()` passes 1, which keeps the file in points and independent
  of the display it happened to be written on.
* `build_display_points()` takes its thinning threshold as a parameter, and the
  screen passes one *device* pixel, because the points it is handed are device
  pixels now.
* `main()` sets `SDL_HINT_RENDER_LINE_METHOD=2` before creating the renderer,
  and only if nothing has set it already, so the other methods stay reachable
  for comparison. The hint is read at renderer-creation time, so it has to
  precede `SDL_CreateRenderer()`.
* `draw_overlays()` puts the renderer back at the display scale for the status
  line and the help overlay. SDL's debug font is a fixed 8x8 **pixels**, so at
  scale 1 it would come out half size on a 2x display. The curve wants a device
  pixel per point; the font wants a point per pixel.

Measured on the real display, vsync off, default 480/648/816 wheels, N=2001,
fullscreen 1440x900 pts / 2880x1800 px:

| | geom ms | draw ms | present ms | frame ms |
|---|---|---|---|---|
| before: points, scale 2, default method | 0.070 | 23.250 | 24.650 | 47.969 |
| after: pixels, scale 1, method 2 | 0.059 | 0.156 | 1.630 | 1.845 |

and the same pair in a 900x900-point window (1800x1800 px):

| | geom ms | draw ms | present ms | frame ms |
|---|---|---|---|---|
| before | 0.056 | 22.084 | 95.752 | 117.892 |
| after | 0.044 | 0.177 | 0.007 | 0.227 |

The draw falls by ~150x, and the default-wheels frame goes from 47.97 ms to
1.85 ms — under the 16.7 ms a 60 Hz frame allows, with the curve maths now a
larger share of what is left than the drawing is. `present` moves around a lot
between runs (it is the swap, and vsync is off); `draw` is the stable signal and
the one the change is about.

Method `2` is the driver's `GL_LINE_STRIP` and keeps the thin lines. Method `3`
was measured at +29% ink and is documented as drawing "thicker diagonal lines",
so it was the wrong default to take. Method `2` is documented as "occasionally
missing line endpoints based on hardware driver quirks", which is why the hint
is only a default that the environment can override.

The residual is the per-segment term now, not the per-pixel one. But the table
above is N=2001, where the thinning keeps every sample and there are only 2001
of them; **the ~150x does not generalise upward**, and re-measuring the top of
the range is the useful correction. At N=1048576 the frame was 161.6 ms with
104.0 ms of draw before this change, and 100.4 ms with 73.8 ms of draw after it:
1.6x, not 150x. The two paths cost different things, and only one of them
saturates. The software path cost ~17 ns per device *pixel* of arc, and the ink
a 2880x1800 frame can hold is bounded by the frame itself — 5.2 Mpx is ~88 ms,
so it flattens out. The driver line path costs ~80 ns per *point drawn*, and
there is no such ceiling: at N=1048576 there are 916k points, so it does not.
That leaves this change as the one that makes the *default* sample count cheap,
and the renderer choice below as the one that matters at six figures.

### The renderer costs more than the drawing did

SDL's line drawing is not one cost, it is one cost per backend. Same frame,
same points, same everything, only `SDL_RENDER_DRIVER` changed, N=262144,
fullscreen 1440x900 pts / 2880x1800 px, vsync off:

| backend | draw ms | ns per drawn point |
|---|---|---|
| `opengl` | 21.361 | 80.6 |
| `vulkan` | 2.045 | 7.8 |
| `software` | 0.392 | (12.549 ms of it reappears in `present`) |
| `opengl`, line method 3 (geometry) | 29.280 | 119.7, plus 25.8 ms of present |

and it holds across the range: 9.9x at N=65536, 10.4x at N=262144, 10.2x at
N=1048576 (73.8 ms against 5.9 ms). The default renderer on this machine was
`opengl`, which is why six-figure sample counts were unusable.

The two draw the same picture, which is the part that had to be checked before
switching. Frames captured from both backends at N=262144 agree **pixel for
pixel**: 99.6% mask overlap and the same ink to 0.1% (70096 px against 70119 px
out of 5.18 Mpx). At N=2001 the masks overlap only 81% — that is SDL's
documented "occasionally misses line endpoints based on hardware driver quirks"
showing up on long chords — but the ink total still matches to 0.6% (145826
against 145009) and the two frames cannot be told apart side by side.

`main()` asked for Vulkan by name on the strength of that, with SDL's own pick
as the fallback, and an explicit `SDL_RENDER_DRIVER` still wins so every backend
stays reachable for exactly this comparison. Both fallbacks were checked: with
`SDL_VULKAN_LIBRARY` pointed at nothing it comes up `opengl`. **That default was
reversed the next day** — see "The default renderer went back to GL" below,
which is the part of this section that aged.

Two caveats. Vulkan's `present` is the more expensive of the two (2.6-3.2 ms
against 0.4-2.1 ms), so at the *default* sample count, where the draw is now
0.025 ms, the total is slightly worse — 3.4 ms against 2.4 ms. Both are far
inside the 16.7 ms a frame allows, so nothing is visible, but it is the reason
this is a switch to make for the top of the range and not a free win at the
bottom. And `present` was the noisiest number in the whole sweep: one N=1048576
run reported 15.9 ms of it and a 49.5 ms frame, while three repeats gave 33.8,
34.0 and 34.4 ms. Nothing here is GPU-bound, either — 1-3 ms of `present`
against 800k vertices says the work is in SDL's vertex handling, not the
rasteriser.

The third caveat is the one to read before trusting this on another machine.
Mesa says so on startup, unprompted:

```
MESA-INTEL: warning: Haswell Vulkan support is incomplete
```

This is a Gen7.5 part and Mesa's Vulkan driver for it is not finished. The
frames above were verified pixel for pixel against the GL ones on *this* driver,
so it renders correctly here today, and `SDL_RENDER_DRIVER=opengl` is the way
back — but the fallback in `main()` only catches a Vulkan renderer that fails to
*create*, not one that creates and draws wrongly. On a different Haswell, or
after a Mesa change, the GL path is the trustworthy one, and the comparison to
re-run is the one in this section: same wheels, same N, capture both, compare
ink.

### The rainbow hue runs

Rainbow mode divided once per drawn point:

```c
const int q = (int)((long long)a->didx[s + 1] * RAINBOW_STEPS / n);
while (e < m - 2 && (int)((long long)a->didx[e + 2] * RAINBOW_STEPS / n) == q)
```

A 64-bit integer division is about 40 cycles and `m` runs to six figures.
Measured at N=262144 on Vulkan: `draw` is 2.045 ms plain and 5.826 ms in
rainbow. Of the 3.8 ms difference, 255 extra draw calls account for ~0.1 ms, so
~3.7 ms — 14 ns a point, 42 cycles, exactly a division — was this.

`didx` is monotone, so the bucket is monotone, and the end of a run is the first
sample whose bucket is past `q`, which is `ceil((q+1)*n/RAINBOW_STEPS)`. That is
one division per *run* (at most 256 of them) instead of one per point. Measured:
5.826 ms down to 2.321 ms, so rainbow now costs 0.28 ms over plain rather than
3.8 ms, and at N=1048576 it is inside the noise of the plain draw.

The two loops pick exactly the same runs, which is not something to take on
trust — the rewrite changes where the boundary comes from. It was checked
against the original on 3640 cases: `n` from 2 to 2097152 including values that
are not multiples of 256, and `didx` dense, thinned and degenerate, with the
boundary jittered off the exact multiples of `n/256` because that is where an
off-by-one would live. 0 mismatches.

### The stroke width

SDL 3.4.16 has no line width. There is no `SDL_SetRenderLineWidth` in any SDL3
header — grepping `/usr/include/SDL3/*.h` finds `glLineWidth` in the GL headers
and nothing else — so a stroke can only be drawn by the app. The API that could
do it properly is `SDL_RenderGeometry`, and the closest measured stand-in for
that work (line method 3, which builds the outline and hands it over, exactly
what a geometric stroke is) cost 29.28 ms of draw at N=262144 against 2.05 ms
for the driver line path on Vulkan — 119.7 ns a point against 7.8, about 15x the
thin line for a 5 px stroke. So a stroke is **k offset copies of the thin
line**, pushed along the point's normal: `w` passes down the same polyline.

Whole pixels, not fractions. SDL's line rasteriser snaps to the pixel grid, so
two copies half a pixel apart land on the same pixels and the stroke comes out
uneven. The offsets go outwards from the centre — 0, −1, +1, −2, +2 — which
makes a stroke of width `w` symmetric about the curve, and makes `w=1` exactly
the single call it always was.

Measured offscreen at 900x900, scale 1.00, vsync off, on the default
480/648/816 wheels. `draw` and `thin` are the trustworthy columns; `total` is
not, and neither is `present` (see "Measuring it"):

| N=262144, 262144 points | thin ms | draw ms | ns/pt |
|---|---|---|---|
| `w=1` (default) | 0.68 | 1.83 | 7.0 |
| `w=3` | 2.55 | 6.49 | 24.8 |
| `w=5` | 2.53 | 11.20 | 42.7 |
| `w=8` | 2.62 | 19.11 | 72.9 |
| `w=5 taper` | 3.51 | 5.31 | 20.3 |
| `w=5 speed` | 7.52 | 7.26 | 27.7 |

and at N=1048576, where the thinning leaves 802681 points to draw:

| N=1048576, 802681 points | thin ms | draw ms | ns/pt |
|---|---|---|---|
| `w=1` | 2.32 | 5.70 | 7.1 |
| `w=3` | 8.14 | 21.42 | 26.7 |
| `w=5` | 8.20 | 36.65 | 45.7 |
| `w=5 taper` | 10.88 | 17.25 | 21.5 |
| `w=5 speed` | 23.01 | 20.54 | 25.6 |

The law is **~9.5 ns per drawn point for each extra pixel of width**, over a
base of ~7 ns: 9.2 ns at N=262144 and 9.6 at N=1048576, so it does not depend
on how densely the samples sit. A 5 px stroke is about 6.4x a thin line, and it
is the drawing, not the geometry, that pays — `thin` is flat in the width
(+1.85 ms at N=262144 whatever `w` is, +5.9 at N=1048576) because building the
normals and the width table does not care how wide the stroke ends up.

Offscreen reports scale 1.00 and so flatters this path; the `w=1` base of
7.0 ns a point is the same figure the Vulkan renderer gives on the display
(7.8), and nowhere near `opengl`'s 80.6, so these runs are on the renderer the
app actually picks. It also means these are **device** pixels: at scale 2 a
5 px stroke is 2.5 points on screen, half the visual width it has here. A
stroke measurement on the real display at scale 2 has not been made.

**How the width varies.** `v` cycles flat / taper / speed, and `,` `.` are the
width (1-16 device pixels, held they repeat like the arrows). Both default to
off: `w=1 flat` is the old behaviour, and at `w=1` none of the stroke geometry
is built at all — `build_normals`, `build_widths` and the `wid`/`nrm`/`thk`
buffers are never touched — so every number earlier in these notes still
describes the default.

`taper` ramps with `didx`, cheap and obvious. `speed` is the interesting one:
width follows how fast the curve is travelling, and the thing to get right is
that it follows the point's **rank** in the speed distribution rather than the
speed itself. A spirograph is a sum of three rotating vectors, so its speed
spans orders of magnitude, and any direct mapping puts nearly the whole curve
at one end of the width range — measured, a linear map left the stroke at 44%
of the ink the same mean width should give. Ranking against a 128-bin histogram
in log space fixes it without knowing what the distribution is, on this curve
or the next one: 71% of the ink. The log is `fast_log2`, the exponent shifted
out of the float and the mantissa standing in for the fraction — monotone,
about a tenth of a bin, and it replaced an honest `log()` that cost ~50 cycles
a point, 20 ms at a million points, four times the drawing it was in aid of.
Speed and the normals are also built in one pass now rather than two, because
each pass wanted its own `sqrt`.

**Verification.** The default path was checked to emit the *identical* sequence
of `(colour, SDL_RenderLines(ptr, count))` calls as the code before it on 3850
cases — and that sequence is the only way `draw_curve` can affect the image, so
identical calls mean identical pixels. The thick path was checked on the same
3850 cases to tile the polyline exactly once: every sample covered, none twice,
no run left short. `./test.sh` is unchanged at 594/614 with a worst 0.043 px,
which it should be, since the SVG is not touched. And the strokes nest: `w=5`
covers `w=3` covers `w=1`, with 15 pixels outside `w=5` out of ~268k of `w=3`
ink, so "width" means what it says.

**What it does not do.** `save_svg()` still writes `stroke-width="1"`. That is
deliberate — the SVG is the artefact compared against gnuplot and the oracle
parses its polyline — but it does mean the saved file does not carry the stroke,
and if it should, that is a small change.

### The wave

The stroke modes are flat, taper, speed and wave. `wave` is the sine: the width
runs `1 + (maxw-1) * (0.5 - 0.5*cos(2*pi*waves*t))`, so `t=0` is exactly the
thinnest line the app can draw rather than an arbitrary point on the curve, and
the peaks are exactly `maxw`. `v` cycles the modes, `,` `.` are the width,
`<` `>` (shifted comma and period) are how many waves fit along the curve —
1 to 64, seven by default.

The one thing worth writing down is what it cost to compute. A cosine per
drawn point is a libm call per point, and at 802681 points (what N=1048576
draws) that measured **26.25 ns a point, 21.070 ms a frame** — comparable to the
whole drawing it was in aid of. The width only ever lands on one of `maxw`
integers, so the phase is accumulated and the answer is looked up in a
1024-entry table of finished widths: **1.76 ns a point, 1.415 ms**, a 19.7 ms
saving. In the app that shows as `thin` of 9.93 ms for `wave` against 8.76 for
`flat` at the same width — the table, not the cosine.

Measured, offscreen 900x900, `thin`/`draw` in ms:

| | N=262144 | N=1048576 |
|---|---|---|
| `flat` w=7 | 2.63 / 17.29 | 8.76 / 58.17 |
| `wave/1` | 3.19 / 7.82 | 10.15 / 27.81 |
| `wave/7` | 3.20 / 7.99 | 9.93 / 25.95 |
| `wave/64` | 3.17 / 7.77 | 10.61 / 28.38 |
| `taper` w=7 | 3.50 / 7.47 | 11.04 / 25.98 |

`wave` comes out cheaper than `flat` at the same width because its mean width
is 3.5, not 7, and the wave count is nearly free: the number of runs is
`12*waves + 1` (85 at seven waves, 769 at sixty-four), so a finer wave does not
fragment the drawing. The default appearance is untouched — the frame at `w=1
flat` is byte-for-byte the one from before the mode existed, checked with
`cmp`.

Two honest notes. The wave count only has a peak where the samples can resolve
one: at N=32, or N=2001 with 64 waves, the maximum width reached is 6 rather
than 7. That is sampling, not arithmetic. And the mean is 3.501 rather than
4.0 because the width is truncated to whole pixels, exactly as `taper` does.

### How many colours there are

`RAINBOW_STEPS` was 256 with a comment claiming that was past what the eye
resolves. It is not, and 256 was throwing away most of the wheel. At full
saturation the colour has one channel at 255, one at 0 and one walking between
them, so eight-bit RGB can express `6*255` of them and no more. Measured, the
number of distinct colours a sweep of the wheel actually produces:

| steps | distinct colours |
|---|---|
| 256 | 256 |
| 1024 | 1023 |
| **1530** | **1527** |
| 1536, 4096, 65536 | 1527 (identical — repeats) |

So 1530 is the whole wheel and everything past it is a duplicate. The cost is
per *draw call*, not per colour — the run count is bounded by
`min(RAINBOW_STEPS, drawn points)` — so the ceiling is 1530 calls, about
0.36 ms, whatever the sample count. Measured at N=2001: `draw` 0.049 -> 0.241 ms
for the 1274 extra calls, about 150 ns each.

The number of colours *in a frame* is a different question, and it is answered
by the geometry, not by this constant: the same build draws 215 distinct
colours at 480/648/816 and 1521-1528 at 5/7/9. See below.

### The rainbow is destroyed by the order it is drawn in

This one is worth reading before "fixing" the rainbow a third time.

The hue walks the wheel with the sample index, so the runs come out in hue
order and the **last** run drawn wins wherever the curve crosses itself. A
spirograph crosses itself a great deal: at N=65536 the curve has about 1.4M
device pixels of arc and leaves only ~35k distinct ones, so each pixel is
painted some forty times. The last few percent of the arc alone is enough to
repaint the whole figure, and the last few percent is red.

Measured, the ink in a frame by hue, 15-degree buckets, at 480/648/816:

| N | 0-15 | ... | 315-330 | 330-345 | 345-360 |
|---|---|---|---|---|---|
| 2001 | 1% | ~0% | 30% | 34% | 35% |
| 65536 | 0% | 0% | 6% | 16% | 70% |

and the same source on wheels that do not cross themselves (`5 7 9`,
`7 11 13`, `23 41 61`) gives an even spread across all 24 buckets and
1521-1528 distinct colours. **The algorithm is not the variable — the shape
is.** A rasteriser in Python, doing exactly what `draw_curve` does (same runs,
same hue formula, a plain DDA instead of SDL), reproduces the app's own frame:
69.6% red against the app's 89%. The code is faithful to the design; the design
is what fails.

It also has a second face at higher sample counts, which is what a user
reported as "artefacts above 4000 points": once the samples are closer together
than a pixel, adjacent pixels along a line are painted by different passes, and
the picture becomes per-pixel colour noise rather than a gradient. At N=8004
the parametric rainbow is a speckle of unrelated hues; the same frame on
`5 7 9` is clean.

The candidate fix is to take the hue from the point's **direction from the
centre of the figure** instead of from the sample index. Two passes over the
same place have the same direction, so a later pass paints the same colour on
top of itself and the gradient survives. It is implemented and reachable with
`FLOWERY_RAINBOW_MODE=angle`; it is not the default. It does not use `atan2` —
`|dy|/(|dx|+|dy|)` runs 0..1 across an octant and the signs pick which, which is
monotone round the circle and costs a divide and a test. What it does cost is
draw calls: the direction sweeps the whole circle once per turn, so at
N=262144 the runs would be 166026 without a limit. `MAX_RAINBOW_RUNS` caps them
at 8192 (a run is not cut for a colour change until it is `m/8192` points long),
which is about 1.2 ms of drawing at any sample count and leaves the colour per
point exactly where the curve is coarse enough for that to be what the eye
sees. At N=8004 it turns the noise into a coherent marbling in one hue family
rather than fixing it outright — the curve still passes over itself forty times.

### The default renderer went back to GL

Vulkan was made the default for the ten-fold line speed, and on 2026-09-16 it
was made not the default again, for a reason that took a while to see.

The symptom: past roughly ten thousand samples the HUD comes out smeared across
the top-left corner, with a stray line running from it toward the curve.
Offscreen the same build is clean — at N=2001, 16008, 64032, 256128 and
1024512, at render scale 1 and at a forced scale 2, the top-left corner is
pixel-identical to the N=2001 frame every time. The frames are fine. What
identified it was the user saying the artefact **cannot be caught in a
screenshot**: a screenshot reads a finished buffer, so if the buffer is clean
and the screen is not, the difference is made between the buffer and the
screen — swapchain, damage tracking, compositor — and none of the app's own
geometry, buffers or colours are implicated.

GL is clean on the display, so GL is the default, and it keeps about twice as
many distinct colours besides (428 against 215 in the same frame). Vulkan
remains one `SDL_RENDER_DRIVER=vulkan` away. The relevant line at startup is
Mesa's own: `MESA-INTEL: warning: Haswell Vulkan support is incomplete`.

### What the GL line cost actually is

With GL as the default, the 80 ns a point matters again, so it was measured
properly — offscreen, 900x900, vsync off, per point, with a `glFinish` where
there is one to call:

| path | N=262144 | N=1048576 |
|---|---|---|
| raw GL, own shader, `glDrawArrays(GL_TRIANGLE_STRIP)`, 2 verts/point | **5.6** | **2.6** |
| vulkan, `SDL_RenderLines` | 4.8 | 4.8 |
| GL, `SDL_RenderLines` (the current default) | 69.8 | 72.4 |
| GL, `SDL_RenderGeometryRaw`, our own vertices, every layout tried | 46.8-53.1 | 52-68 |
| vulkan, our own geometry | 42.4 | 50.1 |

So the 80 ns is Mesa's `GL_LINE_STRIP`, not OpenGL, and building the triangles
ourselves removes it — the strip is about twice as fast as Vulkan, not merely
level with it. Two things close the other doors: `SDL_RenderGeometryRaw`
without raw GL is only 1.2-1.5x for 90-110 MB of buffers a frame, and on
**vulkan** our own geometry is 8-11x *worse* than plain `SDL_RenderLines`,
because SDL transforms vertices on the CPU there. This rewrite is only worth
anything on GL.

The price is the HUD. After our own GL calls `SDL_RenderDebugText` draws
nothing — it returns true and sets no error and puts 0 pixels on the screen,
measured with an isolated probe (491 px of text from a clean GL renderer, 0 px
after a bare `glUseProgram`/`glBindVertexArray`) — and `SDL_FlushRenderer`, the
documented "call this between SDL's render API and the low-level API", does not
repair it from either side. Curve in raw GL and HUD through SDL do not coexist
in one frame; the HUD would need its own font. The harness that produced the
table, and the probes that isolated this, are in `tmp/linebench/`.

### The curve and the HUD went into raw GL

Both halves of that were paid. The curve is one
`glDrawArrays(GL_TRIANGLE_STRIP)` with our own shader, two vertices a point,
the colour on the vertex and the stroke built in the shader from a per-point
normal; the HUD is drawn from an atlas cut out of the very font SDL uses for
its debug text, sampled for alpha only and coloured per vertex. `FLOWERY_DRAW=lines`
brings back the SDL renderer and every line of the old drawing code; nothing
else about the app changed.

**The number that matters is not the one in the table above.** That table
measured *drawing* with the vertex buffer uploaded once, outside the frame
loop. The app is not that: SDL's line path builds its vertex data again on
every call, so at N=1048576 `SDL_RenderLines` costs 49.5 ms of **CPU** a frame
whether or not the curve changed, while the raw path costs 0.037 ms — the strip
is built and uploaded only when something it is built from changes
(`vertsDirty`), and an idle frame is one `glDrawArrays` over a resident buffer.
Same run, offscreen 900x900, scale 1, `FLOWERY_PROFILE=1`, no benchmark, ms:

| N=1048576, curve untouched | kernel | layout | thin | draw | hud | present | total |
|---|---|---|---|---|---|---|---|
| raw GL | 0.007 | 0.002 | 0.003 | **0.037** | 0.068 | 0.007 | 0.125 |
| SDL lines | 0.044 | 0.010 | 0.008 | **49.476** | 0.085 | 1.137 | 50.760 |

That is the whole case for the rewrite, and it is not GPU throughput: it is a
per-frame CPU cost that disappears. A benchmark does not show it, because
`FLOWERY_BENCH` forces the curve dirty every frame on purpose — that is the
worst case, and there the difference is 31.5 ms of draw against 52.5 (total
55.0 against 73.7). Both numbers are real and they answer different questions.
`space` dirties the curve every frame, so a running animation lives in the
worst-case column; a curve you are only looking at lives in the other one.

**Offscreen cannot price the rasteriser.** Nothing here calls `glFinish`, and
an offscreen swap never blocks, so both columns are CPU time per phase. On the
display the frame is bounded by the swap, and what the 49.5 ms decides is
whether vsync is reachable at all: at a million samples the SDL path cannot
draw a frame inside 16.7 ms, while the raw path leaves the whole budget to the
GPU.

**What it looks like.** Ink over the curve area, background and the HUD's 24
rows excluded:

| N | raw GL | SDL lines | GL / lines | old pixels with no new pixel in 1 px |
|---|---|---|---|---|
| 2001 | 62359 | 64635 | 96.5% | 1.11% |
| 262144 | 45883 | 29745 | 154% | 0.00% |

The HUD is not in that table because it is *identical*: 2074 px at N=2001 and
2065 at N=262144 on both paths, and zero differing pixels between them. The
atlas reproduces SDL's debug font exactly, down to the pixel, which is the only
reason the ink comparison above is worth reading at all.

The stroke is `half = 0.5 * width * 1.414` — √2, because that is where a 45°
diagonal's neighbour pixel falls inside the band. Measured at N=2001 against
the old line: 1.0 gives 71.3% of the ink and leaves 3.8% of the old pixels
uncovered, 1.2 gives 83.6% and 2.1%, **1.414 gives 96.6% and 1.1%**, 1.5 gives
101.7% and 0.9%. So this is the width that agrees with Bresenham best, and the
price shows on the other side of the table: at N=262144 the curve carries 1.54x
the ink, which is not a thicker stroke but the diagonal steps Bresenham leaves
empty, now filled in. Coverage is 100% one way and 98.9% the other, so the two
pictures are close to the same picture — but this is a *look*, and a look is
decided by eye on the display, not by a table.

Three traps, all of them measured:

* `SDL_RenderDebugText` into the **software** renderer draws nothing until
  something calls `SDL_RenderPresent` — SDL's renderers queue and execute at
  present. 0 of 760 columns before, 563 after. The raw-GL version of the same
  symptom is the one in the section above; they are different bugs.
* The two vertex builders count different things: in the strip `k` counts
  vertices, in the HUD it counts floats. Uploading the HUD with the strip's
  convention sent about sixteen characters of the status line and nothing else.
* `glReadPixels` hands its rows over bottom-up, so `FLOWERY_BENCH_BMP` produced
  a frame mirrored against the one the SDL path writes. Both flip now and the
  two BMPs compare directly.

**What it costs in memory.** The strip is 9 floats a vertex and 2 vertices a
point, so 72 bytes a point: 57.8 MB at `drawn` = 802681, and the same again as
CPU scratch — 116 MB, against roughly 6 MB for the old path. Memory is the
price of this rewrite; everything above is CPU.

**Not verified, and worth saying before anyone repeats it.** Everything here is
offscreen at scale 1, and offscreen cannot reproduce the scale-2 path; the only
scale-dependent piece is the `u_pscale` uniform, which reads the display scale
the way the rest of the app does. Vsync is untested — `SDL_GL_SetSwapInterval`
answers "unavailable" offscreen. A run with `FLOWERY_BENCH_BMP` prices
`glReadPixels` (27 ms at 900x900) inside its `present` column, so those rows are
not comparable with the rest of the table.

### Measuring it

The app carries its own harness, because the numbers above are not reproducible
by eye and the interesting ones are all at sample counts nobody types in by
hand.

* `FLOWERY_BENCH=<n>` holds the curve still, forces it dirty every frame (the
  worst case — an idle frame does no curve maths at all, so there is nothing to
  compare), turns vsync off, drops the help overlay, and quits after `n` counted
  frames. The first 30 frames are drawn but not timed: the compositor is still
  resizing the window and the first draw compiles shaders.
* `FLOWERY_PROFILE=1` just prints the same line every 60 frames while the app
  runs normally.
* `FLOWERY_BENCH_FULLSCREEN=1` is not a convenience. Hyprland resizes the
  window underneath the app — the runs above landed on 410x427 points, then
  401x418, and one on 1440x900 — and the thinning threshold sits close enough to
  the sample spacing that a 2% smaller window moves `drawn` by 7%. Only the
  fullscreen size is stable enough to compare two builds.
* `FLOWERY_BENCH_BMP=<file>` leaves the last frame on disk, which is how the two
  backends were compared pixel by pixel. `FLOWERY_RAINBOW=1` too.
* `FLOWERY_DRAW=lines` runs the same binary on the old SDL drawing path — the
  way to check a look or a timing against what was there before. Everything
  else — the keys, the stroke flags, the rainbow — is shared.
* A benchmark on the display is listening to the keyboard while it runs. This is
  not theoretical: a `,` typed during a stroke-width run moved the width from 5
  to 1 half way through the measurement, and the run reported the mean of two
  different strokes. Bench mode now logs the key (`bench: key , during a
  benchmark run`) so a disturbed run says so itself. The stroke numbers above
  are offscreen for that reason — no window, no keyboard, deterministic
  geometry. Offscreen cannot say anything about the scale-2 path, but for
  comparing one width against another it is the better instrument.

Each line reports kernel, layout (bbox + mapping), thinning, draw, HUD and
present separately, plus `samples` and `drawn`, because the ratio between those
two is what says whether the thinning is doing anything — and at 480/648/816 it
mostly is not: at N=262144 it removes 7% and at N=1048576 it removes 24%,
because the curve's arc is genuinely ~800k device pixels long. There is no
cheap "render fewer samples" win here: the sample spacing at these counts is
already about one pixel, so dropping samples to make the drawing cheaper would
make the curve visibly coarser. That was worth measuring rather than assuming —
it is the obvious idea and it does not hold.

### What is left

At N=1048576 on Vulkan the frame is 33.8 ms and it splits 18.5 kernel, 3.9
layout, 2.7 thin, 5.9 draw, 2.7 present — so the kernel is now well over half of
it, and it is the one term that has had no attention since the AVX2 port. On
the raw GL path the draw term leaves that split altogether in the idle case
(0.037 ms against 5.9 at the same N), which makes the kernel not just the
largest term but effectively the whole frame; the work above did not touch what
this section is about.

It is not obviously broken. `flowery_points` runs 159 instructions per four
samples, 60 of them FMA, with **zero register spills**, and the clock on this
machine measures 2.28 GHz (a dependent-FMA chain, 5 cycles an FMA), which makes
17.6 ns a sample about 40 cycles — an IPC near 1.0. The port-limited floor for
that mix is roughly 52 cycles per iteration, so there is up to ~3x of theoretical
headroom, but it is all in the quadrant logic: `vblendvpd` (two uops), the
`vpmovsxdq` masks and the port-1-only `vroundpd`/`vcvttpd2dq` between them cost
more than the two polynomials do. Getting at it means restructuring that logic,
and the arithmetic in `sincos4` is deliberately the same operations in the same
order as the scalar path — that invariance is what the 0.043 px agreement with
gnuplot rests on, so it is not a change to make for a 2x on a sample count the
app rarely sits at.

The cheaper structural items are the three O(N) passes: the bounding box reads
back the 16 MB the kernel just wrote, and folding it into the kernel loop would
remove one of them for perhaps 1.5 ms at N=1048576. Beyond that the lever is
threads — the kernel is per-sample independent, so a persistent pool over index
ranges would be bit-exact and is the only thing here that could plausibly reach
the 16.7 ms frame at a million samples. It is also a different kind of change
from everything above, and this is a laptop that is being used while it runs.

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
everything the app draws is soft. With it, the curve is laid out identically but
rasterised at the full native resolution. `SDL_GetWindowDisplayScale()` is the
source of the scale (SDL 3.2+).

The scale is applied to the **layout**, not to the renderer:
`build_screen_points()` scales the coordinates into device pixels and the
renderer stays at 1, because a scaled renderer costs SDL its driver line path
and roughly 150x on the draw. The HUD is the exception and is drawn in points.
See "Drawing in pixels".

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

## Paused work

`master` now holds `d7b2d81` ("m", 2026-09-17 16:26), which the user committed
from their own terminal: the raw-GL round, the bounding box fold below, and the
documentation of both. Still uncommitted are `src/main.cpp` and this file - the
radius animation and the random history - which have never been compiled,
because building is banned as of 2026-09-17.

### The bounding box moved into the kernel — built, not confirmed

`flowery_points()` takes a fourth argument, `double *bbox`, and accumulates
{minx, miny, maxx, maxy} while it samples. `flowery_bbox()` is still there and
still returns the same number — min and max are exact and associative, and the
four AVX2 accumulators are folded down before the scalar tail runs — it is
simply no longer called by the app; `build_screen_points()` is handed the box
instead of computing it. The point is to stop reading the two arrays back, 16 MB
at N=1048576, for something the sampling loop already has in registers.

An agent wrote this and was stopped before it reported. What it left behind is in
`tmp/bbox/`, and it is more than the write-up suggested: a pre-change binary
(`tmp/flowery-bbox-before`), eight-run profile logs for both binaries at both
sample counts, a paired A/B log that alternates the two binaries, and frames from
each. The binary in the repo root is the **after** build — `./run.sh` launches
this patch. `tmp/flowery-bbox-before` is the binary the rest of this document was
measured against.

**What is settled.** The change compiles and links, and it does not alter the
picture: the before and after frames are **byte-identical** at N=2001 and at
N=262144, which is the box being bit-exact, not merely close. So the fold is
correct, and the look is unchanged.

**What is not.** The oracle suite was never run — there is no `594/614` and no
`passed` anywhere in its logs — so the 0.043 px agreement has not been
re-checked after this change, and the only guard on the numerics is the pair of
identical frames above. The profile numbers below are the agent's; I could not
reproduce them, because building is banned as of 2026-09-17.

**The numbers, and why they do not settle the question.** Layout does get
cheaper, which is the whole claim, and in the paired A/B log it is consistent:
at N=1048576, 2.99/3.10/2.89/2.70 -> 2.25/2.89/2.24/2.02 ms, and at N=262144,
0.81/0.93/0.67/0.66/1.07 -> 0.78/0.54/0.55/0.64/0.79. But the same pairs put the
**kernel** consistently slower at N=1048576 — 13.86/13.96/13.92/13.79 ->
14.26/14.58/14.44/14.30, about +0.5 ms every time, which is the cost "What is
left" predicted when it said the kernel was the wrong side of the port budget to
add work to. The unpaired batches disagree: there the kernel reads 14.14 against
14.16 and only layout moves. So the saving is real and the cost is plausible, and
they are the same size — roughly a wash at N=1048576, which is also what the
totals show once the draw phase is accounted for.

Treat it as an unconfirmed draw, not a win. It is worth building and reproducing
before it is kept or dropped, and the honest expectation from both logs is that
net time will not move; the case for keeping it would be that the layout phase
gets a fifth cheaper, not that the frame gets faster.

**A trap this left behind.** `thin` is not comparable between the two draw
paths: at N=1048576 the raw-GL binary reports 5.8-8.0 ms and the SDL-lines
binary 1.87-1.99 ms for the same phase, because the GL path does work there the
lines path does not. Comparing phase by phase across `FLOWERY_DRAW` is comparing
different work, so only compare like with like.

### The animation and the random history — written, not compiled

Both additions are now in `src/main.cpp`, and nothing else was touched.
**Nothing here has been compiled or run** - the build ban means the whole of
this is source that has been read, not a result. What follows is what the code
says, which is all there is to go on until it is built.

**The animation breathes the radii.** `space` still advances the phases, and
now also puts `a[k]` on a sine through the same 0.75..2 range
`randomize_wheels()` draws from: centre `ANIM_RADIUS_MID` 1.375, amplitude
`ANIM_RADIUS_AMP` 0.625, one rate for all three wheels (`ANIM_RADIUS_RATE`
0.004 turns/s), and a per-wheel offset of `k/3`. The offset is what makes the
three radii sit a third of a turn apart, and because they share a frequency
their sines sum to exactly zero - so the figure holds its size while its shape
changes. That is the "wheels breathing" reading rather than the whole figure
pulsing, and it is a property of the chosen numbers, not of the code. The rate
is below every wheel's phase rotation (0.008*(k+1) turns/s), which is what was
decided when the feature was specified; a full breath takes 250 s, so it is one
constant to tune by eye. The sine overwrites `a[k]` and stays wherever it is
when the animation is switched off, as chosen: there is deliberately no undo
for the animation itself, though a `z` back past an entry does take the radii
with it, since `a` is in the snapshot like everything else - while the
animation is stopped. With `space` on there is nothing for it to take them
back to: the sine owns `a[k]` and rewrites it on the next frame, so a `z` shows
the roll's `n` and `s` and the radii carry on breathing. An edit made while
animating likewise records the sine's instantaneous radii into the entry under
the cursor, because that is what the live state holds. The radii are only
remembered as state at all while the animation is off. The animation clock
runs only while animating, so pausing freezes the breath and resuming carries
it on rather than jumping.

**The randomisations are recorded.** `r` rolls and records the state it rolled;
`z` and `x` step back and forward through the recorded states; the HUD shows
the position as `hist n/m`. The snapshot is the full state - the wheels plus
stroke width, stroke mode, wave count, rainbow flag and sample count - so
stepping back past a roll also undoes a width or colour change made after it,
and stepping forward returns to that change rather than to the state the roll
left. `sel` is not in the snapshot; which wheel is selected is not part of the
picture. A fresh `r` while stepped back drops everything ahead of the cursor,
and the list is seeded with the startup state, so `z` from the first roll lands
on the state the app opened in. It holds 64 states - the startup seed and then
63 rolls - and after that drops the oldest, which is where `z` stops reaching.
Both keys auto-repeat along with the other stepping keys.

**The status line became two lines.** It already ran to about 127 characters
against the 112 that fit a 900-point window at the 8-point debug font, so
`hist n/m` could not be appended to it: line 1 carries the shape (n, a, s, N)
and line 2 the way it is drawn (stroke, rainbow, hist, window and scale). Both
drawing paths draw both lines, at y=12 and y=24, and the help overlay still
starts at y=40.

**What this leaves unsettled.** Everything that needs a build: that it
compiles, that the two keys do what the paragraphs above say, and that the
two-line HUD reads well on the display rather than merely fitting. One thing it
does not disturb is the oracle: the test binary is
`tests/test_flowery.cpp src/flowery.cpp` and does not contain `main.cpp`, so
none of this reaches it - the still-unconfirmed 594/614 for the bbox fold is
neither helped nor hurt by this part.

Free keys now (at the time of writing): `u i o t w y a d e g j k l m n p`.
