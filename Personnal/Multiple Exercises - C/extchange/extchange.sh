#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Usage: extchange.sh 'ext1' 'ext2'" >/dev/stderr
    exit 1
fi

ls_con=`echo *`

bool=0
 
for line in $ls_con; do
    ext_l=`echo "$line" | cut -d'.' --complement -f1`
    name_l=`echo "$line" | cut -d'.' --complement -f2`
    if [ "$ext_l" = "$1" ]; then
        mv "$line" "$name_l"."$2"
        echo "mv $line $name_l.$2"
        bool=1
    fi
done
#done <<< "$ls_con"
if [ $bool -eq 0 ]; then
    exit 2
fi
exit 0
