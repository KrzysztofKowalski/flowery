#!/usr/bin/env sh
apt=`command -v apt-get | wc -l`; brew=`command -v brew|wc -l`
if [ $apt -gt 0 ]
then
  printf "Linux, great\n"
  sudo apt-get update
  sudo apt-get install parallel gnuplot
fi
if [ $brew -gt 0 ]
then
  printf "Mac, even better\n"
  brew install gnuplot parallel
fi