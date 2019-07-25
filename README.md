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

Values: Values: Klucze

![Values: Klucze](./samples/Klucze)

Values: Values: Licencyjne

![Values: Licencyjne](./samples/Licencyjne.pages)

Values: Values: 0001 0121 0011

![Values: 0001 0121 0011](./samples/spiro-0001-0121-0011.svg)

Values: Values: 0001 0121 0016

![Values: 0001 0121 0016](./samples/spiro-0001-0121-0016.svg)

Values: Values: 0001 0121 0039

![Values: 0001 0121 0039](./samples/spiro-0001-0121-0039.svg)

Values: Values: 0001 0121 0040

![Values: 0001 0121 0040](./samples/spiro-0001-0121-0040.svg)

Values: Values: 0001 0121 0059

![Values: 0001 0121 0059](./samples/spiro-0001-0121-0059.svg)

Values: Values: 0001 0121 0060

![Values: 0001 0121 0060](./samples/spiro-0001-0121-0060.svg)

Values: Values: 0001 0121 0074

![Values: 0001 0121 0074](./samples/spiro-0001-0121-0074.svg)

Values: Values: 0001 0121 0082

![Values: 0001 0121 0082](./samples/spiro-0001-0121-0082.svg)

Values: Values: 0001 0121 0084

![Values: 0001 0121 0084](./samples/spiro-0001-0121-0084.svg)

Values: Values: 0001 0121 0092

![Values: 0001 0121 0092](./samples/spiro-0001-0121-0092.svg)

Values: Values: 0001 0121 0093

![Values: 0001 0121 0093](./samples/spiro-0001-0121-0093.svg)

Values: Values: 0001 0121 0094

![Values: 0001 0121 0094](./samples/spiro-0001-0121-0094.svg)

Values: Values: 0001 0121 0098

![Values: 0001 0121 0098](./samples/spiro-0001-0121-0098.svg)

Values: Values: 0001 0121 0109

![Values: 0001 0121 0109](./samples/spiro-0001-0121-0109.svg)

Values: Values: 0001 0121 0115

![Values: 0001 0121 0115](./samples/spiro-0001-0121-0115.svg)

Values: Values: 0001 0121 0116

![Values: 0001 0121 0116](./samples/spiro-0001-0121-0116.svg)

Values: Values: 0001 0121 0118

![Values: 0001 0121 0118](./samples/spiro-0001-0121-0118.svg)

Values: Values: 0001 0121 0125

![Values: 0001 0121 0125](./samples/spiro-0001-0121-0125.svg)

Values: Values: 0001 0121 0129

![Values: 0001 0121 0129](./samples/spiro-0001-0121-0129.svg)

Values: Values: 0001 0121 0130

![Values: 0001 0121 0130](./samples/spiro-0001-0121-0130.svg)

Values: Values: 0001 0121 0131

![Values: 0001 0121 0131](./samples/spiro-0001-0121-0131.svg)

Values: Values: 0001 0121 0136

![Values: 0001 0121 0136](./samples/spiro-0001-0121-0136.svg)

Values: Values: 0001 0121 0140

![Values: 0001 0121 0140](./samples/spiro-0001-0121-0140.svg)

Values: Values: 0001 0121 0144

![Values: 0001 0121 0144](./samples/spiro-0001-0121-0144.svg)

Values: Values: 0001 0121 0160

![Values: 0001 0121 0160](./samples/spiro-0001-0121-0160.svg)

Values: Values: 0001 0121 0163

![Values: 0001 0121 0163](./samples/spiro-0001-0121-0163.svg)

Values: Values: 0001 0121 0171

![Values: 0001 0121 0171](./samples/spiro-0001-0121-0171.svg)

Values: Values: 0001 0121 0183

![Values: 0001 0121 0183](./samples/spiro-0001-0121-0183.svg)

Values: Values: 0001 0121 0203

![Values: 0001 0121 0203](./samples/spiro-0001-0121-0203.svg)

Values: Values: 0001 0121 0216

![Values: 0001 0121 0216](./samples/spiro-0001-0121-0216.svg)

Values: Values: 0001 0121 0220

![Values: 0001 0121 0220](./samples/spiro-0001-0121-0220.svg)

Values: Values: 0001 0121 0235

![Values: 0001 0121 0235](./samples/spiro-0001-0121-0235.svg)

