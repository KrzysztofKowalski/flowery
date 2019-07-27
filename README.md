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

Values 0001
0001[0284](./samples/!)
Values 0001
0001[0284](./samples/spiro-0001-0001-0284-2001.svg)
Values 0001
0001[0331](./samples/!)
Values 0001
0001[0331](./samples/spiro-0001-0001-0331-2001.svg)
Values 0001
0001[0332](./samples/!)
Values 0001
0001[0332](./samples/spiro-0001-0001-0332-2001.svg)
Values 0001
0001[0333](./samples/!)
Values 0001
0001[0333](./samples/spiro-0001-0001-0333-2001.svg)
Values 0001
0001[0363](./samples/!)
Values 0001
0001[0363](./samples/spiro-0001-0001-0363-2001.svg)
Values 0001
0001[0398](./samples/!)
Values 0001
0001[0398](./samples/spiro-0001-0001-0398-2001.svg)
Values 0001
0001[0399](./samples/!)
Values 0001
0001[0399](./samples/spiro-0001-0001-0399-2001.svg)
Values 0001
0001[0400](./samples/!)
Values 0001
0001[0400](./samples/spiro-0001-0001-0400-2001.svg)
Values 0001
0001[0443](./samples/!)
Values 0001
0001[0443](./samples/spiro-0001-0001-0443-2001.svg)
Values 0001
0001[0444](./samples/!)
Values 0001
0001[0444](./samples/spiro-0001-0001-0444-2001.svg)
Values 0001
0001[0445](./samples/!)
Values 0001
0001[0445](./samples/spiro-0001-0001-0445-2001.svg)
Values 0001
0001[0446](./samples/!)
Values 0001
0001[0446](./samples/spiro-0001-0001-0446-2001.svg)
Values 0001
0001[0447](./samples/!)
Values 0001
0001[0447](./samples/spiro-0001-0001-0447-2001.svg)
Values 0001
0001[0463](./samples/!)
Values 0001
0001[0463](./samples/spiro-0001-0001-0463-2001.svg)
Values 0001
0001[0464](./samples/!)
Values 0001
0001[0464](./samples/spiro-0001-0001-0464-2001.svg)
Values 0001
0001[0495](./samples/!)
Values 0001
0001[0495](./samples/spiro-0001-0001-0495-2001.svg)
Values 0001
0001[0496](./samples/!)
Values 0001
0001[0496](./samples/spiro-0001-0001-0496-2001.svg)
Values 0001
0001[0498](./samples/!)
Values 0001
0001[0498](./samples/spiro-0001-0001-0498-2001.svg)
Values 0001
0001[0499](./samples/!)
Values 0001
0001[0499](./samples/spiro-0001-0001-0499-2001.svg)
Values 0001
0001[0500](./samples/!)
Values 0001
0001[0500](./samples/spiro-0001-0001-0500-2001.svg)
Values 0001
0001[0501](./samples/!)
Values 0001
0001[0501](./samples/spiro-0001-0001-0501-2001.svg)
Values 0001
0001[0502](./samples/!)
Values 0001
0001[0502](./samples/spiro-0001-0001-0502-2001.svg)
Values 0001
0001[0503](./samples/!)
Values 0001
0001[0503](./samples/spiro-0001-0001-0503-2001.svg)
Values 0001
0001[0545](./samples/!)
Values 0001
0001[0545](./samples/spiro-0001-0001-0545-2001.svg)
Values 0001
0001[0547](./samples/!)
Values 0001
0001[0547](./samples/spiro-0001-0001-0547-2001.svg)
Values 0001
0001[0548](./samples/!)
Values 0001
0001[0548](./samples/spiro-0001-0001-0548-2001.svg)
Values 0001
0001[0570](./samples/!)
Values 0001
0001[0570](./samples/spiro-0001-0001-0570-2001.svg)
Values 0001
0001[0571](./samples/!)
Values 0001
0001[0571](./samples/spiro-0001-0001-0571-2001.svg)
Values 0001
0001[0572](./samples/!)
Values 0001
0001[0572](./samples/spiro-0001-0001-0572-2001.svg)
Values 0001
0001[0573](./samples/!)
Values 0001
0001[0573](./samples/spiro-0001-0001-0573-2001.svg)
Values 0001
0001[0574](./samples/!)
Values 0001
0001[0574](./samples/spiro-0001-0001-0574-2001.svg)
Values 0001
0001[0602](./samples/!)
Values 0001
0001[0602](./samples/spiro-0001-0001-0602-2001.svg)
Values 0001
0001[0661](./samples/!)
Values 0001
0001[0661](./samples/spiro-0001-0001-0661-2001.svg)
Values 0001
0001[0662](./samples/!)
Values 0001
0001[0662](./samples/spiro-0001-0001-0662-2001.svg)
Values 0001
0001[0663](./samples/!)
Values 0001
0001[0663](./samples/spiro-0001-0001-0663-2001.svg)
Values 0001
0001[0664](./samples/!)
Values 0001
0001[0664](./samples/spiro-0001-0001-0664-2001.svg)
Values 0001
0001[0665](./samples/!)
Values 0001
0001[0665](./samples/spiro-0001-0001-0665-2001.svg)
Values 0001
0001[0666](./samples/!)
Values 0001
0001[0666](./samples/spiro-0001-0001-0666-2001.svg)
Values 0001
0001[0667](./samples/!)
Values 0001
0001[0667](./samples/spiro-0001-0001-0667-2001.svg)
Values 0001
0001[0668](./samples/!)
Values 0001
0001[0668](./samples/spiro-0001-0001-0668-2001.svg)
Values 0001
0001[0669](./samples/!)
Values 0001
0001[0669](./samples/spiro-0001-0001-0669-2001.svg)
Values 0001
0001[0670](./samples/!)
Values 0001
0001[0670](./samples/spiro-0001-0001-0670-2001.svg)
Values 0001
0001[0671](./samples/!)
Values 0001
0001[0671](./samples/spiro-0001-0001-0671-2001.svg)
Values 0001
0001[0672](./samples/!)
Values 0001
0001[0672](./samples/spiro-0001-0001-0672-2001.svg)
Values 0001
0001[0716](./samples/!)
Values 0001
0001[0716](./samples/spiro-0001-0001-0716-2001.svg)
Values 0001
0001[0727](./samples/!)
Values 0001
0001[0727](./samples/spiro-0001-0001-0727-2001.svg)
Values 0001
0001[0728](./samples/!)
Values 0001
0001[0728](./samples/spiro-0001-0001-0728-2001.svg)
Values 0001
0001[0729](./samples/!)
Values 0001
0001[0729](./samples/spiro-0001-0001-0729-2001.svg)
Values 0001
0001[0730](./samples/!)
Values 0001
0001[0730](./samples/spiro-0001-0001-0730-2001.svg)
Values 0001
0001[0752](./samples/!)
Values 0001
0001[0752](./samples/spiro-0001-0001-0752-2001.svg)
Values 0001
0001[0798](./samples/!)
Values 0001
0001[0798](./samples/spiro-0001-0001-0798-2001.svg)
Values 0001
0001[0799](./samples/!)
Values 0001
0001[0799](./samples/spiro-0001-0001-0799-2001.svg)
Values 0001
0001[0800](./samples/!)
Values 0001
0001[0800](./samples/spiro-0001-0001-0800-2001.svg)
Values 0001
0001[0801](./samples/!)
Values 0001
0001[0801](./samples/spiro-0001-0001-0801-2001.svg)
Values 0001
0001[0802](./samples/!)
Values 0001
0001[0802](./samples/spiro-0001-0001-0802-2001.svg)
Values 0001
0001[0835](./samples/!)
Values 0001
0001[0835](./samples/spiro-0001-0001-0835-2001.svg)
Values 0001
0001[0855](./samples/!)
Values 0001
0001[0855](./samples/spiro-0001-0001-0855-2001.svg)
Values 0001
0001[0856](./samples/!)
Values 0001
0001[0856](./samples/spiro-0001-0001-0856-2001.svg)
Values 0001
0001[0857](./samples/!)
Values 0001
0001[0857](./samples/spiro-0001-0001-0857-2001.svg)
Values 0001
0001[0859](./samples/!)
Values 0001
0001[0859](./samples/spiro-0001-0001-0859-2001.svg)
Values 0001
0001[0860](./samples/!)
Values 0001
0001[0860](./samples/spiro-0001-0001-0860-2001.svg)
Values 0001
0001[0886](./samples/!)
Values 0001
0001[0886](./samples/spiro-0001-0001-0886-2001.svg)
Values 0001
0001[0887](./samples/!)
Values 0001
0001[0887](./samples/spiro-0001-0001-0887-2001.svg)
Values 0001
0001[0889](./samples/!)
Values 0001
0001[0889](./samples/spiro-0001-0001-0889-2001.svg)
Values 0001
0001[0891](./samples/!)
Values 0001
0001[0891](./samples/spiro-0001-0001-0891-2001.svg)
Values 0001
0001[0911](./samples/!)
Values 0001
0001[0911](./samples/spiro-0001-0001-0911-2001.svg)
Values 0001
0001[0912](./samples/!)
Values 0001
0001[0912](./samples/spiro-0001-0001-0912-2001.svg)
Values 0001
0001[0925](./samples/!)
Values 0001
0001[0925](./samples/spiro-0001-0001-0925-2001.svg)
Values 0001
0001[0935](./samples/!)
Values 0001
0001[0935](./samples/spiro-0001-0001-0935-2001.svg)
Values 0001
0001[0939](./samples/!)
Values 0001
0001[0939](./samples/spiro-0001-0001-0939-2001.svg)
Values 0001
0001[0943](./samples/!)
Values 0001
0001[0943](./samples/spiro-0001-0001-0943-2001.svg)
Values 0001
0001[0994](./samples/!)
Values 0001
0001[0994](./samples/spiro-0001-0001-0994-2001.svg)
Values 0001
0001[0995](./samples/!)
Values 0001
0001[0995](./samples/spiro-0001-0001-0995-2001.svg)
Values 0001
0001[0996](./samples/!)
Values 0001
0001[0996](./samples/spiro-0001-0001-0996-2001.svg)
Values 0001
0001[0997](./samples/!)
Values 0001
0001[0997](./samples/spiro-0001-0001-0997-2001.svg)
Values 0001
0001[0998](./samples/!)
Values 0001
0001[0998](./samples/spiro-0001-0001-0998-2001.svg)
Values 0001
0001[0999](./samples/!)
Values 0001
0001[0999](./samples/spiro-0001-0001-0999-2001.svg)
Values 0001
0001[1000](./samples/!)
Values 0001
0001[1000](./samples/spiro-0001-0001-1000-2001.svg)
Values 0001
0001[1001](./samples/!)
Values 0001
0001[1001](./samples/spiro-0001-0001-1001-2001.svg)
Values 0001
0001[1002](./samples/!)
Values 0001
0001[1002](./samples/spiro-0001-0001-1002-2001.svg)
Values 0001
0001[1003](./samples/!)
Values 0001
0001[1003](./samples/spiro-0001-0001-1003-2001.svg)
Values 0001
0001[1004](./samples/!)
Values 0001
0001[1004](./samples/spiro-0001-0001-1004-2001.svg)
Values 0001
0001[1005](./samples/!)
Values 0001
0001[1005](./samples/spiro-0001-0001-1005-2001.svg)
Values 0001
0001[1006](./samples/!)
Values 0001
0001[1006](./samples/spiro-0001-0001-1006-2001.svg)
Values 0001
0001[1007](./samples/!)
Values 0001
0001[1007](./samples/spiro-0001-0001-1007-2001.svg)
Values 0001
0001[1008](./samples/!)
Values 0001
0001[1008](./samples/spiro-0001-0001-1008-2001.svg)
Values 0001
0001[1197](./samples/!)
Values 0001
0001[1197](./samples/spiro-0001-0001-1197-2001.svg)
Values 0001
0001[1198](./samples/!)
Values 0001
0001[1198](./samples/spiro-0001-0001-1198-2001.svg)
Values 0001
0001[1199](./samples/!)
Values 0001
0001[1199](./samples/spiro-0001-0001-1199-2001.svg)
Values 0001
0001[1200](./samples/!)
Values 0001
0001[1200](./samples/spiro-0001-0001-1200-2001.svg)
Values 0001
0001[1201](./samples/!)
Values 0001
0001[1201](./samples/spiro-0001-0001-1201-2001.svg)
Values 0001
0001[1202](./samples/!)
Values 0001
0001[1202](./samples/spiro-0001-0001-1202-2001.svg)
Values 0001
0001[1203](./samples/!)
Values 0001
0001[1203](./samples/spiro-0001-0001-1203-2001.svg)
Values 0001
0001[1252](./samples/!)
Values 0001
0001[1252](./samples/spiro-0001-0001-1252-2001.svg)
Values 0001
0001[1329](./samples/!)
Values 0001
0001[1329](./samples/spiro-0001-0001-1329-2001.svg)
Values 0001
0001[1330](./samples/!)
Values 0001
0001[1330](./samples/spiro-0001-0001-1330-2001.svg)
Values 0001
0001[1331](./samples/!)
Values 0001
0001[1331](./samples/spiro-0001-0001-1331-2001.svg)
Values 0001
0001[1332](./samples/!)
Values 0001
0001[1332](./samples/spiro-0001-0001-1332-2001.svg)
Values 0001
0001[1333](./samples/!)
Values 0001
0001[1333](./samples/spiro-0001-0001-1333-2001.svg)
Values 0001
0001[1334](./samples/!)
Values 0001
0001[1334](./samples/spiro-0001-0001-1334-2001.svg)
Values 0001
0001[1335](./samples/!)
Values 0001
0001[1335](./samples/spiro-0001-0001-1335-2001.svg)
Values 0001
0001[1336](./samples/!)
Values 0001
0001[1336](./samples/spiro-0001-0001-1336-2001.svg)
Values 0001
0001[1497](./samples/!)
Values 0001
0001[1497](./samples/spiro-0001-0001-1497-2001.svg)
Values 0001
0001[1498](./samples/!)
Values 0001
0001[1498](./samples/spiro-0001-0001-1498-2001.svg)
Values 0001
0001[1499](./samples/!)
Values 0001
0001[1499](./samples/spiro-0001-0001-1499-2001.svg)
Values 0001
0001[1500](./samples/!)
Values 0001
0001[1500](./samples/spiro-0001-0001-1500-2001.svg)
Values 0001
0001[1501](./samples/!)
Values 0001
0001[1501](./samples/spiro-0001-0001-1501-2001.svg)
Values 0001
0001[1502](./samples/!)
Values 0001
0001[1502](./samples/spiro-0001-0001-1502-2001.svg)
Values 0001
0001[1503](./samples/!)
Values 0001
0001[1503](./samples/spiro-0001-0001-1503-2001.svg)
Values 0001
0001[1991](./samples/!)
Values 0001
0001[1991](./samples/spiro-0001-0001-1991-2001.svg)
Values 0001
0001[1992](./samples/!)
Values 0001
0001[1992](./samples/spiro-0001-0001-1992-2001.svg)
Values 0001
0001[1993](./samples/!)
Values 0001
0001[1993](./samples/spiro-0001-0001-1993-2001.svg)
Values 0001
0001[1994](./samples/!)
Values 0001
0001[1994](./samples/spiro-0001-0001-1994-2001.svg)
Values 0001
0001[1995](./samples/!)
Values 0001
0001[1995](./samples/spiro-0001-0001-1995-2001.svg)
Values 0001
0001[1996](./samples/!)
Values 0001
0001[1996](./samples/spiro-0001-0001-1996-2001.svg)
Values 0001
0001[1997](./samples/!)
Values 0001
0001[1997](./samples/spiro-0001-0001-1997-2001.svg)
Values 0001
0001[1998](./samples/!)
Values 0001
0001[1998](./samples/spiro-0001-0001-1998-2001.svg)
Values 0001
0001[1999](./samples/!)
Values 0001
0001[1999](./samples/spiro-0001-0001-1999-2001.svg)
Values 0001
0001[1999](./samples/!)
Values 0001
0001[1999](./samples/spiro-0001-0001-1999-9999.svg)
Values 0001
0001[2000](./samples/!)
Values 0001
0001[2000](./samples/spiro-0001-0001-2000-9999.svg)
Values 0001
0001[2001](./samples/!)
Values 0001
0001[2001](./samples/spiro-0001-0001-2001-9999.svg)
Values 0001
0001[2103](./samples/!)
Values 0001
0001[2103](./samples/spiro-0001-0001-2103-9999.svg)
Values 0001
0001[2498](./samples/!)
Values 0001
0001[2498](./samples/spiro-0001-0001-2498-9999.svg)
Values 0001
0001[2499](./samples/!)
Values 0001
0001[2499](./samples/spiro-0001-0001-2499-9999.svg)
Values 0001
0001[2500](./samples/!)
Values 0001
0001[2500](./samples/spiro-0001-0001-2500-9999.svg)
Values 0001
0001[2501](./samples/!)
Values 0001
0001[2501](./samples/spiro-0001-0001-2501-9999.svg)
Values 0001
0001[2502](./samples/!)
Values 0001
0001[2502](./samples/spiro-0001-0001-2502-9999.svg)
Values 0001
0001[2854](./samples/!)
Values 0001
0001[2854](./samples/spiro-0001-0001-2854-9999.svg)
Values 0001
0001[2856](./samples/!)
Values 0001
0001[2856](./samples/spiro-0001-0001-2856-9999.svg)
Values 0001
0001[2858](./samples/!)
Values 0001
0001[2858](./samples/spiro-0001-0001-2858-9999.svg)
Values 0001
0001[3001](./samples/!)
Values 0001
0001[3001](./samples/spiro-0001-0001-3001-9999.svg)
Values 0001
0001[3002](./samples/!)
Values 0001
0001[3002](./samples/spiro-0001-0001-3002-9999.svg)
Values 0001
0001[3329](./samples/!)
Values 0001
0001[3329](./samples/spiro-0001-0001-3329-9999.svg)
Values 0001
0001[3331](./samples/!)
Values 0001
0001[3331](./samples/spiro-0001-0001-3331-9999.svg)
Values 0001
0001[3332](./samples/!)
Values 0001
0001[3332](./samples/spiro-0001-0001-3332-9999.svg)
Values 0001
0001[3333](./samples/!)
Values 0001
0001[3333](./samples/spiro-0001-0001-3333-9999.svg)
Values 0001
0001[3334](./samples/!)
Values 0001
0001[3334](./samples/spiro-0001-0001-3334-9999.svg)
Values 0001
0001[3335](./samples/!)
Values 0001
0001[3335](./samples/spiro-0001-0001-3335-9999.svg)
Values 0001
0001[3336](./samples/!)
Values 0001
0001[3336](./samples/spiro-0001-0001-3336-9999.svg)
Values 0001
0001[3337](./samples/!)
Values 0001
0001[3337](./samples/spiro-0001-0001-3337-9999.svg)
Values 0001
0001[3637](./samples/!)
Values 0001
0001[3637](./samples/spiro-0001-0001-3637-9999.svg)
Values 0001
0001[3638](./samples/!)
Values 0001
0001[3638](./samples/spiro-0001-0001-3638-9999.svg)
Values 0001
0001[3746](./samples/!)
Values 0001
0001[3746](./samples/spiro-0001-0001-3746-9999.svg)
Values 0001
0001[3747](./samples/!)
Values 0001
0001[3747](./samples/spiro-0001-0001-3747-9999.svg)
Values 0001
0001[3748](./samples/!)
Values 0001
0001[3748](./samples/spiro-0001-0001-3748-9999.svg)
Values 0001
0001[3751](./samples/!)
Values 0001
0001[3751](./samples/spiro-0001-0001-3751-9999.svg)
Values 0001
0001[3752](./samples/!)
Values 0001
0001[3752](./samples/spiro-0001-0001-3752-9999.svg)
Values 0001
0001[3847](./samples/!)
Values 0001
0001[3847](./samples/spiro-0001-0001-3847-9999.svg)
Values 0001
0001[3994](./samples/!)
Values 0001
0001[3994](./samples/spiro-0001-0001-3994-9999.svg)
Values 0001
0001[3996](./samples/!)
Values 0001
0001[3996](./samples/spiro-0001-0001-3996-9999.svg)
Values 0001
0001[3997](./samples/!)
Values 0001
0001[3997](./samples/spiro-0001-0001-3997-9999.svg)
Values 0001
0001[3998](./samples/!)
Values 0001
0001[3998](./samples/spiro-0001-0001-3998-9999.svg)
Values 0001
0001[4001](./samples/!)
Values 0001
0001[4001](./samples/spiro-0001-0001-4001-9999.svg)
Values 0001
0001[4002](./samples/!)
Values 0001
0001[4002](./samples/spiro-0001-0001-4002-9999.svg)
Values 0001
0001[4282](./samples/!)
Values 0001
0001[4282](./samples/spiro-0001-0001-4282-9999.svg)
Values 0001
0001[4284](./samples/!)
Values 0001
0001[4284](./samples/spiro-0001-0001-4284-9999.svg)
Values 0001
0001[4287](./samples/!)
Values 0001
0001[4287](./samples/spiro-0001-0001-4287-9999.svg)
Values 0001
0001[4288](./samples/!)
Values 0001
0001[4288](./samples/spiro-0001-0001-4288-9999.svg)
Values 0001
0001[4372](./samples/!)
Values 0001
0001[4372](./samples/spiro-0001-0001-4372-9999.svg)
Values 0001
0001[4441](./samples/!)
Values 0001
0001[4441](./samples/spiro-0001-0001-4441-9999.svg)
Values 0001
0001[4442](./samples/!)
Values 0001
0001[4442](./samples/spiro-0001-0001-4442-9999.svg)
Values 0001
0001[4445](./samples/!)
Values 0001
0001[4445](./samples/spiro-0001-0001-4445-9999.svg)
Values 0001
0001[4446](./samples/!)
Values 0001
0001[4446](./samples/spiro-0001-0001-4446-9999.svg)
Values 0001
0001[4542](./samples/!)
Values 0001
0001[4542](./samples/spiro-0001-0001-4542-9999.svg)
Values 0001
0001[4546](./samples/!)
Values 0001
0001[4546](./samples/spiro-0001-0001-4546-9999.svg)
Values 0001
0001[4616](./samples/!)
Values 0001
0001[4616](./samples/spiro-0001-0001-4616-9999.svg)
Values 0001
0001[4990](./samples/!)
Values 0001
0001[4990](./samples/spiro-0001-0001-4990-9999.svg)
Values 0001
0001[4991](./samples/!)
Values 0001
0001[4991](./samples/spiro-0001-0001-4991-9999.svg)
Values 0001
0001[4992](./samples/!)
Values 0001
0001[4992](./samples/spiro-0001-0001-4992-9999.svg)
Values 0001
0001[4993](./samples/!)
Values 0001
0001[4993](./samples/spiro-0001-0001-4993-9999.svg)
Values 0001
0001[4994](./samples/!)
Values 0001
0001[4994](./samples/spiro-0001-0001-4994-9999.svg)
Values 0001
0001[4995](./samples/!)
Values 0001
0001[4995](./samples/spiro-0001-0001-4995-9999.svg)
Values 0001
0001[4996](./samples/!)
Values 0001
0001[4996](./samples/spiro-0001-0001-4996-9999.svg)
Values 0001
0001[4997](./samples/!)
Values 0001
0001[4997](./samples/spiro-0001-0001-4997-9999.svg)
Values 0001
0001[4998](./samples/!)
Values 0001
0001[4998](./samples/spiro-0001-0001-4998-9999.svg)
Values 0001
0001[4999](./samples/!)
Values 0001
0001[4999](./samples/spiro-0001-0001-4999-9999.svg)
Values 0001
0001[5000](./samples/!)
Values 0001
0001[5000](./samples/spiro-0001-0001-5000-9999.svg)
Values 0001
0001[5001](./samples/!)
Values 0001
0001[5001](./samples/spiro-0001-0001-5001-9999.svg)
Values 0001
0001[5002](./samples/!)
Values 0001
0001[5002](./samples/spiro-0001-0001-5002-9999.svg)
Values 0001
0001[5003](./samples/!)
Values 0001
0001[5003](./samples/spiro-0001-0001-5003-9999.svg)
Values 0001
0001[5004](./samples/!)
Values 0001
0001[5004](./samples/spiro-0001-0001-5004-9999.svg)
Values 0001
0001[5005](./samples/!)
Values 0001
0001[5005](./samples/spiro-0001-0001-5005-9999.svg)
Values 0001
0001[5006](./samples/!)
Values 0001
0001[5006](./samples/spiro-0001-0001-5006-9999.svg)
Values 0001
0001[5214](./samples/!)
Values 0001
0001[5214](./samples/spiro-0001-0001-5214-9999.svg)
Values 0001
0001[5239](./samples/!)
Values 0001
0001[5239](./samples/spiro-0001-0001-5239-9999.svg)
Values 0001
0001[5260](./samples/!)
Values 0001
0001[5260](./samples/spiro-0001-0001-5260-9999.svg)
Values 0001
0001[5291](./samples/!)
Values 0001
0001[5291](./samples/spiro-0001-0001-5291-9999.svg)
Values 0001
0001[5295](./samples/!)
Values 0001
0001[5295](./samples/spiro-0001-0001-5295-9999.svg)
Values 0001
0001[5334](./samples/!)
Values 0001
0001[5334](./samples/spiro-0001-0001-5334-9999.svg)
Values 0001
0001[5335](./samples/!)
Values 0001
0001[5335](./samples/spiro-0001-0001-5335-9999.svg)
Values 0001
0001[5381](./samples/!)
Values 0001
0001[5381](./samples/spiro-0001-0001-5381-9999.svg)
Values 0001
0001[5382](./samples/!)
Values 0001
0001[5382](./samples/spiro-0001-0001-5382-9999.svg)
Values 0001
0001[5385](./samples/!)
Values 0001
0001[5385](./samples/spiro-0001-0001-5385-9999.svg)
Values 0001
0001[5386](./samples/!)
Values 0001
0001[5386](./samples/spiro-0001-0001-5386-9999.svg)
Values 0001
0121[0011](./samples/!)
Values 0001
0121[0011](./samples/spiro-0001-0121-0011.svg)
Values 0001
0121[0016](./samples/!)
Values 0001
0121[0016](./samples/spiro-0001-0121-0016.svg)
Values 0001
0121[0039](./samples/!)
Values 0001
0121[0039](./samples/spiro-0001-0121-0039.svg)
Values 0001
0121[0040](./samples/!)
Values 0001
0121[0040](./samples/spiro-0001-0121-0040.svg)
Values 0001
0121[0059](./samples/!)
Values 0001
0121[0059](./samples/spiro-0001-0121-0059.svg)
Values 0001
0121[0060](./samples/!)
Values 0001
0121[0060](./samples/spiro-0001-0121-0060.svg)
Values 0001
0121[0074](./samples/!)
Values 0001
0121[0074](./samples/spiro-0001-0121-0074.svg)
Values 0001
0121[0082](./samples/!)
Values 0001
0121[0082](./samples/spiro-0001-0121-0082.svg)
Values 0001
0121[0084](./samples/!)
Values 0001
0121[0084](./samples/spiro-0001-0121-0084.svg)
Values 0001
0121[0092](./samples/!)
Values 0001
0121[0092](./samples/spiro-0001-0121-0092.svg)
Values 0001
0121[0093](./samples/!)
Values 0001
0121[0093](./samples/spiro-0001-0121-0093.svg)
Values 0001
0121[0094](./samples/!)
Values 0001
0121[0094](./samples/spiro-0001-0121-0094.svg)
Values 0001
0121[0098](./samples/!)
Values 0001
0121[0098](./samples/spiro-0001-0121-0098.svg)
Values 0001
0121[0109](./samples/!)
Values 0001
0121[0109](./samples/spiro-0001-0121-0109.svg)
Values 0001
0121[0115](./samples/!)
Values 0001
0121[0115](./samples/spiro-0001-0121-0115.svg)
Values 0001
0121[0116](./samples/!)
Values 0001
0121[0116](./samples/spiro-0001-0121-0116.svg)
Values 0001
0121[0118](./samples/!)
Values 0001
0121[0118](./samples/spiro-0001-0121-0118.svg)
Values 0001
0121[0125](./samples/!)
Values 0001
0121[0125](./samples/spiro-0001-0121-0125.svg)
Values 0001
0121[0129](./samples/!)
Values 0001
0121[0129](./samples/spiro-0001-0121-0129.svg)
Values 0001
0121[0130](./samples/!)
Values 0001
0121[0130](./samples/spiro-0001-0121-0130.svg)
Values 0001
0121[0131](./samples/!)
Values 0001
0121[0131](./samples/spiro-0001-0121-0131.svg)
Values 0001
0121[0136](./samples/!)
Values 0001
0121[0136](./samples/spiro-0001-0121-0136.svg)
Values 0001
0121[0140](./samples/!)
Values 0001
0121[0140](./samples/spiro-0001-0121-0140.svg)
Values 0001
0121[0144](./samples/!)
Values 0001
0121[0144](./samples/spiro-0001-0121-0144.svg)
Values 0001
0121[0160](./samples/!)
Values 0001
0121[0160](./samples/spiro-0001-0121-0160.svg)
Values 0001
0121[0163](./samples/!)
Values 0001
0121[0163](./samples/spiro-0001-0121-0163.svg)
Values 0001
0121[0171](./samples/!)
Values 0001
0121[0171](./samples/spiro-0001-0121-0171.svg)
Values 0001
0121[0183](./samples/!)
Values 0001
0121[0183](./samples/spiro-0001-0121-0183.svg)
Values 0001
0121[0203](./samples/!)
Values 0001
0121[0203](./samples/spiro-0001-0121-0203.svg)
Values 0001
0121[0216](./samples/!)
Values 0001
0121[0216](./samples/spiro-0001-0121-0216.svg)
Values 0001
0121[0220](./samples/!)
Values 0001
0121[0220](./samples/spiro-0001-0121-0220.svg)
Values 0001
0121[0235](./samples/!)
Values 0001
0121[0235](./samples/spiro-0001-0121-0235.svg)
Values 0001
0121[0236](./samples/!)
Values 0001
0121[0236](./samples/spiro-0001-0121-0236.svg)
Values 0001
0121[0237](./samples/!)
Values 0001
0121[0237](./samples/spiro-0001-0121-0237.svg)
Values 0001
0121[0238](./samples/!)
Values 0001
0121[0238](./samples/spiro-0001-0121-0238.svg)
Values 0001
0121[0239](./samples/!)
Values 0001
0121[0239](./samples/spiro-0001-0121-0239.svg)
Values 0001
0121[0244](./samples/!)
Values 0001
0121[0244](./samples/spiro-0001-0121-0244.svg)
Values 0001
0121[0245](./samples/!)
Values 0001
0121[0245](./samples/spiro-0001-0121-0245.svg)
Values 0001
0121[0246](./samples/!)
Values 0001
0121[0246](./samples/spiro-0001-0121-0246.svg)
Values 0001
0121[0247](./samples/!)
Values 0001
0121[0247](./samples/spiro-0001-0121-0247.svg)
Values 0001
0121[0280](./samples/!)
Values 0001
0121[0280](./samples/spiro-0001-0121-0280.svg)
Values 0001
0121[0304](./samples/!)
Values 0001
0121[0304](./samples/spiro-0001-0121-0304.svg)
Values 0001
0121[0305](./samples/!)
Values 0001
0121[0305](./samples/spiro-0001-0121-0305.svg)
Values 0001
0121[0321](./samples/!)
Values 0001
0121[0321](./samples/spiro-0001-0121-0321.svg)
Values 0001
0121[0336](./samples/!)
Values 0001
0121[0336](./samples/spiro-0001-0121-0336.svg)
Values 0001
0121[0359](./samples/!)
Values 0001
0121[0359](./samples/spiro-0001-0121-0359.svg)
Values 0001
0121[0363](./samples/!)
Values 0001
0121[0363](./samples/spiro-0001-0121-0363.svg)
Values 0001
0121[0365](./samples/!)
Values 0001
0121[0365](./samples/spiro-0001-0121-0365.svg)
Values 0001
0121[0366](./samples/!)
Values 0001
0121[0366](./samples/spiro-0001-0121-0366.svg)
Values 0001
0121[0367](./samples/!)
Values 0001
0121[0367](./samples/spiro-0001-0121-0367.svg)
Values 0001
0121[0368](./samples/!)
Values 0001
0121[0368](./samples/spiro-0001-0121-0368.svg)
Values 0001
0121[0441](./samples/!)
Values 0001
0121[0441](./samples/spiro-0001-0121-0441.svg)
Values 0001
0121[0478](./samples/!)
Values 0001
0121[0478](./samples/spiro-0001-0121-0478.svg)
Values 0001
0121[0481](./samples/!)
Values 0001
0121[0481](./samples/spiro-0001-0121-0481.svg)
Values 0001
0121[0485](./samples/!)
Values 0001
0121[0485](./samples/spiro-0001-0121-0485.svg)
Values 0001
0121[0486](./samples/!)
Values 0001
0121[0486](./samples/spiro-0001-0121-0486.svg)
Values 0001
0121[0487](./samples/!)
Values 0001
0121[0487](./samples/spiro-0001-0121-0487.svg)
Values 0001
0121[0488](./samples/!)
Values 0001
0121[0488](./samples/spiro-0001-0121-0488.svg)
Values 0001
0121[0489](./samples/!)
Values 0001
0121[0489](./samples/spiro-0001-0121-0489.svg)
Values 0001
0121[0499](./samples/!)
Values 0001
0121[0499](./samples/spiro-0001-0121-0499.svg)
Values 0001
0121[0505](./samples/!)
Values 0001
0121[0505](./samples/spiro-0001-0121-0505.svg)
Values 0001
0121[0521](./samples/!)
Values 0001
0121[0521](./samples/spiro-0001-0121-0521.svg)
Values 0001
0121[0545](./samples/!)
Values 0001
0121[0545](./samples/spiro-0001-0121-0545.svg)
Values 0001
0121[0585](./samples/!)
Values 0001
0121[0585](./samples/spiro-0001-0121-0585.svg)
Values 0001
0121[0586](./samples/!)
Values 0001
0121[0586](./samples/spiro-0001-0121-0586.svg)
Values 0001
0121[0605](./samples/!)
Values 0001
0121[0605](./samples/spiro-0001-0121-0605.svg)
Values 0001
0121[0606](./samples/!)
Values 0001
0121[0606](./samples/spiro-0001-0121-0606.svg)
Values 0001
0121[0607](./samples/!)
Values 0001
0121[0607](./samples/spiro-0001-0121-0607.svg)
Values 0001
0121[0626](./samples/!)
Values 0001
0121[0626](./samples/spiro-0001-0121-0626.svg)
Values 0001
0121[0654](./samples/!)
Values 0001
0121[0654](./samples/spiro-0001-0121-0654.svg)
Values 0001
0121[0666](./samples/!)
Values 0001
0121[0666](./samples/spiro-0001-0121-0666.svg)
Values 0001
0121[0667](./samples/!)
Values 0001
0121[0667](./samples/spiro-0001-0121-0667.svg)
Values 0001
0121[0680](./samples/!)
Values 0001
0121[0680](./samples/spiro-0001-0121-0680.svg)
Values 0001
0121[0683](./samples/!)
Values 0001
0121[0683](./samples/spiro-0001-0121-0683.svg)
Values 0001
0121[0695](./samples/!)
Values 0001
0121[0695](./samples/spiro-0001-0121-0695.svg)
Values 0001
0121[0698](./samples/!)
Values 0001
0121[0698](./samples/spiro-0001-0121-0698.svg)
Values 0001
0121[0701](./samples/!)
Values 0001
0121[0701](./samples/spiro-0001-0121-0701.svg)
Values 0001
0121[0706](./samples/!)
Values 0001
0121[0706](./samples/spiro-0001-0121-0706.svg)
Values 0001
0121[0727](./samples/!)
Values 0001
0121[0727](./samples/spiro-0001-0121-0727.svg)
Values 0001
0121[0746](./samples/!)
Values 0001
0121[0746](./samples/spiro-0001-0121-0746.svg)
Values 0001
0121[0751](./samples/!)
Values 0001
0121[0751](./samples/spiro-0001-0121-0751.svg)
Values 0001
0121[0756](./samples/!)
Values 0001
0121[0756](./samples/spiro-0001-0121-0756.svg)
Values 0001
0121[0757](./samples/!)
Values 0001
0121[0757](./samples/spiro-0001-0121-0757.svg)
Values 0001
0121[0761](./samples/!)
Values 0001
0121[0761](./samples/spiro-0001-0121-0761.svg)
Values 0001
0121[0776](./samples/!)
Values 0001
0121[0776](./samples/spiro-0001-0121-0776.svg)
Values 0001
0121[0787](./samples/!)
Values 0001
0121[0787](./samples/spiro-0001-0121-0787.svg)
Values 0001
0121[0788](./samples/!)
Values 0001
0121[0788](./samples/spiro-0001-0121-0788.svg)
Values 0001
0121[0791](./samples/!)
Values 0001
0121[0791](./samples/spiro-0001-0121-0791.svg)
Values 0001
0121[0800](./samples/!)
Values 0001
0121[0800](./samples/spiro-0001-0121-0800.svg)
Values 0001
0121[0816](./samples/!)
Values 0001
0121[0816](./samples/spiro-0001-0121-0816.svg)
Values 0001
0121[0817](./samples/!)
Values 0001
0121[0817](./samples/spiro-0001-0121-0817.svg)
Values 0001
0121[0829](./samples/!)
Values 0001
0121[0829](./samples/spiro-0001-0121-0829.svg)
Values 0001
0121[0848](./samples/!)
Values 0001
0121[0848](./samples/spiro-0001-0121-0848.svg)
Values 0001
0121[0849](./samples/!)
Values 0001
0121[0849](./samples/spiro-0001-0121-0849.svg)
Values 0001
0121[0877](./samples/!)
Values 0001
0121[0877](./samples/spiro-0001-0121-0877.svg)
Values 0001
0121[0878](./samples/!)
Values 0001
0121[0878](./samples/spiro-0001-0121-0878.svg)
Values 0001
0121[0898](./samples/!)
Values 0001
0121[0898](./samples/spiro-0001-0121-0898.svg)
Values 0001
0121[0908](./samples/!)
Values 0001
0121[0908](./samples/spiro-0001-0121-0908.svg)
Values 0001
0121[0909](./samples/!)
Values 0001
0121[0909](./samples/spiro-0001-0121-0909.svg)
Values 0001
0121[0919](./samples/!)
Values 0001
0121[0919](./samples/spiro-0001-0121-0919.svg)
Values 0001
0121[0938](./samples/!)
Values 0001
0121[0938](./samples/spiro-0001-0121-0938.svg)
Values 0001
0121[0939](./samples/!)
Values 0001
0121[0939](./samples/spiro-0001-0121-0939.svg)
Values 0001
0121[0959](./samples/!)
Values 0001
0121[0959](./samples/spiro-0001-0121-0959.svg)
Values 0001
0121[0961](./samples/!)
Values 0001
0121[0961](./samples/spiro-0001-0121-0961.svg)
Values 0001
0121[0968](./samples/!)
Values 0001
0121[0968](./samples/spiro-0001-0121-0968.svg)
Values 0001
0121[0969](./samples/!)
Values 0001
0121[0969](./samples/spiro-0001-0121-0969.svg)
Values 0001
0121[0970](./samples/!)
Values 0001
0121[0970](./samples/spiro-0001-0121-0970.svg)
Values 0001
0121[0979](./samples/!)
Values 0001
0121[0979](./samples/spiro-0001-0121-0979.svg)
Values 0001
0121[0993](./samples/!)
Values 0001
0121[0993](./samples/spiro-0001-0121-0993.svg)
Values 0001
0121[0995](./samples/!)
Values 0001
0121[0995](./samples/spiro-0001-0121-0995.svg)
Values 0001
0121[0996](./samples/!)
Values 0001
0121[0996](./samples/spiro-0001-0121-0996.svg)
Values 0001
0121[0997](./samples/!)
Values 0001
0121[0997](./samples/spiro-0001-0121-0997.svg)
Values 0001
0121[0999](./samples/!)
Values 0001
0121[0999](./samples/spiro-0001-0121-0999.svg)
