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