Values: Values: 0001 0121 0236

![Values: 0001 0121 0236](./samples/spiro-0001-0121-0236.svg)

Values: Values: 0001 0121 0237

![Values: 0001 0121 0237](./samples/spiro-0001-0121-0237.svg)

Values: Values: 0001 0121 0238

![Values: 0001 0121 0238](./samples/spiro-0001-0121-0238.svg)

Values: Values: 0001 0121 0239

![Values: 0001 0121 0239](./samples/spiro-0001-0121-0239.svg)

Values: Values: 0001 0121 0244

![Values: 0001 0121 0244](./samples/spiro-0001-0121-0244.svg)

Values: Values: 0001 0121 0245

![Values: 0001 0121 0245](./samples/spiro-0001-0121-0245.svg)

Values: Values: 0001 0121 0246

![Values: 0001 0121 0246](./samples/spiro-0001-0121-0246.svg)

Values: Values: 0001 0121 0247

![Values: 0001 0121 0247](./samples/spiro-0001-0121-0247.svg)

Values: Values: 0001 0121 0280

![Values: 0001 0121 0280](./samples/spiro-0001-0121-0280.svg)

Values: Values: 0001 0121 0304

![Values: 0001 0121 0304](./samples/spiro-0001-0121-0304.svg)

Values: Values: 0001 0121 0305

![Values: 0001 0121 0305](./samples/spiro-0001-0121-0305.svg)

Values: Values: 0001 0121 0321

![Values: 0001 0121 0321](./samples/spiro-0001-0121-0321.svg)

Values: Values: 0001 0121 0336

![Values: 0001 0121 0336](./samples/spiro-0001-0121-0336.svg)

Values: Values: 0001 0121 0359

![Values: 0001 0121 0359](./samples/spiro-0001-0121-0359.svg)

Values: Values: 0001 0121 0363

![Values: 0001 0121 0363](./samples/spiro-0001-0121-0363.svg)

Values: Values: 0001 0121 0365

![Values: 0001 0121 0365](./samples/spiro-0001-0121-0365.svg)

Values: Values: 0001 0121 0366

![Values: 0001 0121 0366](./samples/spiro-0001-0121-0366.svg)

Values: Values: 0001 0121 0367

![Values: 0001 0121 0367](./samples/spiro-0001-0121-0367.svg)

Values: Values: 0001 0121 0368

![Values: 0001 0121 0368](./samples/spiro-0001-0121-0368.svg)

Values: Values: 0001 0121 0441

![Values: 0001 0121 0441](./samples/spiro-0001-0121-0441.svg)

Values: Values: 0001 0121 0478

![Values: 0001 0121 0478](./samples/spiro-0001-0121-0478.svg)

Values: Values: 0001 0121 0481

![Values: 0001 0121 0481](./samples/spiro-0001-0121-0481.svg)

Values: Values: 0001 0121 0485

![Values: 0001 0121 0485](./samples/spiro-0001-0121-0485.svg)

Values: Values: 0001 0121 0486

![Values: 0001 0121 0486](./samples/spiro-0001-0121-0486.svg)

Values: Values: 0001 0121 0487

![Values: 0001 0121 0487](./samples/spiro-0001-0121-0487.svg)

Values: Values: 0001 0121 0488

![Values: 0001 0121 0488](./samples/spiro-0001-0121-0488.svg)

Values: Values: 0001 0121 0489

![Values: 0001 0121 0489](./samples/spiro-0001-0121-0489.svg)

Values: Values: 0001 0121 0499

![Values: 0001 0121 0499](./samples/spiro-0001-0121-0499.svg)

Values: Values: 0001 0121 0505

![Values: 0001 0121 0505](./samples/spiro-0001-0121-0505.svg)

Values: Values: 0001 0121 0521

![Values: 0001 0121 0521](./samples/spiro-0001-0121-0521.svg)

Values: Values: 0001 0121 0545

![Values: 0001 0121 0545](./samples/spiro-0001-0121-0545.svg)

Values: Values: 0001 0121 0585

![Values: 0001 0121 0585](./samples/spiro-0001-0121-0585.svg)

Values: Values: 0001 0121 0586

![Values: 0001 0121 0586](./samples/spiro-0001-0121-0586.svg)

Values: Values: 0001 0121 0605

