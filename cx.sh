#!/usr/bin/env csh

set code = $0:t
if ($#argv < 3) goto help
set n1 = $1; set n2 = $2; set n3 = $3
set a1 = 1.0; set a2 = 1.0; set a3 = 1.0
set s1 = 0.0; set s2 = 0.0; set s3 = 0.0
set output = $4
if ($#argv >= 5) set a1 = $5
if ($#argv >= 6) set a2 = $6
if ($#argv >= 7) set a3 = $7
if ($#argv >= 8) set s1 = $8
if ($#argv >= 9) set s2 = $9
if ($#argv >= 10) set s3 = ${10}


set aformat = `printf "%04d" $n1`

set bformat = `printf "%04d" $n2`

set cformat = `printf "%04d" $n3`

set fname = "${output}/spiro-${aformat}-${bformat}-${cformat}.svg"

#printf %s $fname

cat <<EOF | gnuplot
 set size ratio -1
 set nokey
 set noxtics
 set noytics
 set noborder
 set parametric
#
 n1p = {0,1}*2*pi*${n1}
 n2p = {0,1}*2*pi*${n2}
 n3p = {0,1}*2*pi*${n3}
 s1p = {0,1}*2*pi*${s1}
 s2p = {0,1}*2*pi*${s2}
 s3p = {0,1}*2*pi*${s3}
 z(t) = ${a1}*exp(n1p*t+s1p) \
      + ${a2}*exp(n2p*t+s2p) \
      + ${a3}*exp(n3p*t+s3p)
#
 set terminal svg size 777,777
 set output "${fname}"
#
 set samples 2001
 plot [t=0:1] real(z(t)),imag(z(t))
EOF

#open fig-spiro03.png
exit(0)

help:
cat << EOF
USE:     $code n1 n2 n3 [a1 a2 a3 [s1 s2 s3]]
PURPOSE: Plot Farris wheels on wheels on wheels
         curve for (n1,n2,n3,a1,a2,a3,s1,s2,s3).
         Default value for a1, a2, a3: 1.0.
         Default value for s1, s2, s3: 0.0.
EXAMPLE: $code 1 7 -17 1 0.5 1.0/3 0 0 0.24
EOF
