# Geometry calculator

Produces beautiful geometry

Less than 10kB size! [*1]

## Requiremens:
```
brew install parallel gnuplot
```
or

```
sh ./requirements.sh
```

## Usage

```
./run.sh
```

or 

```
run.sh NUMBERYOUWANT
```

## For some nice samples press:

```
./samples.sh
open samples
```

## To generate just 1 image

x=480, y=648, z=816, plotsize=2001

```
mkdir tmp
./cx.sh 480 648 816 tmp/ 2001
open tmp
```

### Links

https://linuxgazette.net/133/luana.html
[*1]

```
wc -c * 
    1477 base.md
    1244 cx.sh
     378 launch.sh
      32 makedirectories.sh
     591 preparecommands.sh
     283 requirements.sh
     248 run.sh
     967 runner.sh
    3063 samples.sh
     684 subprepare.sh
     268 updatereadme.sh
    9235 total
```

## License
Copyright (C) 2019 Krzysztof Kowalski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.