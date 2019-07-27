#!/usr/bin/env dash
code=$0:t
if [ $# -lt 4 ]; then goto help; fi
n1=$1; n2=$2; n3=$3; a1=1.0; a2=1.0; a3=1.0; s1=0.0; s2=0.0; s3=0.0; output=$4; samples=$5
if [ $# -gt 6 ]; then a1=${6}; fi
if [ $# -gt 7 ]; then a2=${7}; fi
if [ $# -gt 8 ]; then a3=${8}; fi
if [ $# -gt 9 ]; then s1=${9}; fi
if [ $# -gt 10 ]; then s2=${10}; fi
if [ $# -gt 11 ]; then s3=${11}; fi
aformat=`printf "%04d" $n1`; bformat=`printf "%04d" $n2`; cformat=`printf "%04d" $n3`
fname="${output}/spiro-${aformat}-${bformat}-${cformat}-${samples}.svg"
if [ -f $fname ]
then
  printf "%s generated already!\n" $fname
  exit
fi
cat <<EOF | gnuplot
 set size ratio -1
 set nokey
 set noxtics
 set noytics
 set noborder
 set parametric
 n1p={0,1}*2*pi*${n1}
 n2p={0,1}*2*pi*${n2}
 n3p={0,1}*2*pi*${n3}
 s1p={0,1}*2*pi*${s1}
 s2p={0,1}*2*pi*${s2}
 s3p={0,1}*2*pi*${s3}
 z(t)=${a1}*exp(n1p*t+s1p) \
      + ${a2}*exp(n2p*t+s2p) \
      + ${a3}*exp(n3p*t+s3p)
 set terminal svg size 777,777
 set output "${fname}"
 set samples ${samples}
 plot [t=0:1] real(z(t)),imag(z(t))
EOF
printf "%s saved!\n" $fname
exit
help:
cat << EOF
USE:     $code n1 n2 n3 [a1 a2 a3 [s1 s2 s3]]
PURPOSE: Plot Farris wheels on wheels on wheels
         curve for (n1,n2,n3,a1,a2,a3,s1,s2,s3).
         Default value for a1, a2, a3: 1.0.
         Default value for s1, s2, s3: 0.0.
EXAMPLE: $code 1 7 -17 1 0.5 1.0/3 0 0 0.24
EOF