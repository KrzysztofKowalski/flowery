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
/cx.sh 480 648 816 tmp/ 2001
open tmp
```

### Links

https://linuxgazette.net/133/luana.html

## Samples

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

