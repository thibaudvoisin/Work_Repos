#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Usage: mix_files file1 file2" >/dev/stderr
    exit 1
fi

if [ ! -f "$1" ]; then
    exit 1
fi

if [ ! -f "$2" ]; then
    exit 1
fi

f1_nb=$(wc -l < "$1")
f2_nb=$(wc -l < "$2")
f1_nb_c=$(wc -l < "$1")
f2_nb_c=$(wc -l < "$2")

while [ 1 -eq 1 ]
do
    if [ $f1_nb_c -eq -1 ]; then 
        if [ $f2_nb_c -eq -1 ]; then
            exit 0
        else
            tail -n+$((f2_nb-f2_nb_c + 1)) "$2" | head -1
            f2_nb_c=$((f2_nb_c - 1))    
        fi
    else
        if [ $f2_nb_c -eq -1 ]; then
            tail -n+$((f1_nb-f1_nb_c + 1)) "$1" | head -1
            f1_nb_c=$((f1_nb_c - 1))    
        else
            tail -n+$((f1_nb-f1_nb_c + 1)) "$1" | head -1
            tail -n+$((f2_nb-f2_nb_c + 1)) "$2" | head -1
            f1_nb_c=$((f1_nb_c - 1))    
            f2_nb_c=$((f2_nb_c - 1))    
            #mix one line
        fi
    fi
done
