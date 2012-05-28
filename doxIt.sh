#! /bin/bash

##list='stepper rs232 CImg'
list='stepper'
#move code versions to temporary directory
for d in $list
do
  mkdir -p ../DoxIt.tmp/$d
  mv ../$d/*.v?.?.?.* ../DoxIt.tmp/$d
done
#stepper help output (xterm-color unformated)
dir=../stepper
fb=stepper
bin=$dir/$fb
out=serial_$fb.help
$bin -h 2> $dir/$out; cat -v $dir/$out | sed 's/\^\[\[//g' | sed 's/1m//g' | sed 's/0;0;0m//g' | sed 's/0;35;59m//g' | sed 's/0;32;59m//g' | sed 's/4;31;59m//g' > $dir/$out.output;rm $dir/$out
#create documentation
cat stepper.Doxygen | sed 's/\ VERSION/\ '`cat VERSION`'/g' > stepper.Doxygen.version
doxygen stepper.Doxygen.version
rm stepper.Doxygen.version
#put back code versions
for d in $list
do
  mv ../DoxIt.tmp/$d/* ../$d/
  rmdir ../DoxIt.tmp/$d
done
rmdir ../DoxIt.tmp