![Values: 0001 0121 0605](./samples/spiro-0001-0121-0605.svg)

Values: Values: 0001 0121 0606

![Values: 0001 0121 0606](./samples/spiro-0001-0121-0606.svg)

Values: Values: 0001 0121 0607

![Values: 0001 0121 0607](./samples/spiro-0001-0121-0607.svg)

Values: Values: 0001 0121 0626

![Values: 0001 0121 0626](./samples/spiro-0001-0121-0626.svg)

Values: Values: 0001 0121 0654

![Values: 0001 0121 0654](./samples/spiro-0001-0121-0654.svg)

Values: Values: 0001 0121 0666

![Values: 0001 0121 0666](./samples/spiro-0001-0121-0666.svg)

Values: Values: 0001 0121 0667

![Values: 0001 0121 0667](./samples/spiro-0001-0121-0667.svg)

Values: Values: 0001 0121 0680

![Values: 0001 0121 0680](./samples/spiro-0001-0121-0680.svg)

Values: Values: 0001 0121 0683

![Values: 0001 0121 0683](./samples/spiro-0001-0121-0683.svg)

Values: Values: 0001 0121 0695

![Values: 0001 0121 0695](./samples/spiro-0001-0121-0695.svg)

Values: Values: 0001 0121 0698

![Values: 0001 0121 0698](./samples/spiro-0001-0121-0698.svg)

Values: Values: 0001 0121 0701

![Values: 0001 0121 0701](./samples/spiro-0001-0121-0701.svg)

Values: Values: 0001 0121 0706

![Values: 0001 0121 0706](./samples/spiro-0001-0121-0706.svg)

Values: Values: 0001 0121 0727

![Values: 0001 0121 0727](./samples/spiro-0001-0121-0727.svg)

Values: Values: 0001 0121 0746

![Values: 0001 0121 0746](./samples/spiro-0001-0121-0746.svg)

Values: Values: 0001 0121 0751

![Values: 0001 0121 0751](./samples/spiro-0001-0121-0751.svg)

Values: Values: 0001 0121 0756

![Values: 0001 0121 0756](./samples/spiro-0001-0121-0756.svg)

Values: Values: 0001 0121 0757

![Values: 0001 0121 0757](./samples/spiro-0001-0121-0757.svg)

Values: Values: 0001 0121 0761

![Values: 0001 0121 0761](./samples/spiro-0001-0121-0761.svg)

Values: Values: 0001 0121 0776

![Values: 0001 0121 0776](./samples/spiro-0001-0121-0776.svg)

Values: Values: 0001 0121 0787

![Values: 0001 0121 0787](./samples/spiro-0001-0121-0787.svg)

Values: Values: 0001 0121 0788

![Values: 0001 0121 0788](./samples/spiro-0001-0121-0788.svg)

Values: Values: 0001 0121 0791

![Values: 0001 0121 0791](./samples/spiro-0001-0121-0791.svg)

Values: Values: 0001 0121 0800

![Values: 0001 0121 0800](./samples/spiro-0001-0121-0800.svg)

Values: Values: 0001 0121 0816

![Values: 0001 0121 0816](./samples/spiro-0001-0121-0816.svg)

Values: Values: 0001 0121 0817

![Values: 0001 0121 0817](./samples/spiro-0001-0121-0817.svg)

Values: Values: 0001 0121 0829

![Values: 0001 0121 0829](./samples/spiro-0001-0121-0829.svg)

Values: Values: 0001 0121 0848

![Values: 0001 0121 0848](./samples/spiro-0001-0121-0848.svg)

Values: Values: 0001 0121 0849

![Values: 0001 0121 0849](./samples/spiro-0001-0121-0849.svg)

Values: Values: 0001 0121 0877

![Values: 0001 0121 0877](./samples/spiro-0001-0121-0877.svg)

Values: Values: 0001 0121 0878

![Values: 0001 0121 0878](./samples/spiro-0001-0121-0878.svg)

Values: Values: 0001 0121 0898

![Values: 0001 0121 0898](./samples/spiro-0001-0121-0898.svg)

Values: Values: 0001 0121 0908

![Values: 0001 0121 0908](./samples/spiro-0001-0121-0908.svg)

Values: Values: 0001 0121 0909

