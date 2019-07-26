#!/usr/bin/env dash

a=$1
plotsize=$2
sb=$3
zb=$4
basedir=$5
aformat=`printf "%04d" $a `
increment=1
z=1
j=999

for b in `seq $sb $zb`
do
	bformat=`printf "%04d" $b`
	commanddir="out/commands/${aformat}/"
	mkdir -p $commanddir
	output="${commanddir}/${bformat}.sh"
	printf "%s\n" "#!/usr/bin/env dash" > $output
	./runner.sh $imgdir $a $b $z $a $b $j $increment $basedir $plotsize >> $output
	chmod +x $output
done