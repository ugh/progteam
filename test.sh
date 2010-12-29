#!/bin/bash

# Test script for ICPC problems.
# Excpects the source file to be Main.class

num=1

while [[ -f test.$num.in ]]; do
	echo "Testing test.$num.in"
	java Main < test.$num.in > outFile;

	diff test.$num.out outFile > diffFile;
	if [[ $? -ne 0 ]]; then
		echo "Test number $num failed: "
		cat diffFile;
		break;
	else
		num=$((num+1));
	fi
done

echo "Done!";

rm outFile
rm diffFile