![Values: 0001 0121 0909](./samples/spiro-0001-0121-0909.svg)

Values: Values: 0001 0121 0919

![Values: 0001 0121 0919](./samples/spiro-0001-0121-0919.svg)

Values: Values: 0001 0121 0938

![Values: 0001 0121 0938](./samples/spiro-0001-0121-0938.svg)

Values: Values: 0001 0121 0939

![Values: 0001 0121 0939](./samples/spiro-0001-0121-0939.svg)

Values: Values: 0001 0121 0959

![Values: 0001 0121 0959](./samples/spiro-0001-0121-0959.svg)

Values: Values: 0001 0121 0961

![Values: 0001 0121 0961](./samples/spiro-0001-0121-0961.svg)

Values: Values: 0001 0121 0968

![Values: 0001 0121 0968](./samples/spiro-0001-0121-0968.svg)

Values: Values: 0001 0121 0969

![Values: 0001 0121 0969](./samples/spiro-0001-0121-0969.svg)

Values: Values: 0001 0121 0970

![Values: 0001 0121 0970](./samples/spiro-0001-0121-0970.svg)

Values: Values: 0001 0121 0979

![Values: 0001 0121 0979](./samples/spiro-0001-0121-0979.svg)

Values: Values: 0001 0121 0993

![Values: 0001 0121 0993](./samples/spiro-0001-0121-0993.svg)

Values: Values: 0001 0121 0995

![Values: 0001 0121 0995](./samples/spiro-0001-0121-0995.svg)

Values: Values: 0001 0121 0996

![Values: 0001 0121 0996](./samples/spiro-0001-0121-0996.svg)

Values: Values: 0001 0121 0997

![Values: 0001 0121 0997](./samples/spiro-0001-0121-0997.svg)

Values: Values: 0001 0121 0999

![Values: 0001 0121 0999](./samples/spiro-0001-0121-0999.svg)

Values: Values: 0024 0120 0120

![Values: 0024 0120 0120](./samples/spiro-0024-0120-0120.svg)

Values: Values: 0024 0120 0216

![Values: 0024 0120 0216](./samples/spiro-0024-0120-0216.svg)

Values: Values: 0033 0198 0198

![Values: 0033 0198 0198](./samples/spiro-0033-0198-0198.svg)

Values: Values: 0033 0198 0363

![Values: 0033 0198 0363](./samples/spiro-0033-0198-0363.svg)

Values: Values: 0033 0198 0759

![Values: 0033 0198 0759](./samples/spiro-0033-0198-0759.svg)

Values: Values: 0033 0264 0495

![Values: 0033 0264 0495](./samples/spiro-0033-0264-0495.svg)

Values: Values: 0033 0264 1056

![Values: 0033 0264 1056](./samples/spiro-0033-0264-1056.svg)

Values: Values: 0033 0264 1089

![Values: 0033 0264 1089](./samples/spiro-0033-0264-1089.svg)

Values: Values: 0033 0330 0627

![Values: 0033 0330 0627](./samples/spiro-0033-0330-0627.svg)

Values: Values: 0033 0363 0363

![Values: 0033 0363 0363](./samples/spiro-0033-0363-0363.svg)

Values: Values: 0033 0528 0495

![Values: 0033 0528 0495](./samples/spiro-0033-0528-0495.svg)

Values: Values: 0033 0528 0528

![Values: 0033 0528 0528](./samples/spiro-0033-0528-0528.svg)

Values: Values: 0033 0561 1089

![Values: 0033 0561 1089](./samples/spiro-0033-0561-1089.svg)

Values: Values: 0033 0627 0033

![Values: 0033 0627 0033](./samples/spiro-0033-0627-0033.svg)

Values: Values: 0033 0726 0726

![Values: 0033 0726 0726](./samples/spiro-0033-0726-0726.svg)

Values: Values: 0033 0726 0825

![Values: 0033 0726 0825](./samples/spiro-0033-0726-0825.svg)

Values: Values: 0033 0792 0891

![Values: 0033 0792 0891](./samples/spiro-0033-0792-0891.svg)

Values: Values: 0033 0825 0231

![Values: 0033 0825 0231](./samples/spiro-0033-0825-0231.svg)

Values: Values: 0033 0825 0561

![Values: 0033 0825 0561](./samples/spiro-0033-0825-0561.svg)

