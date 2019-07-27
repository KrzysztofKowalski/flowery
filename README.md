# Geometry calculator

Produces beautiful geometry

Only 5353 bytes in size!

```
maczito:fx-flower k$ du -ks . 
32	.

maczito:fx-flower k$ wc -c * 
     310 base.md
    1357 cx.sh
       0 im.log
     128 launch.sh
     488 preparecommands.sh
     522 runner.sh
    2276 samples.sh
     272 updatereadme.sh
    5353 total

maczito:fx-flower k$ ls -lha
total 64
drwxr-xr-x  11 k     wheel   352B 24 lip 23:43 .
drwxrwxrwt  14 root  wheel   448B 24 lip 23:42 ..
-rw-r--r--   1 k     wheel    44B 24 lip 23:43 .gitignore
-rw-r--r--   1 k     wheel   310B 24 lip 23:43 base.md
-rwxr-xr-x   1 k     wheel   1,3K 24 lip 23:43 cx.sh
-rw-r--r--   1 k     wheel     0B 24 lip 23:43 im.log
-rwxr-xr-x   1 k     wheel   128B 24 lip 23:43 launch.sh
-rwxr-xr-x   1 k     wheel   488B 24 lip 23:43 preparecommands.sh
-rwxr-xr-x   1 k     wheel   522B 24 lip 23:43 runner.sh
-rwxr-xr-x   1 k     wheel   2,2K 24 lip 23:43 samples.sh
-rwxr-xr-x   1 k     wheel   272B 24 lip 23:43 updatereadme.sh
```

## Requiremens:
```
brew install parallel gnuplot
```
## Usage

Generates entire set of 999^3 images

If you want to generate less, tweak parallel command to include less input.

```
./preparecommands.sh `pwd` out/images
./launch.sh
```

## For some nice samples press:

```
./samples.sh
open samples
```

## To generate just 1 image

x=480
y=648
z=816
plotsize=2001

```
mkdir tmp
./cx.sh 480 648 816 tmp/ 2001
open tmp
```

### Links

https://linuxgazette.net/133/luana.html

## Samples

