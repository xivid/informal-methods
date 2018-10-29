#!/usr/bin/env bash

if [[ $# < 2 ]]; then
    echo "Usage: ./lcss.sh [FILE1] [FILE2]"
    echo "To use this script, compile lis_output_sequence.cpp with g++ and put the executable \"a.out\" in the same folder."
    exit
fi
echo "Comparing $1 and $2"

# Construct correspondence
f1=`nl $1 | sort -b -k 2`  # add line number to the lines of file1 and sort starting from the second column (line contents)
echo "$1:"
echo "$f1"
f2=`nl $2 | sort -b -k 2`  # same file file2
echo "$2:"
echo "$f2"
pairs=`join -o 1.1,2.1 -1 2 -2 2 <(echo "$f1") <(echo "$f2")`
echo "Correspondences:"
echo "$pairs"

# Remove false positives
realpairs=`
echo "$pairs" | while read n1 n2
do
    diff <( tail -1 <( head -${n1} $1 ) ) <( tail -1 <( head -${n2} $2 ) ) > /dev/null
    s=$?
    if [[ ${s} == 0 ]]; then
        echo "${n1} ${n2}"
    fi
done
`
echo "Real Correspondences:"
echo "$realpairs"
pairs=${realpairs}

# Sort by first value ascending, then by second value descending
pairs=`sort -b -k1,1n -k2,2nr <(echo "$pairs")`
echo "Sorted:"
echo "$pairs"

# Find the longest upsequence in the second column
lcs=`echo "$pairs" | awk '{print $2}' | ./a.out`
echo "Line number of longest common subsequences in $2:"
echo "$lcs"

# Print the actual lines
echo "Actual lines:"
lines=`join <(nl $2) <(echo "$lcs")`  # join by the first column (lineno)
echo "$lines" | awk '{$1=""; print substr($0,2)}'