Values: Values: 0033 0924 0132

![Values: 0033 0924 0132](./samples/spiro-0033-0924-0132.svg)

Values: Values: 0033 0924 0924

![Values: 0033 0924 0924](./samples/spiro-0033-0924-0924.svg)

Values: Values: 0033 1056 0033

![Values: 0033 1056 0033](./samples/spiro-0033-1056-0033.svg)

Values: Values: 0072 0264 0072

![Values: 0072 0264 0072](./samples/spiro-0072-0264-0072.svg)

Values: Values: 0072 0264 0264

![Values: 0072 0264 0264](./samples/spiro-0072-0264-0264.svg)

Values: Values: 0072 0312 0312

![Values: 0072 0312 0312](./samples/spiro-0072-0312-0312.svg)

Values: Values: 0072 0312 0792

![Values: 0072 0312 0792](./samples/spiro-0072-0312-0792.svg)

Values: Values: 0072 0336 0072

![Values: 0072 0336 0072](./samples/spiro-0072-0336-0072.svg)

Values: Values: 0072 0336 0336

![Values: 0072 0336 0336](./samples/spiro-0072-0336-0336.svg)

Values: Values: 0072 0336 0888

![Values: 0072 0336 0888](./samples/spiro-0072-0336-0888.svg)

Values: Values: 0072 0360 0360

![Values: 0072 0360 0360](./samples/spiro-0072-0360-0360.svg)

Values: Values: 0144 0360 0144

![Values: 0144 0360 0144](./samples/spiro-0144-0360-0144.svg)

Values: Values: 0144 0360 0576

![Values: 0144 0360 0576](./samples/spiro-0144-0360-0576.svg)

Values: Values: 0144 0360 0720

![Values: 0144 0360 0720](./samples/spiro-0144-0360-0720.svg)

Values: Values: 0144 0384 0144

![Values: 0144 0384 0144](./samples/spiro-0144-0384-0144.svg)

Values: Values: 0144 0384 0384

![Values: 0144 0384 0384](./samples/spiro-0144-0384-0384.svg)

Values: Values: 0144 0384 0744

![Values: 0144 0384 0744](./samples/spiro-0144-0384-0744.svg)

Values: Values: 0144 0408 0144

![Values: 0144 0408 0144](./samples/spiro-0144-0408-0144.svg)

Values: Values: 0144 0408 0408

![Values: 0144 0408 0408](./samples/spiro-0144-0408-0408.svg)

Values: Values: 0144 0408 0672

![Values: 0144 0408 0672](./samples/spiro-0144-0408-0672.svg)

Values: Values: 0144 0528 0120

![Values: 0144 0528 0120](./samples/spiro-0144-0528-0120.svg)

Values: Values: 0231 0297 0528

![Values: 0231 0297 0528](./samples/spiro-0231-0297-0528.svg)

Values: Values: 0231 0297 0594

![Values: 0231 0297 0594](./samples/spiro-0231-0297-0594.svg)

Values: Values: 0231 0330 0330

![Values: 0231 0330 0330](./samples/spiro-0231-0330-0330.svg)

Values: Values: 0231 0363 0231

![Values: 0231 0363 0231](./samples/spiro-0231-0363-0231.svg)

Values: Values: 0231 0363 0495

![Values: 0231 0363 0495](./samples/spiro-0231-0363-0495.svg)

Values: Values: 0231 0495 0231

![Values: 0231 0495 0231](./samples/spiro-0231-0495-0231.svg)

Values: Values: 0231 0528 0231

![Values: 0231 0528 0231](./samples/spiro-0231-0528-0231.svg)

Values: Values: 0231 0528 0528

![Values: 0231 0528 0528](./samples/spiro-0231-0528-0528.svg)

Values: Values: 0231 0561 0561

![Values: 0231 0561 0561](./samples/spiro-0231-0561-0561.svg)

Values: Values: 0231 0627 0231

![Values: 0231 0627 0231](./samples/spiro-0231-0627-0231.svg)

Values: Values: 0231 0726 0231

![Values: 0231 0726 0231](./samples/spiro-0231-0726-0231.svg)

Values: Values: 0231 0759 0495

![Values: 0231 0759 0495](./samples/spiro-0231-0759-0495.svg)

Values: Values: 0231 0759 0759

