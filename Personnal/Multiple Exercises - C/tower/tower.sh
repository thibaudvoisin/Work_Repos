#!/bin/sh

if [ $# -ne 2 ]; then
    exit 1
fi

nb_floor=$1
forme=$2

if [ $nb_floor -lt 0 ];then
    exit 1;
fi


echo " _________________"
if [ "$forme" = "triangle" ];then
    for i in $(seq $nb_floor)
    do
        for j in $(seq 4)
        do
            if [ $j -eq 1 ];then
                echo "|                 |"
            fi
            if [ $j -eq 2 ];then
                echo "|   /\       /\   |"
            fi
            if [ $j -eq 3 ];then
                echo "|  /__\     /__\  |"
            fi
            if [ $j -eq 4 ];then
                echo "|                 |"
            fi
        done
    done
fi

if [ "$forme" = "square" ];then
    for i in $(seq $nb_floor)
    do
        for j in $(seq 4)
        do
            if [ $j -eq 1 ];then
                echo "|  ___       ___  |"
            fi
            if [ $j -eq 2 ];then
                echo "| |_|_|     |_|_| |"
            fi
            if [ $j -eq 3 ];then
                echo "| |_|_|     |_|_| |"
            fi
            if [ $j -eq 4 ];then
                echo "|                 |"
            fi
        done
    done
fi
echo "|        _        |"
echo "|       | |       |"
echo "|       | |       |"
echo "|_______| |_______|"
exit 0