Values X: 0001 Y: 0001 Z: 0284, P: 2001 
![0001 Y: 0001 Z: 0284, P: 2001](./samples/spiro-0001-0001-0284-2001.svg)
Values X: 0001 Y: 0001 Z: 0331, P: 2001 
![0001 Y: 0001 Z: 0331, P: 2001](./samples/spiro-0001-0001-0331-2001.svg)
Values X: 0001 Y: 0001 Z: 0332, P: 2001 
![0001 Y: 0001 Z: 0332, P: 2001](./samples/spiro-0001-0001-0332-2001.svg)
Values X: 0001 Y: 0001 Z: 0333, P: 2001 
![0001 Y: 0001 Z: 0333, P: 2001](./samples/spiro-0001-0001-0333-2001.svg)
Values X: 0001 Y: 0001 Z: 0363, P: 2001 
![0001 Y: 0001 Z: 0363, P: 2001](./samples/spiro-0001-0001-0363-2001.svg)
Values X: 0001 Y: 0001 Z: 0398, P: 2001 
![0001 Y: 0001 Z: 0398, P: 2001](./samples/spiro-0001-0001-0398-2001.svg)
Values X: 0001 Y: 0001 Z: 0399, P: 2001 
![0001 Y: 0001 Z: 0399, P: 2001](./samples/spiro-0001-0001-0399-2001.svg)
Values X: 0001 Y: 0001 Z: 0400, P: 2001 
![0001 Y: 0001 Z: 0400, P: 2001](./samples/spiro-0001-0001-0400-2001.svg)
Values X: 0001 Y: 0001 Z: 0443, P: 2001 
![0001 Y: 0001 Z: 0443, P: 2001](./samples/spiro-0001-0001-0443-2001.svg)
Values X: 0001 Y: 0001 Z: 0444, P: 2001 
![0001 Y: 0001 Z: 0444, P: 2001](./samples/spiro-0001-0001-0444-2001.svg)
Values X: 0001 Y: 0001 Z: 0445, P: 2001 
![0001 Y: 0001 Z: 0445, P: 2001](./samples/spiro-0001-0001-0445-2001.svg)
Values X: 0001 Y: 0001 Z: 0446, P: 2001 
![0001 Y: 0001 Z: 0446, P: 2001](./samples/spiro-0001-0001-0446-2001.svg)
Values X: 0001 Y: 0001 Z: 0447, P: 2001 
![0001 Y: 0001 Z: 0447, P: 2001](./samples/spiro-0001-0001-0447-2001.svg)
Values X: 0001 Y: 0001 Z: 0463, P: 2001 
![0001 Y: 0001 Z: 0463, P: 2001](./samples/spiro-0001-0001-0463-2001.svg)
Values X: 0001 Y: 0001 Z: 0464, P: 2001 
![0001 Y: 0001 Z: 0464, P: 2001](./samples/spiro-0001-0001-0464-2001.svg)
Values X: 0001 Y: 0001 Z: 0495, P: 2001 
![0001 Y: 0001 Z: 0495, P: 2001](./samples/spiro-0001-0001-0495-2001.svg)
Values X: 0001 Y: 0001 Z: 0496, P: 2001 
![0001 Y: 0001 Z: 0496, P: 2001](./samples/spiro-0001-0001-0496-2001.svg)
Values X: 0001 Y: 0001 Z: 0498, P: 2001 
![0001 Y: 0001 Z: 0498, P: 2001](./samples/spiro-0001-0001-0498-2001.svg)
Values X: 0001 Y: 0001 Z: 0499, P: 2001 
![0001 Y: 0001 Z: 0499, P: 2001](./samples/spiro-0001-0001-0499-2001.svg)
Values X: 0001 Y: 0001 Z: 0500, P: 2001 
![0001 Y: 0001 Z: 0500, P: 2001](./samples/spiro-0001-0001-0500-2001.svg)
Values X: 0001 Y: 0001 Z: 0501, P: 2001 
![0001 Y: 0001 Z: 0501, P: 2001](./samples/spiro-0001-0001-0501-2001.svg)
Values X: 0001 Y: 0001 Z: 0502, P: 2001 
![0001 Y: 0001 Z: 0502, P: 2001](./samples/spiro-0001-0001-0502-2001.svg)
Values X: 0001 Y: 0001 Z: 0503, P: 2001 
![0001 Y: 0001 Z: 0503, P: 2001](./samples/spiro-0001-0001-0503-2001.svg)
Values X: 0001 Y: 0001 Z: 0545, P: 2001 
![0001 Y: 0001 Z: 0545, P: 2001](./samples/spiro-0001-0001-0545-2001.svg)
Values X: 0001 Y: 0001 Z: 0547, P: 2001 
![0001 Y: 0001 Z: 0547, P: 2001](./samples/spiro-0001-0001-0547-2001.svg)
Values X: 0001 Y: 0001 Z: 0548, P: 2001 
![0001 Y: 0001 Z: 0548, P: 2001](./samples/spiro-0001-0001-0548-2001.svg)
Values X: 0001 Y: 0001 Z: 0570, P: 2001 
![0001 Y: 0001 Z: 0570, P: 2001](./samples/spiro-0001-0001-0570-2001.svg)
Values X: 0001 Y: 0001 Z: 0571, P: 2001 
![0001 Y: 0001 Z: 0571, P: 2001](./samples/spiro-0001-0001-0571-2001.svg)
Values X: 0001 Y: 0001 Z: 0572, P: 2001 
![0001 Y: 0001 Z: 0572, P: 2001](./samples/spiro-0001-0001-0572-2001.svg)
Values X: 0001 Y: 0001 Z: 0573, P: 2001 
![0001 Y: 0001 Z: 0573, P: 2001](./samples/spiro-0001-0001-0573-2001.svg)
Values X: 0001 Y: 0001 Z: 0574, P: 2001 
![0001 Y: 0001 Z: 0574, P: 2001](./samples/spiro-0001-0001-0574-2001.svg)
Values X: 0001 Y: 0001 Z: 0602, P: 2001 
![0001 Y: 0001 Z: 0602, P: 2001](./samples/spiro-0001-0001-0602-2001.svg)
Values X: 0001 Y: 0001 Z: 0661, P: 2001 
![0001 Y: 0001 Z: 0661, P: 2001](./samples/spiro-0001-0001-0661-2001.svg)
Values X: 0001 Y: 0001 Z: 0662, P: 2001 
![0001 Y: 0001 Z: 0662, P: 2001](./samples/spiro-0001-0001-0662-2001.svg)
Values X: 0001 Y: 0001 Z: 0663, P: 2001 
![0001 Y: 0001 Z: 0663, P: 2001](./samples/spiro-0001-0001-0663-2001.svg)
Values X: 0001 Y: 0001 Z: 0664, P: 2001 
![0001 Y: 0001 Z: 0664, P: 2001](./samples/spiro-0001-0001-0664-2001.svg)
Values X: 0001 Y: 0001 Z: 0665, P: 2001 
![0001 Y: 0001 Z: 0665, P: 2001](./samples/spiro-0001-0001-0665-2001.svg)
Values X: 0001 Y: 0001 Z: 0666, P: 2001 
![0001 Y: 0001 Z: 0666, P: 2001](./samples/spiro-0001-0001-0666-2001.svg)
Values X: 0001 Y: 0001 Z: 0667, P: 2001 
![0001 Y: 0001 Z: 0667, P: 2001](./samples/spiro-0001-0001-0667-2001.svg)
Values X: 0001 Y: 0001 Z: 0668, P: 2001 
![0001 Y: 0001 Z: 0668, P: 2001](./samples/spiro-0001-0001-0668-2001.svg)
Values X: 0001 Y: 0001 Z: 0669, P: 2001 
![0001 Y: 0001 Z: 0669, P: 2001](./samples/spiro-0001-0001-0669-2001.svg)
Values X: 0001 Y: 0001 Z: 0670, P: 2001 
![0001 Y: 0001 Z: 0670, P: 2001](./samples/spiro-0001-0001-0670-2001.svg)
Values X: 0001 Y: 0001 Z: 0671, P: 2001 
![0001 Y: 0001 Z: 0671, P: 2001](./samples/spiro-0001-0001-0671-2001.svg)
Values X: 0001 Y: 0001 Z: 0672, P: 2001 
![0001 Y: 0001 Z: 0672, P: 2001](./samples/spiro-0001-0001-0672-2001.svg)
Values X: 0001 Y: 0001 Z: 0716, P: 2001 
![0001 Y: 0001 Z: 0716, P: 2001](./samples/spiro-0001-0001-0716-2001.svg)
Values X: 0001 Y: 0001 Z: 0727, P: 2001 
![0001 Y: 0001 Z: 0727, P: 2001](./samples/spiro-0001-0001-0727-2001.svg)
Values X: 0001 Y: 0001 Z: 0728, P: 2001 
![0001 Y: 0001 Z: 0728, P: 2001](./samples/spiro-0001-0001-0728-2001.svg)
Values X: 0001 Y: 0001 Z: 0729, P: 2001 
![0001 Y: 0001 Z: 0729, P: 2001](./samples/spiro-0001-0001-0729-2001.svg)
Values X: 0001 Y: 0001 Z: 0730, P: 2001 
![0001 Y: 0001 Z: 0730, P: 2001](./samples/spiro-0001-0001-0730-2001.svg)
Values X: 0001 Y: 0001 Z: 0752, P: 2001 
![0001 Y: 0001 Z: 0752, P: 2001](./samples/spiro-0001-0001-0752-2001.svg)
Values X: 0001 Y: 0001 Z: 0798, P: 2001 
![0001 Y: 0001 Z: 0798, P: 2001](./samples/spiro-0001-0001-0798-2001.svg)
Values X: 0001 Y: 0001 Z: 0799, P: 2001 
![0001 Y: 0001 Z: 0799, P: 2001](./samples/spiro-0001-0001-0799-2001.svg)
Values X: 0001 Y: 0001 Z: 0800, P: 2001 
![0001 Y: 0001 Z: 0800, P: 2001](./samples/spiro-0001-0001-0800-2001.svg)
Values X: 0001 Y: 0001 Z: 0801, P: 2001 
![0001 Y: 0001 Z: 0801, P: 2001](./samples/spiro-0001-0001-0801-2001.svg)
Values X: 0001 Y: 0001 Z: 0802, P: 2001 
![0001 Y: 0001 Z: 0802, P: 2001](./samples/spiro-0001-0001-0802-2001.svg)
Values X: 0001 Y: 0001 Z: 0835, P: 2001 
![0001 Y: 0001 Z: 0835, P: 2001](./samples/spiro-0001-0001-0835-2001.svg)
Values X: 0001 Y: 0001 Z: 0855, P: 2001 
![0001 Y: 0001 Z: 0855, P: 2001](./samples/spiro-0001-0001-0855-2001.svg)
Values X: 0001 Y: 0001 Z: 0856, P: 2001 
![0001 Y: 0001 Z: 0856, P: 2001](./samples/spiro-0001-0001-0856-2001.svg)
Values X: 0001 Y: 0001 Z: 0857, P: 2001 
![0001 Y: 0001 Z: 0857, P: 2001](./samples/spiro-0001-0001-0857-2001.svg)
Values X: 0001 Y: 0001 Z: 0859, P: 2001 
![0001 Y: 0001 Z: 0859, P: 2001](./samples/spiro-0001-0001-0859-2001.svg)
Values X: 0001 Y: 0001 Z: 0860, P: 2001 
![0001 Y: 0001 Z: 0860, P: 2001](./samples/spiro-0001-0001-0860-2001.svg)
Values X: 0001 Y: 0001 Z: 0886, P: 2001 
![0001 Y: 0001 Z: 0886, P: 2001](./samples/spiro-0001-0001-0886-2001.svg)
Values X: 0001 Y: 0001 Z: 0887, P: 2001 
![0001 Y: 0001 Z: 0887, P: 2001](./samples/spiro-0001-0001-0887-2001.svg)
Values X: 0001 Y: 0001 Z: 0889, P: 2001 
![0001 Y: 0001 Z: 0889, P: 2001](./samples/spiro-0001-0001-0889-2001.svg)
Values X: 0001 Y: 0001 Z: 0891, P: 2001 
![0001 Y: 0001 Z: 0891, P: 2001](./samples/spiro-0001-0001-0891-2001.svg)
Values X: 0001 Y: 0001 Z: 0911, P: 2001 
![0001 Y: 0001 Z: 0911, P: 2001](./samples/spiro-0001-0001-0911-2001.svg)
Values X: 0001 Y: 0001 Z: 0912, P: 2001 
![0001 Y: 0001 Z: 0912, P: 2001](./samples/spiro-0001-0001-0912-2001.svg)
Values X: 0001 Y: 0001 Z: 0925, P: 2001 
![0001 Y: 0001 Z: 0925, P: 2001](./samples/spiro-0001-0001-0925-2001.svg)
Values X: 0001 Y: 0001 Z: 0935, P: 2001 
![0001 Y: 0001 Z: 0935, P: 2001](./samples/spiro-0001-0001-0935-2001.svg)
Values X: 0001 Y: 0001 Z: 0939, P: 2001 
![0001 Y: 0001 Z: 0939, P: 2001](./samples/spiro-0001-0001-0939-2001.svg)
Values X: 0001 Y: 0001 Z: 0943, P: 2001 
![0001 Y: 0001 Z: 0943, P: 2001](./samples/spiro-0001-0001-0943-2001.svg)
Values X: 0001 Y: 0001 Z: 0994, P: 2001 
![0001 Y: 0001 Z: 0994, P: 2001](./samples/spiro-0001-0001-0994-2001.svg)
Values X: 0001 Y: 0001 Z: 0995, P: 2001 
![0001 Y: 0001 Z: 0995, P: 2001](./samples/spiro-0001-0001-0995-2001.svg)
Values X: 0001 Y: 0001 Z: 0996, P: 2001 
![0001 Y: 0001 Z: 0996, P: 2001](./samples/spiro-0001-0001-0996-2001.svg)
Values X: 0001 Y: 0001 Z: 0997, P: 2001 
![0001 Y: 0001 Z: 0997, P: 2001](./samples/spiro-0001-0001-0997-2001.svg)
Values X: 0001 Y: 0001 Z: 0998, P: 2001 
![0001 Y: 0001 Z: 0998, P: 2001](./samples/spiro-0001-0001-0998-2001.svg)
Values X: 0001 Y: 0001 Z: 0999, P: 2001 
![0001 Y: 0001 Z: 0999, P: 2001](./samples/spiro-0001-0001-0999-2001.svg)
Values X: 0001 Y: 0001 Z: 1000, P: 2001 
![0001 Y: 0001 Z: 1000, P: 2001](./samples/spiro-0001-0001-1000-2001.svg)
Values X: 0001 Y: 0001 Z: 1001, P: 2001 
![0001 Y: 0001 Z: 1001, P: 2001](./samples/spiro-0001-0001-1001-2001.svg)
Values X: 0001 Y: 0001 Z: 1002, P: 2001 
![0001 Y: 0001 Z: 1002, P: 2001](./samples/spiro-0001-0001-1002-2001.svg)
Values X: 0001 Y: 0001 Z: 1003, P: 2001 
![0001 Y: 0001 Z: 1003, P: 2001](./samples/spiro-0001-0001-1003-2001.svg)
Values X: 0001 Y: 0001 Z: 1004, P: 2001 
![0001 Y: 0001 Z: 1004, P: 2001](./samples/spiro-0001-0001-1004-2001.svg)
Values X: 0001 Y: 0001 Z: 1005, P: 2001 
![0001 Y: 0001 Z: 1005, P: 2001](./samples/spiro-0001-0001-1005-2001.svg)
Values X: 0001 Y: 0001 Z: 1006, P: 2001 
![0001 Y: 0001 Z: 1006, P: 2001](./samples/spiro-0001-0001-1006-2001.svg)
Values X: 0001 Y: 0001 Z: 1007, P: 2001 
![0001 Y: 0001 Z: 1007, P: 2001](./samples/spiro-0001-0001-1007-2001.svg)
Values X: 0001 Y: 0001 Z: 1008, P: 2001 
![0001 Y: 0001 Z: 1008, P: 2001](./samples/spiro-0001-0001-1008-2001.svg)
Values X: 0001 Y: 0001 Z: 1197, P: 2001 
![0001 Y: 0001 Z: 1197, P: 2001](./samples/spiro-0001-0001-1197-2001.svg)
Values X: 0001 Y: 0001 Z: 1198, P: 2001 
![0001 Y: 0001 Z: 1198, P: 2001](./samples/spiro-0001-0001-1198-2001.svg)
Values X: 0001 Y: 0001 Z: 1199, P: 2001 
![0001 Y: 0001 Z: 1199, P: 2001](./samples/spiro-0001-0001-1199-2001.svg)
Values X: 0001 Y: 0001 Z: 1200, P: 2001 
![0001 Y: 0001 Z: 1200, P: 2001](./samples/spiro-0001-0001-1200-2001.svg)
Values X: 0001 Y: 0001 Z: 1201, P: 2001 
![0001 Y: 0001 Z: 1201, P: 2001](./samples/spiro-0001-0001-1201-2001.svg)
Values X: 0001 Y: 0001 Z: 1202, P: 2001 
![0001 Y: 0001 Z: 1202, P: 2001](./samples/spiro-0001-0001-1202-2001.svg)
Values X: 0001 Y: 0001 Z: 1203, P: 2001 
![0001 Y: 0001 Z: 1203, P: 2001](./samples/spiro-0001-0001-1203-2001.svg)
Values X: 0001 Y: 0001 Z: 1252, P: 2001 
![0001 Y: 0001 Z: 1252, P: 2001](./samples/spiro-0001-0001-1252-2001.svg)
Values X: 0001 Y: 0001 Z: 1329, P: 2001 
![0001 Y: 0001 Z: 1329, P: 2001](./samples/spiro-0001-0001-1329-2001.svg)
Values X: 0001 Y: 0001 Z: 1330, P: 2001 
![0001 Y: 0001 Z: 1330, P: 2001](./samples/spiro-0001-0001-1330-2001.svg)
Values X: 0001 Y: 0001 Z: 1331, P: 2001 
![0001 Y: 0001 Z: 1331, P: 2001](./samples/spiro-0001-0001-1331-2001.svg)
Values X: 0001 Y: 0001 Z: 1332, P: 2001 
![0001 Y: 0001 Z: 1332, P: 2001](./samples/spiro-0001-0001-1332-2001.svg)
Values X: 0001 Y: 0001 Z: 1333, P: 2001 
![0001 Y: 0001 Z: 1333, P: 2001](./samples/spiro-0001-0001-1333-2001.svg)
Values X: 0001 Y: 0001 Z: 1334, P: 2001 
![0001 Y: 0001 Z: 1334, P: 2001](./samples/spiro-0001-0001-1334-2001.svg)
Values X: 0001 Y: 0001 Z: 1335, P: 2001 
![0001 Y: 0001 Z: 1335, P: 2001](./samples/spiro-0001-0001-1335-2001.svg)
Values X: 0001 Y: 0001 Z: 1336, P: 2001 
![0001 Y: 0001 Z: 1336, P: 2001](./samples/spiro-0001-0001-1336-2001.svg)
Values X: 0001 Y: 0001 Z: 1497, P: 2001 
![0001 Y: 0001 Z: 1497, P: 2001](./samples/spiro-0001-0001-1497-2001.svg)
Values X: 0001 Y: 0001 Z: 1498, P: 2001 
![0001 Y: 0001 Z: 1498, P: 2001](./samples/spiro-0001-0001-1498-2001.svg)
Values X: 0001 Y: 0001 Z: 1499, P: 2001 
![0001 Y: 0001 Z: 1499, P: 2001](./samples/spiro-0001-0001-1499-2001.svg)
Values X: 0001 Y: 0001 Z: 1500, P: 2001 
![0001 Y: 0001 Z: 1500, P: 2001](./samples/spiro-0001-0001-1500-2001.svg)
Values X: 0001 Y: 0001 Z: 1501, P: 2001 
![0001 Y: 0001 Z: 1501, P: 2001](./samples/spiro-0001-0001-1501-2001.svg)
Values X: 0001 Y: 0001 Z: 1502, P: 2001 
![0001 Y: 0001 Z: 1502, P: 2001](./samples/spiro-0001-0001-1502-2001.svg)
Values X: 0001 Y: 0001 Z: 1503, P: 2001 
![0001 Y: 0001 Z: 1503, P: 2001](./samples/spiro-0001-0001-1503-2001.svg)
Values X: 0001 Y: 0001 Z: 1991, P: 2001 
![0001 Y: 0001 Z: 1991, P: 2001](./samples/spiro-0001-0001-1991-2001.svg)
Values X: 0001 Y: 0001 Z: 1992, P: 2001 
![0001 Y: 0001 Z: 1992, P: 2001](./samples/spiro-0001-0001-1992-2001.svg)
Values X: 0001 Y: 0001 Z: 1993, P: 2001 
![0001 Y: 0001 Z: 1993, P: 2001](./samples/spiro-0001-0001-1993-2001.svg)
Values X: 0001 Y: 0001 Z: 1994, P: 2001 
![0001 Y: 0001 Z: 1994, P: 2001](./samples/spiro-0001-0001-1994-2001.svg)
Values X: 0001 Y: 0001 Z: 1995, P: 2001 
![0001 Y: 0001 Z: 1995, P: 2001](./samples/spiro-0001-0001-1995-2001.svg)
Values X: 0001 Y: 0001 Z: 1996, P: 2001 
![0001 Y: 0001 Z: 1996, P: 2001](./samples/spiro-0001-0001-1996-2001.svg)
Values X: 0001 Y: 0001 Z: 1997, P: 2001 
![0001 Y: 0001 Z: 1997, P: 2001](./samples/spiro-0001-0001-1997-2001.svg)
Values X: 0001 Y: 0001 Z: 1998, P: 2001 
![0001 Y: 0001 Z: 1998, P: 2001](./samples/spiro-0001-0001-1998-2001.svg)
Values X: 0001 Y: 0001 Z: 1999, P: 2001 
![0001 Y: 0001 Z: 1999, P: 2001](./samples/spiro-0001-0001-1999-2001.svg)
Values X: 0001 Y: 0001 Z: 1999, P: 9999 
![0001 Y: 0001 Z: 1999, P: 9999](./samples/spiro-0001-0001-1999-9999.svg)
Values X: 0001 Y: 0001 Z: 2000, P: 9999 
![0001 Y: 0001 Z: 2000, P: 9999](./samples/spiro-0001-0001-2000-9999.svg)
Values X: 0001 Y: 0001 Z: 2001, P: 9999 
![0001 Y: 0001 Z: 2001, P: 9999](./samples/spiro-0001-0001-2001-9999.svg)
Values X: 0001 Y: 0001 Z: 2103, P: 9999 
![0001 Y: 0001 Z: 2103, P: 9999](./samples/spiro-0001-0001-2103-9999.svg)
Values X: 0001 Y: 0001 Z: 2498, P: 9999 
![0001 Y: 0001 Z: 2498, P: 9999](./samples/spiro-0001-0001-2498-9999.svg)
Values X: 0001 Y: 0001 Z: 2499, P: 9999 
![0001 Y: 0001 Z: 2499, P: 9999](./samples/spiro-0001-0001-2499-9999.svg)
Values X: 0001 Y: 0001 Z: 2500, P: 9999 
![0001 Y: 0001 Z: 2500, P: 9999](./samples/spiro-0001-0001-2500-9999.svg)
Values X: 0001 Y: 0001 Z: 2501, P: 9999 
![0001 Y: 0001 Z: 2501, P: 9999](./samples/spiro-0001-0001-2501-9999.svg)
Values X: 0001 Y: 0001 Z: 2502, P: 9999 
![0001 Y: 0001 Z: 2502, P: 9999](./samples/spiro-0001-0001-2502-9999.svg)
Values X: 0001 Y: 0001 Z: 2854, P: 9999 
![0001 Y: 0001 Z: 2854, P: 9999](./samples/spiro-0001-0001-2854-9999.svg)
Values X: 0001 Y: 0001 Z: 2856, P: 9999 
![0001 Y: 0001 Z: 2856, P: 9999](./samples/spiro-0001-0001-2856-9999.svg)
Values X: 0001 Y: 0001 Z: 2858, P: 9999 
![0001 Y: 0001 Z: 2858, P: 9999](./samples/spiro-0001-0001-2858-9999.svg)
Values X: 0001 Y: 0001 Z: 3001, P: 9999 
![0001 Y: 0001 Z: 3001, P: 9999](./samples/spiro-0001-0001-3001-9999.svg)
Values X: 0001 Y: 0001 Z: 3002, P: 9999 
![0001 Y: 0001 Z: 3002, P: 9999](./samples/spiro-0001-0001-3002-9999.svg)
Values X: 0001 Y: 0001 Z: 3329, P: 9999 
![0001 Y: 0001 Z: 3329, P: 9999](./samples/spiro-0001-0001-3329-9999.svg)
Values X: 0001 Y: 0001 Z: 3331, P: 9999 
![0001 Y: 0001 Z: 3331, P: 9999](./samples/spiro-0001-0001-3331-9999.svg)
Values X: 0001 Y: 0001 Z: 3332, P: 9999 
![0001 Y: 0001 Z: 3332, P: 9999](./samples/spiro-0001-0001-3332-9999.svg)
Values X: 0001 Y: 0001 Z: 3333, P: 9999 
![0001 Y: 0001 Z: 3333, P: 9999](./samples/spiro-0001-0001-3333-9999.svg)
Values X: 0001 Y: 0001 Z: 3334, P: 9999 
![0001 Y: 0001 Z: 3334, P: 9999](./samples/spiro-0001-0001-3334-9999.svg)
Values X: 0001 Y: 0001 Z: 3335, P: 9999 
![0001 Y: 0001 Z: 3335, P: 9999](./samples/spiro-0001-0001-3335-9999.svg)
Values X: 0001 Y: 0001 Z: 3336, P: 9999 
![0001 Y: 0001 Z: 3336, P: 9999](./samples/spiro-0001-0001-3336-9999.svg)
Values X: 0001 Y: 0001 Z: 3337, P: 9999 
![0001 Y: 0001 Z: 3337, P: 9999](./samples/spiro-0001-0001-3337-9999.svg)
Values X: 0001 Y: 0001 Z: 3637, P: 9999 
![0001 Y: 0001 Z: 3637, P: 9999](./samples/spiro-0001-0001-3637-9999.svg)
Values X: 0001 Y: 0001 Z: 3638, P: 9999 
![0001 Y: 0001 Z: 3638, P: 9999](./samples/spiro-0001-0001-3638-9999.svg)
Values X: 0001 Y: 0001 Z: 3746, P: 9999 
![0001 Y: 0001 Z: 3746, P: 9999](./samples/spiro-0001-0001-3746-9999.svg)
Values X: 0001 Y: 0001 Z: 3747, P: 9999 
![0001 Y: 0001 Z: 3747, P: 9999](./samples/spiro-0001-0001-3747-9999.svg)
Values X: 0001 Y: 0001 Z: 3748, P: 9999 
![0001 Y: 0001 Z: 3748, P: 9999](./samples/spiro-0001-0001-3748-9999.svg)
Values X: 0001 Y: 0001 Z: 3751, P: 9999 
![0001 Y: 0001 Z: 3751, P: 9999](./samples/spiro-0001-0001-3751-9999.svg)
Values X: 0001 Y: 0001 Z: 3752, P: 9999 
![0001 Y: 0001 Z: 3752, P: 9999](./samples/spiro-0001-0001-3752-9999.svg)
Values X: 0001 Y: 0001 Z: 3847, P: 9999 
![0001 Y: 0001 Z: 3847, P: 9999](./samples/spiro-0001-0001-3847-9999.svg)
Values X: 0001 Y: 0001 Z: 3994, P: 9999 
![0001 Y: 0001 Z: 3994, P: 9999](./samples/spiro-0001-0001-3994-9999.svg)
Values X: 0001 Y: 0001 Z: 3996, P: 9999 
![0001 Y: 0001 Z: 3996, P: 9999](./samples/spiro-0001-0001-3996-9999.svg)
Values X: 0001 Y: 0001 Z: 3997, P: 9999 
![0001 Y: 0001 Z: 3997, P: 9999](./samples/spiro-0001-0001-3997-9999.svg)
Values X: 0001 Y: 0001 Z: 3998, P: 9999 
![0001 Y: 0001 Z: 3998, P: 9999](./samples/spiro-0001-0001-3998-9999.svg)
Values X: 0001 Y: 0001 Z: 4001, P: 9999 
![0001 Y: 0001 Z: 4001, P: 9999](./samples/spiro-0001-0001-4001-9999.svg)
Values X: 0001 Y: 0001 Z: 4002, P: 9999 
![0001 Y: 0001 Z: 4002, P: 9999](./samples/spiro-0001-0001-4002-9999.svg)
Values X: 0001 Y: 0001 Z: 4282, P: 9999 
![0001 Y: 0001 Z: 4282, P: 9999](./samples/spiro-0001-0001-4282-9999.svg)
Values X: 0001 Y: 0001 Z: 4284, P: 9999 
![0001 Y: 0001 Z: 4284, P: 9999](./samples/spiro-0001-0001-4284-9999.svg)
Values X: 0001 Y: 0001 Z: 4287, P: 9999 
![0001 Y: 0001 Z: 4287, P: 9999](./samples/spiro-0001-0001-4287-9999.svg)
Values X: 0001 Y: 0001 Z: 4288, P: 9999 
![0001 Y: 0001 Z: 4288, P: 9999](./samples/spiro-0001-0001-4288-9999.svg)
Values X: 0001 Y: 0001 Z: 4372, P: 9999 
![0001 Y: 0001 Z: 4372, P: 9999](./samples/spiro-0001-0001-4372-9999.svg)
Values X: 0001 Y: 0001 Z: 4441, P: 9999 
![0001 Y: 0001 Z: 4441, P: 9999](./samples/spiro-0001-0001-4441-9999.svg)
Values X: 0001 Y: 0001 Z: 4442, P: 9999 
![0001 Y: 0001 Z: 4442, P: 9999](./samples/spiro-0001-0001-4442-9999.svg)
Values X: 0001 Y: 0001 Z: 4445, P: 9999 
![0001 Y: 0001 Z: 4445, P: 9999](./samples/spiro-0001-0001-4445-9999.svg)
Values X: 0001 Y: 0001 Z: 4446, P: 9999 
![0001 Y: 0001 Z: 4446, P: 9999](./samples/spiro-0001-0001-4446-9999.svg)
Values X: 0001 Y: 0001 Z: 4542, P: 9999 
![0001 Y: 0001 Z: 4542, P: 9999](./samples/spiro-0001-0001-4542-9999.svg)
Values X: 0001 Y: 0001 Z: 4546, P: 9999 
![0001 Y: 0001 Z: 4546, P: 9999](./samples/spiro-0001-0001-4546-9999.svg)
Values X: 0001 Y: 0001 Z: 4616, P: 9999 
![0001 Y: 0001 Z: 4616, P: 9999](./samples/spiro-0001-0001-4616-9999.svg)
Values X: 0001 Y: 0001 Z: 4990, P: 9999 
![0001 Y: 0001 Z: 4990, P: 9999](./samples/spiro-0001-0001-4990-9999.svg)
Values X: 0001 Y: 0001 Z: 4991, P: 9999 
![0001 Y: 0001 Z: 4991, P: 9999](./samples/spiro-0001-0001-4991-9999.svg)
Values X: 0001 Y: 0001 Z: 4992, P: 9999 
![0001 Y: 0001 Z: 4992, P: 9999](./samples/spiro-0001-0001-4992-9999.svg)
Values X: 0001 Y: 0001 Z: 4993, P: 9999 
![0001 Y: 0001 Z: 4993, P: 9999](./samples/spiro-0001-0001-4993-9999.svg)
Values X: 0001 Y: 0001 Z: 4994, P: 9999 
![0001 Y: 0001 Z: 4994, P: 9999](./samples/spiro-0001-0001-4994-9999.svg)
Values X: 0001 Y: 0001 Z: 4995, P: 9999 
![0001 Y: 0001 Z: 4995, P: 9999](./samples/spiro-0001-0001-4995-9999.svg)
Values X: 0001 Y: 0001 Z: 4996, P: 9999 
![0001 Y: 0001 Z: 4996, P: 9999](./samples/spiro-0001-0001-4996-9999.svg)
Values X: 0001 Y: 0001 Z: 4997, P: 9999 
![0001 Y: 0001 Z: 4997, P: 9999](./samples/spiro-0001-0001-4997-9999.svg)
Values X: 0001 Y: 0001 Z: 4998, P: 9999 
![0001 Y: 0001 Z: 4998, P: 9999](./samples/spiro-0001-0001-4998-9999.svg)
Values X: 0001 Y: 0001 Z: 4999, P: 9999 
![0001 Y: 0001 Z: 4999, P: 9999](./samples/spiro-0001-0001-4999-9999.svg)
Values X: 0001 Y: 0001 Z: 5000, P: 9999 
![0001 Y: 0001 Z: 5000, P: 9999](./samples/spiro-0001-0001-5000-9999.svg)
Values X: 0001 Y: 0001 Z: 5001, P: 9999 
![0001 Y: 0001 Z: 5001, P: 9999](./samples/spiro-0001-0001-5001-9999.svg)
Values X: 0001 Y: 0001 Z: 5002, P: 9999 
![0001 Y: 0001 Z: 5002, P: 9999](./samples/spiro-0001-0001-5002-9999.svg)
Values X: 0001 Y: 0001 Z: 5003, P: 9999 
![0001 Y: 0001 Z: 5003, P: 9999](./samples/spiro-0001-0001-5003-9999.svg)
Values X: 0001 Y: 0001 Z: 5004, P: 9999 
![0001 Y: 0001 Z: 5004, P: 9999](./samples/spiro-0001-0001-5004-9999.svg)
Values X: 0001 Y: 0001 Z: 5005, P: 9999 
![0001 Y: 0001 Z: 5005, P: 9999](./samples/spiro-0001-0001-5005-9999.svg)
Values X: 0001 Y: 0001 Z: 5006, P: 9999 
![0001 Y: 0001 Z: 5006, P: 9999](./samples/spiro-0001-0001-5006-9999.svg)
Values X: 0001 Y: 0001 Z: 5214, P: 9999 
![0001 Y: 0001 Z: 5214, P: 9999](./samples/spiro-0001-0001-5214-9999.svg)
Values X: 0001 Y: 0001 Z: 5239, P: 9999 
![0001 Y: 0001 Z: 5239, P: 9999](./samples/spiro-0001-0001-5239-9999.svg)
Values X: 0001 Y: 0001 Z: 5260, P: 9999 
![0001 Y: 0001 Z: 5260, P: 9999](./samples/spiro-0001-0001-5260-9999.svg)
Values X: 0001 Y: 0001 Z: 5291, P: 9999 
![0001 Y: 0001 Z: 5291, P: 9999](./samples/spiro-0001-0001-5291-9999.svg)
Values X: 0001 Y: 0001 Z: 5295, P: 9999 
![0001 Y: 0001 Z: 5295, P: 9999](./samples/spiro-0001-0001-5295-9999.svg)
Values X: 0001 Y: 0001 Z: 5334, P: 9999 
![0001 Y: 0001 Z: 5334, P: 9999](./samples/spiro-0001-0001-5334-9999.svg)
Values X: 0001 Y: 0001 Z: 5335, P: 9999 
![0001 Y: 0001 Z: 5335, P: 9999](./samples/spiro-0001-0001-5335-9999.svg)
Values X: 0001 Y: 0001 Z: 5381, P: 9999 
![0001 Y: 0001 Z: 5381, P: 9999](./samples/spiro-0001-0001-5381-9999.svg)
Values X: 0001 Y: 0001 Z: 5382, P: 9999 
![0001 Y: 0001 Z: 5382, P: 9999](./samples/spiro-0001-0001-5382-9999.svg)
Values X: 0001 Y: 0001 Z: 5385, P: 9999 
![0001 Y: 0001 Z: 5385, P: 9999](./samples/spiro-0001-0001-5385-9999.svg)
Values X: 0001 Y: 0001 Z: 5386, P: 9999 
![0001 Y: 0001 Z: 5386, P: 9999](./samples/spiro-0001-0001-5386-9999.svg)
Values X: 0001 Y: 0121 Z: 0011, P: 2001 
![0001 Y: 0121 Z: 0011, P: 2001](./samples/spiro-0001-0121-0011-2001.svg)
Values X: 0001 Y: 0121 Z: 0016, P: 2001 
![0001 Y: 0121 Z: 0016, P: 2001](./samples/spiro-0001-0121-0016-2001.svg)
Values X: 0001 Y: 0121 Z: 0039, P: 2001 
![0001 Y: 0121 Z: 0039, P: 2001](./samples/spiro-0001-0121-0039-2001.svg)
Values X: 0001 Y: 0121 Z: 0040, P: 2001 
![0001 Y: 0121 Z: 0040, P: 2001](./samples/spiro-0001-0121-0040-2001.svg)
Values X: 0001 Y: 0121 Z: 0059, P: 2001 
![0001 Y: 0121 Z: 0059, P: 2001](./samples/spiro-0001-0121-0059-2001.svg)
Values X: 0001 Y: 0121 Z: 0060, P: 2001 
![0001 Y: 0121 Z: 0060, P: 2001](./samples/spiro-0001-0121-0060-2001.svg)
Values X: 0001 Y: 0121 Z: 0074, P: 2001 
![0001 Y: 0121 Z: 0074, P: 2001](./samples/spiro-0001-0121-0074-2001.svg)
Values X: 0001 Y: 0121 Z: 0082, P: 2001 
![0001 Y: 0121 Z: 0082, P: 2001](./samples/spiro-0001-0121-0082-2001.svg)
Values X: 0001 Y: 0121 Z: 0084, P: 2001 
![0001 Y: 0121 Z: 0084, P: 2001](./samples/spiro-0001-0121-0084-2001.svg)
Values X: 0001 Y: 0121 Z: 0092, P: 2001 
![0001 Y: 0121 Z: 0092, P: 2001](./samples/spiro-0001-0121-0092-2001.svg)
Values X: 0001 Y: 0121 Z: 0093, P: 2001 
![0001 Y: 0121 Z: 0093, P: 2001](./samples/spiro-0001-0121-0093-2001.svg)
Values X: 0001 Y: 0121 Z: 0094, P: 2001 
![0001 Y: 0121 Z: 0094, P: 2001](./samples/spiro-0001-0121-0094-2001.svg)
Values X: 0001 Y: 0121 Z: 0098, P: 2001 
![0001 Y: 0121 Z: 0098, P: 2001](./samples/spiro-0001-0121-0098-2001.svg)
Values X: 0001 Y: 0121 Z: 0109, P: 2001 
![0001 Y: 0121 Z: 0109, P: 2001](./samples/spiro-0001-0121-0109-2001.svg)
Values X: 0001 Y: 0121 Z: 0115, P: 2001 
![0001 Y: 0121 Z: 0115, P: 2001](./samples/spiro-0001-0121-0115-2001.svg)
Values X: 0001 Y: 0121 Z: 0116, P: 2001 
![0001 Y: 0121 Z: 0116, P: 2001](./samples/spiro-0001-0121-0116-2001.svg)
Values X: 0001 Y: 0121 Z: 0118, P: 2001 
![0001 Y: 0121 Z: 0118, P: 2001](./samples/spiro-0001-0121-0118-2001.svg)
Values X: 0001 Y: 0121 Z: 0125, P: 2001 
![0001 Y: 0121 Z: 0125, P: 2001](./samples/spiro-0001-0121-0125-2001.svg)
Values X: 0001 Y: 0121 Z: 0129, P: 2001 
![0001 Y: 0121 Z: 0129, P: 2001](./samples/spiro-0001-0121-0129-2001.svg)
Values X: 0001 Y: 0121 Z: 0130, P: 2001 
![0001 Y: 0121 Z: 0130, P: 2001](./samples/spiro-0001-0121-0130-2001.svg)
Values X: 0001 Y: 0121 Z: 0131, P: 2001 
![0001 Y: 0121 Z: 0131, P: 2001](./samples/spiro-0001-0121-0131-2001.svg)
Values X: 0001 Y: 0121 Z: 0136, P: 2001 
![0001 Y: 0121 Z: 0136, P: 2001](./samples/spiro-0001-0121-0136-2001.svg)
Values X: 0001 Y: 0121 Z: 0140, P: 2001 
![0001 Y: 0121 Z: 0140, P: 2001](./samples/spiro-0001-0121-0140-2001.svg)
Values X: 0001 Y: 0121 Z: 0144, P: 2001 
![0001 Y: 0121 Z: 0144, P: 2001](./samples/spiro-0001-0121-0144-2001.svg)
Values X: 0001 Y: 0121 Z: 0160, P: 2001 
![0001 Y: 0121 Z: 0160, P: 2001](./samples/spiro-0001-0121-0160-2001.svg)
Values X: 0001 Y: 0121 Z: 0163, P: 2001 
![0001 Y: 0121 Z: 0163, P: 2001](./samples/spiro-0001-0121-0163-2001.svg)
Values X: 0001 Y: 0121 Z: 0171, P: 2001 
![0001 Y: 0121 Z: 0171, P: 2001](./samples/spiro-0001-0121-0171-2001.svg)
Values X: 0001 Y: 0121 Z: 0183, P: 2001 
![0001 Y: 0121 Z: 0183, P: 2001](./samples/spiro-0001-0121-0183-2001.svg)
Values X: 0001 Y: 0121 Z: 0203, P: 2001 
![0001 Y: 0121 Z: 0203, P: 2001](./samples/spiro-0001-0121-0203-2001.svg)
Values X: 0001 Y: 0121 Z: 0216, P: 2001 
![0001 Y: 0121 Z: 0216, P: 2001](./samples/spiro-0001-0121-0216-2001.svg)
Values X: 0001 Y: 0121 Z: 0220, P: 2001 
![0001 Y: 0121 Z: 0220, P: 2001](./samples/spiro-0001-0121-0220-2001.svg)
Values X: 0001 Y: 0121 Z: 0235, P: 2001 
![0001 Y: 0121 Z: 0235, P: 2001](./samples/spiro-0001-0121-0235-2001.svg)
Values X: 0001 Y: 0121 Z: 0236, P: 2001 
![0001 Y: 0121 Z: 0236, P: 2001](./samples/spiro-0001-0121-0236-2001.svg)
Values X: 0001 Y: 0121 Z: 0237, P: 2001 
![0001 Y: 0121 Z: 0237, P: 2001](./samples/spiro-0001-0121-0237-2001.svg)
Values X: 0001 Y: 0121 Z: 0238, P: 2001 
![0001 Y: 0121 Z: 0238, P: 2001](./samples/spiro-0001-0121-0238-2001.svg)
Values X: 0001 Y: 0121 Z: 0239, P: 2001 
![0001 Y: 0121 Z: 0239, P: 2001](./samples/spiro-0001-0121-0239-2001.svg)
Values X: 0001 Y: 0121 Z: 0244, P: 2001 
![0001 Y: 0121 Z: 0244, P: 2001](./samples/spiro-0001-0121-0244-2001.svg)
Values X: 0001 Y: 0121 Z: 0245, P: 2001 
![0001 Y: 0121 Z: 0245, P: 2001](./samples/spiro-0001-0121-0245-2001.svg)
Values X: 0001 Y: 0121 Z: 0246, P: 2001 
![0001 Y: 0121 Z: 0246, P: 2001](./samples/spiro-0001-0121-0246-2001.svg)
Values X: 0001 Y: 0121 Z: 0247, P: 2001 
![0001 Y: 0121 Z: 0247, P: 2001](./samples/spiro-0001-0121-0247-2001.svg)
Values X: 0001 Y: 0121 Z: 0280, P: 2001 
![0001 Y: 0121 Z: 0280, P: 2001](./samples/spiro-0001-0121-0280-2001.svg)
Values X: 0001 Y: 0121 Z: 0304, P: 2001 
![0001 Y: 0121 Z: 0304, P: 2001](./samples/spiro-0001-0121-0304-2001.svg)
Values X: 0001 Y: 0121 Z: 0305, P: 2001 
![0001 Y: 0121 Z: 0305, P: 2001](./samples/spiro-0001-0121-0305-2001.svg)
Values X: 0001 Y: 0121 Z: 0321, P: 2001 
![0001 Y: 0121 Z: 0321, P: 2001](./samples/spiro-0001-0121-0321-2001.svg)
Values X: 0001 Y: 0121 Z: 0336, P: 2001 
![0001 Y: 0121 Z: 0336, P: 2001](./samples/spiro-0001-0121-0336-2001.svg)
Values X: 0001 Y: 0121 Z: 0359, P: 2001 
![0001 Y: 0121 Z: 0359, P: 2001](./samples/spiro-0001-0121-0359-2001.svg)
Values X: 0001 Y: 0121 Z: 0363, P: 2001 
![0001 Y: 0121 Z: 0363, P: 2001](./samples/spiro-0001-0121-0363-2001.svg)
Values X: 0001 Y: 0121 Z: 0365, P: 2001 
![0001 Y: 0121 Z: 0365, P: 2001](./samples/spiro-0001-0121-0365-2001.svg)
Values X: 0001 Y: 0121 Z: 0366, P: 2001 
![0001 Y: 0121 Z: 0366, P: 2001](./samples/spiro-0001-0121-0366-2001.svg)
Values X: 0001 Y: 0121 Z: 0367, P: 2001 
![0001 Y: 0121 Z: 0367, P: 2001](./samples/spiro-0001-0121-0367-2001.svg)
Values X: 0001 Y: 0121 Z: 0368, P: 2001 
![0001 Y: 0121 Z: 0368, P: 2001](./samples/spiro-0001-0121-0368-2001.svg)
Values X: 0001 Y: 0121 Z: 0441, P: 2001 
![0001 Y: 0121 Z: 0441, P: 2001](./samples/spiro-0001-0121-0441-2001.svg)
Values X: 0001 Y: 0121 Z: 0478, P: 2001 
![0001 Y: 0121 Z: 0478, P: 2001](./samples/spiro-0001-0121-0478-2001.svg)
Values X: 0001 Y: 0121 Z: 0481, P: 2001 
![0001 Y: 0121 Z: 0481, P: 2001](./samples/spiro-0001-0121-0481-2001.svg)
Values X: 0001 Y: 0121 Z: 0485, P: 2001 
![0001 Y: 0121 Z: 0485, P: 2001](./samples/spiro-0001-0121-0485-2001.svg)
Values X: 0001 Y: 0121 Z: 0486, P: 2001 
![0001 Y: 0121 Z: 0486, P: 2001](./samples/spiro-0001-0121-0486-2001.svg)
Values X: 0001 Y: 0121 Z: 0487, P: 2001 
![0001 Y: 0121 Z: 0487, P: 2001](./samples/spiro-0001-0121-0487-2001.svg)
Values X: 0001 Y: 0121 Z: 0488, P: 2001 
![0001 Y: 0121 Z: 0488, P: 2001](./samples/spiro-0001-0121-0488-2001.svg)
Values X: 0001 Y: 0121 Z: 0489, P: 2001 
![0001 Y: 0121 Z: 0489, P: 2001](./samples/spiro-0001-0121-0489-2001.svg)
Values X: 0001 Y: 0121 Z: 0499, P: 2001 
![0001 Y: 0121 Z: 0499, P: 2001](./samples/spiro-0001-0121-0499-2001.svg)
Values X: 0001 Y: 0121 Z: 0505, P: 2001 
![0001 Y: 0121 Z: 0505, P: 2001](./samples/spiro-0001-0121-0505-2001.svg)
Values X: 0001 Y: 0121 Z: 0521, P: 2001 
![0001 Y: 0121 Z: 0521, P: 2001](./samples/spiro-0001-0121-0521-2001.svg)
Values X: 0001 Y: 0121 Z: 0545, P: 2001 
![0001 Y: 0121 Z: 0545, P: 2001](./samples/spiro-0001-0121-0545-2001.svg)
Values X: 0001 Y: 0121 Z: 0585, P: 2001 
![0001 Y: 0121 Z: 0585, P: 2001](./samples/spiro-0001-0121-0585-2001.svg)
Values X: 0001 Y: 0121 Z: 0586, P: 2001 
![0001 Y: 0121 Z: 0586, P: 2001](./samples/spiro-0001-0121-0586-2001.svg)
Values X: 0001 Y: 0121 Z: 0605, P: 2001 
![0001 Y: 0121 Z: 0605, P: 2001](./samples/spiro-0001-0121-0605-2001.svg)
Values X: 0001 Y: 0121 Z: 0606, P: 2001 
![0001 Y: 0121 Z: 0606, P: 2001](./samples/spiro-0001-0121-0606-2001.svg)
Values X: 0001 Y: 0121 Z: 0607, P: 2001 
![0001 Y: 0121 Z: 0607, P: 2001](./samples/spiro-0001-0121-0607-2001.svg)
Values X: 0001 Y: 0121 Z: 0626, P: 2001 
![0001 Y: 0121 Z: 0626, P: 2001](./samples/spiro-0001-0121-0626-2001.svg)
Values X: 0001 Y: 0121 Z: 0654, P: 2001 
![0001 Y: 0121 Z: 0654, P: 2001](./samples/spiro-0001-0121-0654-2001.svg)
Values X: 0001 Y: 0121 Z: 0666, P: 2001 
![0001 Y: 0121 Z: 0666, P: 2001](./samples/spiro-0001-0121-0666-2001.svg)
Values X: 0001 Y: 0121 Z: 0667, P: 2001 
![0001 Y: 0121 Z: 0667, P: 2001](./samples/spiro-0001-0121-0667-2001.svg)
Values X: 0001 Y: 0121 Z: 0680, P: 2001 
![0001 Y: 0121 Z: 0680, P: 2001](./samples/spiro-0001-0121-0680-2001.svg)
Values X: 0001 Y: 0121 Z: 0683, P: 2001 
![0001 Y: 0121 Z: 0683, P: 2001](./samples/spiro-0001-0121-0683-2001.svg)
Values X: 0001 Y: 0121 Z: 0695, P: 2001 
![0001 Y: 0121 Z: 0695, P: 2001](./samples/spiro-0001-0121-0695-2001.svg)
Values X: 0001 Y: 0121 Z: 0698, P: 2001 
![0001 Y: 0121 Z: 0698, P: 2001](./samples/spiro-0001-0121-0698-2001.svg)
Values X: 0001 Y: 0121 Z: 0701, P: 2001 
![0001 Y: 0121 Z: 0701, P: 2001](./samples/spiro-0001-0121-0701-2001.svg)
Values X: 0001 Y: 0121 Z: 0706, P: 2001 
![0001 Y: 0121 Z: 0706, P: 2001](./samples/spiro-0001-0121-0706-2001.svg)
Values X: 0001 Y: 0121 Z: 0727, P: 2001 
![0001 Y: 0121 Z: 0727, P: 2001](./samples/spiro-0001-0121-0727-2001.svg)
Values X: 0001 Y: 0121 Z: 0746, P: 2001 
![0001 Y: 0121 Z: 0746, P: 2001](./samples/spiro-0001-0121-0746-2001.svg)
Values X: 0001 Y: 0121 Z: 0751, P: 2001 
![0001 Y: 0121 Z: 0751, P: 2001](./samples/spiro-0001-0121-0751-2001.svg)
Values X: 0001 Y: 0121 Z: 0756, P: 2001 
![0001 Y: 0121 Z: 0756, P: 2001](./samples/spiro-0001-0121-0756-2001.svg)
Values X: 0001 Y: 0121 Z: 0757, P: 2001 
![0001 Y: 0121 Z: 0757, P: 2001](./samples/spiro-0001-0121-0757-2001.svg)
Values X: 0001 Y: 0121 Z: 0761, P: 2001 
![0001 Y: 0121 Z: 0761, P: 2001](./samples/spiro-0001-0121-0761-2001.svg)
Values X: 0001 Y: 0121 Z: 0776, P: 2001 
![0001 Y: 0121 Z: 0776, P: 2001](./samples/spiro-0001-0121-0776-2001.svg)
Values X: 0001 Y: 0121 Z: 0787, P: 2001 
![0001 Y: 0121 Z: 0787, P: 2001](./samples/spiro-0001-0121-0787-2001.svg)
Values X: 0001 Y: 0121 Z: 0788, P: 2001 
![0001 Y: 0121 Z: 0788, P: 2001](./samples/spiro-0001-0121-0788-2001.svg)
Values X: 0001 Y: 0121 Z: 0791, P: 2001 
![0001 Y: 0121 Z: 0791, P: 2001](./samples/spiro-0001-0121-0791-2001.svg)
Values X: 0001 Y: 0121 Z: 0800, P: 2001 
![0001 Y: 0121 Z: 0800, P: 2001](./samples/spiro-0001-0121-0800-2001.svg)
Values X: 0001 Y: 0121 Z: 0816, P: 2001 
![0001 Y: 0121 Z: 0816, P: 2001](./samples/spiro-0001-0121-0816-2001.svg)
Values X: 0001 Y: 0121 Z: 0817, P: 2001 
![0001 Y: 0121 Z: 0817, P: 2001](./samples/spiro-0001-0121-0817-2001.svg)
Values X: 0001 Y: 0121 Z: 0829, P: 2001 
![0001 Y: 0121 Z: 0829, P: 2001](./samples/spiro-0001-0121-0829-2001.svg)
Values X: 0001 Y: 0121 Z: 0848, P: 2001 
![0001 Y: 0121 Z: 0848, P: 2001](./samples/spiro-0001-0121-0848-2001.svg)
Values X: 0001 Y: 0121 Z: 0849, P: 2001 
![0001 Y: 0121 Z: 0849, P: 2001](./samples/spiro-0001-0121-0849-2001.svg)
Values X: 0001 Y: 0121 Z: 0877, P: 2001 
![0001 Y: 0121 Z: 0877, P: 2001](./samples/spiro-0001-0121-0877-2001.svg)
Values X: 0001 Y: 0121 Z: 0878, P: 2001 
![0001 Y: 0121 Z: 0878, P: 2001](./samples/spiro-0001-0121-0878-2001.svg)
Values X: 0001 Y: 0121 Z: 0898, P: 2001 
![0001 Y: 0121 Z: 0898, P: 2001](./samples/spiro-0001-0121-0898-2001.svg)
Values X: 0001 Y: 0121 Z: 0908, P: 2001 
![0001 Y: 0121 Z: 0908, P: 2001](./samples/spiro-0001-0121-0908-2001.svg)
Values X: 0001 Y: 0121 Z: 0909, P: 2001 
![0001 Y: 0121 Z: 0909, P: 2001](./samples/spiro-0001-0121-0909-2001.svg)
Values X: 0001 Y: 0121 Z: 0919, P: 2001 
![0001 Y: 0121 Z: 0919, P: 2001](./samples/spiro-0001-0121-0919-2001.svg)
Values X: 0001 Y: 0121 Z: 0938, P: 2001 
![0001 Y: 0121 Z: 0938, P: 2001](./samples/spiro-0001-0121-0938-2001.svg)
Values X: 0001 Y: 0121 Z: 0939, P: 2001 
![0001 Y: 0121 Z: 0939, P: 2001](./samples/spiro-0001-0121-0939-2001.svg)
Values X: 0001 Y: 0121 Z: 0959, P: 2001 
![0001 Y: 0121 Z: 0959, P: 2001](./samples/spiro-0001-0121-0959-2001.svg)
Values X: 0001 Y: 0121 Z: 0961, P: 2001 
![0001 Y: 0121 Z: 0961, P: 2001](./samples/spiro-0001-0121-0961-2001.svg)
Values X: 0001 Y: 0121 Z: 0968, P: 2001 
![0001 Y: 0121 Z: 0968, P: 2001](./samples/spiro-0001-0121-0968-2001.svg)
Values X: 0001 Y: 0121 Z: 0969, P: 2001 
![0001 Y: 0121 Z: 0969, P: 2001](./samples/spiro-0001-0121-0969-2001.svg)
Values X: 0001 Y: 0121 Z: 0970, P: 2001 
![0001 Y: 0121 Z: 0970, P: 2001](./samples/spiro-0001-0121-0970-2001.svg)
Values X: 0001 Y: 0121 Z: 0979, P: 2001 
![0001 Y: 0121 Z: 0979, P: 2001](./samples/spiro-0001-0121-0979-2001.svg)
Values X: 0001 Y: 0121 Z: 0993, P: 2001 
![0001 Y: 0121 Z: 0993, P: 2001](./samples/spiro-0001-0121-0993-2001.svg)
Values X: 0001 Y: 0121 Z: 0995, P: 2001 
![0001 Y: 0121 Z: 0995, P: 2001](./samples/spiro-0001-0121-0995-2001.svg)
Values X: 0001 Y: 0121 Z: 0996, P: 2001 
![0001 Y: 0121 Z: 0996, P: 2001](./samples/spiro-0001-0121-0996-2001.svg)
Values X: 0001 Y: 0121 Z: 0997, P: 2001 
![0001 Y: 0121 Z: 0997, P: 2001](./samples/spiro-0001-0121-0997-2001.svg)
Values X: 0001 Y: 0121 Z: 0999, P: 2001 
![0001 Y: 0121 Z: 0999, P: 2001](./samples/spiro-0001-0121-0999-2001.svg)