![Values: 0231 0759 0759](./samples/spiro-0231-0759-0759.svg)

Values: Values: 0231 0825 0033

![Values: 0231 0825 0033](./samples/spiro-0231-0825-0033.svg)

Values: Values: 0231 0825 0528

![Values: 0231 0825 0528](./samples/spiro-0231-0825-0528.svg)

Values: Values: 0231 0825 0627

![Values: 0231 0825 0627](./samples/spiro-0231-0825-0627.svg)

Values: Values: 0231 0858 0198

![Values: 0231 0858 0198](./samples/spiro-0231-0858-0198.svg)

Values: Values: 0231 0858 1056

![Values: 0231 0858 1056](./samples/spiro-0231-0858-1056.svg)

Values: Values: 0231 1023 0363

![Values: 0231 1023 0363](./samples/spiro-0231-1023-0363.svg)

Values: Values: 0231 1023 0429

![Values: 0231 1023 0429](./samples/spiro-0231-1023-0429.svg)

Values: Values: 0231 1023 0495

![Values: 0231 1023 0495](./samples/spiro-0231-1023-0495.svg)

Values: Values: 0231 1056 0099

![Values: 0231 1056 0099](./samples/spiro-0231-1056-0099.svg)

Values: Values: 0231 1056 0264

![Values: 0231 1056 0264](./samples/spiro-0231-1056-0264.svg)

Values: Values: 0231 1056 0792

![Values: 0231 1056 0792](./samples/spiro-0231-1056-0792.svg)

Values: Values: 0231 1056 0924

![Values: 0231 1056 0924](./samples/spiro-0231-1056-0924.svg)

Values: Values: 0240 0840 0816

![Values: 0240 0840 0816](./samples/spiro-0240-0840-0816.svg)

Values: Values: 0240 0864 0240

![Values: 0240 0864 0240](./samples/spiro-0240-0864-0240.svg)

Values: Values: 0240 0864 0552

![Values: 0240 0864 0552](./samples/spiro-0240-0864-0552.svg)

Values: Values: 0240 0888 0240

![Values: 0240 0888 0240](./samples/spiro-0240-0888-0240.svg)

Values: Values: 0240 0888 0888

![Values: 0240 0888 0888](./samples/spiro-0240-0888-0888.svg)

Values: Values: 0240 1008 0600

![Values: 0240 1008 0600](./samples/spiro-0240-1008-0600.svg)

Values: Values: 0240 1008 0624

![Values: 0240 1008 0624](./samples/spiro-0240-1008-0624.svg)

Values: Values: 0264 0024 0024

![Values: 0264 0024 0024](./samples/spiro-0264-0024-0024.svg)

Values: Values: 0264 0072 0072

![Values: 0264 0072 0072](./samples/spiro-0264-0072-0072.svg)

Values: Values: 0264 0096 0096

![Values: 0264 0096 0096](./samples/spiro-0264-0096-0096.svg)

Values: Values: 0264 0099 0759

![Values: 0264 0099 0759](./samples/spiro-0264-0099-0759.svg)

Values: Values: 0264 0099 0924

![Values: 0264 0099 0924](./samples/spiro-0264-0099-0924.svg)

Values: Values: 0264 0165 1056

![Values: 0264 0165 1056](./samples/spiro-0264-0165-1056.svg)

Values: Values: 0264 0264 0363

![Values: 0264 0264 0363](./samples/spiro-0264-0264-0363.svg)

Values: Values: 0264 0264 0561

![Values: 0264 0264 0561](./samples/spiro-0264-0264-0561.svg)

Values: Values: 0264 0264 0594

![Values: 0264 0264 0594](./samples/spiro-0264-0264-0594.svg)

Values: Values: 0264 0264 0627

![Values: 0264 0264 0627](./samples/spiro-0264-0264-0627.svg)

Values: Values: 0264 0264 0693

![Values: 0264 0264 0693](./samples/spiro-0264-0264-0693.svg)

Values: Values: 0264 0264 0726

![Values: 0264 0264 0726](./samples/spiro-0264-0264-0726.svg)

Values: Values: 0264 0336 0192

![Values: 0264 0336 0192](./samples/spiro-0264-0336-0192.svg)

Values: Values: 0480 0648 0816

![Values: 0480 0648 0816](./samples/spiro-0480-0648-0816.svg)

