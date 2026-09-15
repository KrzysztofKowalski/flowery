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
badly enough that both are recorded here:

* **offscreen** (`SDL_VIDEODRIVER=offscreen`) uses SDL's *software* renderer and
  reproduces to the millisecond between runs. It is the one to quote.
* **on the real display** SDL picks the `opengl` renderer. Timings taken there
  are contaminated by shader compilation, buffer allocation and vsync on the
  first frames: an early probe reported 25 ms for work that a warmed-up
  standalone probe measures at **0.36 ms**. Do not trust a single-digit frame
  count from a freshly started window.

### The sampling kernel

`flowery_points` was 110 ns per sample. It is now 16.1 ns, a **6.8x** speedup,
with the curve still matching gnuplot to the same 0.043 px as before.

| change | ns/sample | speedup |
|---|---|---|
| original (`-O2`, no `-march`) | 109 | — |
| `-O3 -march=native` alone | 104 | 1.06x |
| angle hoisted out of the loop | 89 | 1.2x |
| `-ffast-math -fopenmp-simd` → `libmvec` sincos | 17 | 6.4x |
| **what shipped**: hand-written AVX2 sin/cos | **16.1** | **6.8x** |
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

The reduction is only exact while the reduced argument fits comfortably in a
double, so the vector path is taken only when `TAU * (|n| + |s|)` is at most
1e6 per wheel — that covers every sample in `legacy/` (the largest is 6.3e5)
and every wheel the UI reaches in normal use. Past that `flowery_points` falls
back to libm, which reduces exactly. A wheel set to a seven-figure tooth count
therefore still draws correctly, just slower.

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
the Intel Iris Pro, once warm.

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
