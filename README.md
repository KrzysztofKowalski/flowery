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

Values: Values: 0001 0001 0284

![Values: 0001 0001 0284](./samples/spiro-0001-0001-0284-2001.svg)

Values: Values: 0001 0001 0331

![Values: 0001 0001 0331](./samples/spiro-0001-0001-0331-2001.svg)

Values: Values: 0001 0001 0332

![Values: 0001 0001 0332](./samples/spiro-0001-0001-0332-2001.svg)

Values: Values: 0001 0001 0333

![Values: 0001 0001 0333](./samples/spiro-0001-0001-0333-2001.svg)

Values: Values: 0001 0001 0363

![Values: 0001 0001 0363](./samples/spiro-0001-0001-0363-2001.svg)

Values: Values: 0001 0001 0398

![Values: 0001 0001 0398](./samples/spiro-0001-0001-0398-2001.svg)

Values: Values: 0001 0001 0399

![Values: 0001 0001 0399](./samples/spiro-0001-0001-0399-2001.svg)

Values: Values: 0001 0001 0400

![Values: 0001 0001 0400](./samples/spiro-0001-0001-0400-2001.svg)

Values: Values: 0001 0001 0443

![Values: 0001 0001 0443](./samples/spiro-0001-0001-0443-2001.svg)

Values: Values: 0001 0001 0444

![Values: 0001 0001 0444](./samples/spiro-0001-0001-0444-2001.svg)

Values: Values: 0001 0001 0445

![Values: 0001 0001 0445](./samples/spiro-0001-0001-0445-2001.svg)

Values: Values: 0001 0001 0446

![Values: 0001 0001 0446](./samples/spiro-0001-0001-0446-2001.svg)

Values: Values: 0001 0001 0447

![Values: 0001 0001 0447](./samples/spiro-0001-0001-0447-2001.svg)

Values: Values: 0001 0001 0463

![Values: 0001 0001 0463](./samples/spiro-0001-0001-0463-2001.svg)

Values: Values: 0001 0001 0464

![Values: 0001 0001 0464](./samples/spiro-0001-0001-0464-2001.svg)

Values: Values: 0001 0001 0495

![Values: 0001 0001 0495](./samples/spiro-0001-0001-0495-2001.svg)

Values: Values: 0001 0001 0496

![Values: 0001 0001 0496](./samples/spiro-0001-0001-0496-2001.svg)

Values: Values: 0001 0001 0498

![Values: 0001 0001 0498](./samples/spiro-0001-0001-0498-2001.svg)

Values: Values: 0001 0001 0499

![Values: 0001 0001 0499](./samples/spiro-0001-0001-0499-2001.svg)

Values: Values: 0001 0001 0500

![Values: 0001 0001 0500](./samples/spiro-0001-0001-0500-2001.svg)

Values: Values: 0001 0001 0501

![Values: 0001 0001 0501](./samples/spiro-0001-0001-0501-2001.svg)

Values: Values: 0001 0001 0502

![Values: 0001 0001 0502](./samples/spiro-0001-0001-0502-2001.svg)

Values: Values: 0001 0001 0503

![Values: 0001 0001 0503](./samples/spiro-0001-0001-0503-2001.svg)

Values: Values: 0001 0001 0545

![Values: 0001 0001 0545](./samples/spiro-0001-0001-0545-2001.svg)

Values: Values: 0001 0001 0547

![Values: 0001 0001 0547](./samples/spiro-0001-0001-0547-2001.svg)

Values: Values: 0001 0001 0548

![Values: 0001 0001 0548](./samples/spiro-0001-0001-0548-2001.svg)

Values: Values: 0001 0001 0570

![Values: 0001 0001 0570](./samples/spiro-0001-0001-0570-2001.svg)

Values: Values: 0001 0001 0571

![Values: 0001 0001 0571](./samples/spiro-0001-0001-0571-2001.svg)

Values: Values: 0001 0001 0572

![Values: 0001 0001 0572](./samples/spiro-0001-0001-0572-2001.svg)

Values: Values: 0001 0001 0573

![Values: 0001 0001 0573](./samples/spiro-0001-0001-0573-2001.svg)

Values: Values: 0001 0001 0574

![Values: 0001 0001 0574](./samples/spiro-0001-0001-0574-2001.svg)

Values: Values: 0001 0001 0602

![Values: 0001 0001 0602](./samples/spiro-0001-0001-0602-2001.svg)

Values: Values: 0001 0001 0661

![Values: 0001 0001 0661](./samples/spiro-0001-0001-0661-2001.svg)

Values: Values: 0001 0001 0662

![Values: 0001 0001 0662](./samples/spiro-0001-0001-0662-2001.svg)

Values: Values: 0001 0001 0663

![Values: 0001 0001 0663](./samples/spiro-0001-0001-0663-2001.svg)

Values: Values: 0001 0001 0664

![Values: 0001 0001 0664](./samples/spiro-0001-0001-0664-2001.svg)

Values: Values: 0001 0001 0665

![Values: 0001 0001 0665](./samples/spiro-0001-0001-0665-2001.svg)

Values: Values: 0001 0001 0666

![Values: 0001 0001 0666](./samples/spiro-0001-0001-0666-2001.svg)

Values: Values: 0001 0001 0667

