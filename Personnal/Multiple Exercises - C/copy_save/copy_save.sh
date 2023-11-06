#/bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: copy_save.sh file.csv"
    exit 1
fi

if [ -f $tmp ];then
    file_cont=`cat $1`
else
    exit 1
fi

char=","
nb_file_int=0
for i in $(seq ${#file_cont})
do
    if [ "${file_cont:$i:1}" = "," ];then
        nb_file=$((nb_file+1))
    fi
done

if [ $nb_file -ne 0 ]; then
    nb_file=$((nb_file+1))
fi

if ! [ -d "save" ];then
    mkdir save
fi


for i in $(seq $nb_file_int)
do
    tmp=`echo $file_cont | cut -d',' -f$i`
    if [ $? -ne 0 ];then
        exit 1
    fi
    if [ -f $tmp ];then
        cp $tmp save/ 
    else
        echo "$tmp: no such file"
        exit 2
    fi
done

exit 0