![Values: 0001 0001 0667](./samples/spiro-0001-0001-0667-2001.svg)

Values: Values: 0001 0001 0668

![Values: 0001 0001 0668](./samples/spiro-0001-0001-0668-2001.svg)

Values: Values: 0001 0001 0669

![Values: 0001 0001 0669](./samples/spiro-0001-0001-0669-2001.svg)

Values: Values: 0001 0001 0670

![Values: 0001 0001 0670](./samples/spiro-0001-0001-0670-2001.svg)

Values: Values: 0001 0001 0671

![Values: 0001 0001 0671](./samples/spiro-0001-0001-0671-2001.svg)

Values: Values: 0001 0001 0672

![Values: 0001 0001 0672](./samples/spiro-0001-0001-0672-2001.svg)

Values: Values: 0001 0001 0716

![Values: 0001 0001 0716](./samples/spiro-0001-0001-0716-2001.svg)

Values: Values: 0001 0001 0727

![Values: 0001 0001 0727](./samples/spiro-0001-0001-0727-2001.svg)

Values: Values: 0001 0001 0728

![Values: 0001 0001 0728](./samples/spiro-0001-0001-0728-2001.svg)

Values: Values: 0001 0001 0729

![Values: 0001 0001 0729](./samples/spiro-0001-0001-0729-2001.svg)

Values: Values: 0001 0001 0730

![Values: 0001 0001 0730](./samples/spiro-0001-0001-0730-2001.svg)

Values: Values: 0001 0001 0752

![Values: 0001 0001 0752](./samples/spiro-0001-0001-0752-2001.svg)

Values: Values: 0001 0001 0798

![Values: 0001 0001 0798](./samples/spiro-0001-0001-0798-2001.svg)

Values: Values: 0001 0001 0799

![Values: 0001 0001 0799](./samples/spiro-0001-0001-0799-2001.svg)

Values: Values: 0001 0001 0800

![Values: 0001 0001 0800](./samples/spiro-0001-0001-0800-2001.svg)

Values: Values: 0001 0001 0801

![Values: 0001 0001 0801](./samples/spiro-0001-0001-0801-2001.svg)

Values: Values: 0001 0001 0802

![Values: 0001 0001 0802](./samples/spiro-0001-0001-0802-2001.svg)

Values: Values: 0001 0001 0835

![Values: 0001 0001 0835](./samples/spiro-0001-0001-0835-2001.svg)

Values: Values: 0001 0001 0855

![Values: 0001 0001 0855](./samples/spiro-0001-0001-0855-2001.svg)

Values: Values: 0001 0001 0856

![Values: 0001 0001 0856](./samples/spiro-0001-0001-0856-2001.svg)

Values: Values: 0001 0001 0857

![Values: 0001 0001 0857](./samples/spiro-0001-0001-0857-2001.svg)

Values: Values: 0001 0001 0859

![Values: 0001 0001 0859](./samples/spiro-0001-0001-0859-2001.svg)

Values: Values: 0001 0001 0860

![Values: 0001 0001 0860](./samples/spiro-0001-0001-0860-2001.svg)

Values: Values: 0001 0001 0886

![Values: 0001 0001 0886](./samples/spiro-0001-0001-0886-2001.svg)

Values: Values: 0001 0001 0887

![Values: 0001 0001 0887](./samples/spiro-0001-0001-0887-2001.svg)

Values: Values: 0001 0001 0889

![Values: 0001 0001 0889](./samples/spiro-0001-0001-0889-2001.svg)

Values: Values: 0001 0001 0891

![Values: 0001 0001 0891](./samples/spiro-0001-0001-0891-2001.svg)

Values: Values: 0001 0001 0911

![Values: 0001 0001 0911](./samples/spiro-0001-0001-0911-2001.svg)

Values: Values: 0001 0001 0912

![Values: 0001 0001 0912](./samples/spiro-0001-0001-0912-2001.svg)

Values: Values: 0001 0001 0925

![Values: 0001 0001 0925](./samples/spiro-0001-0001-0925-2001.svg)

Values: Values: 0001 0001 0935

![Values: 0001 0001 0935](./samples/spiro-0001-0001-0935-2001.svg)

Values: Values: 0001 0001 0939

![Values: 0001 0001 0939](./samples/spiro-0001-0001-0939-2001.svg)

Values: Values: 0001 0001 0943

![Values: 0001 0001 0943](./samples/spiro-0001-0001-0943-2001.svg)

Values: Values: 0001 0001 0994

![Values: 0001 0001 0994](./samples/spiro-0001-0001-0994-2001.svg)

Values: Values: 0001 0001 0995

![Values: 0001 0001 0995](./samples/spiro-0001-0001-0995-2001.svg)

Values: Values: 0001 0001 0996

![Values: 0001 0001 0996](./samples/spiro-0001-0001-0996-2001.svg)

Values: Values: 0001 0001 0997

![Values: 0001 0001 0997](./samples/spiro-0001-0001-0997-2001.svg)

Values: Values: 0001 0001 0998

![Values: 0001 0001 0998](./samples/spiro-0001-0001-0998-2001.svg)

Values: Values: 0001 0001 0999

![Values: 0001 0001 0999](./samples/spiro-0001-0001-0999-2001.svg)

