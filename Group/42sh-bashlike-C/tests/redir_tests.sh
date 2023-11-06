#!/bin/sh

#    T_REDIR_INPUT, /*  <  */

#    T_REDIR_OUTPUT, /*  > and >|  */
#    T_REDIR_APPEND_OUTPUT, /*  >>  */
#    T_REDIR_DUPLI_INPUT, /*  <&  */
#    T_REDIR_DUPLI_OUTPUT, /*  >&  */
#    T_REDIR_OPEN_FILE, /*  <>  */

binary=$1
if [ ! -f "$binary" ]; then
    echo not a valid binary
fi 

echo
echo
echo
echo

test_n=1
passed=0
failed=0

mkdir tests/redir_files

check()
{
    
    printf '\e[1;34m%-6s\e[m' "==============Test $test_n=============="
    echo
    printf '\e[1;37m%-6s\e[m' "Testing on: "
    echo $@ 
    
    filename_42="tests/redir_files/$2"
    shell_ext="sh"
    filename_dash="$filename_42$shell_ext"
    ./$binary -c "$1 $filename_42" > out_b
    dash -c "$1 $filename_dash" > out_dash
    
    if [ ! -f "$filename_dash" ] && [ -f "$filename_42" ]; then
        printf '\e[1;31m%-6s\e[m' "Failed"
        failed=$((failed+1))
    elif [ -f "$filename_dash" ] && [ ! -f "$filename_42" ]; then
        printf '\e[1;31m%-6s\e[m' "Failed"
        failed=$((failed+1))
    elif [ -f "$filename_dash" ]; then
        tmp=`diff "$filename_42" "$filename_dash"`
        tmp2=`diff out_b out_dash`
        if [ "$tmp" != "" ] || [ "$tmp2" != "" ]; then
            printf '\e[1;31m%-6s\e[m' "Failed"
            failed=$((failed+1))
        else
            printf '\e[1;32m%-6s\e[m' "Passed"
            passed=$((passed+1))
        fi
    else
        printf '\e[1;32m%-6s\e[m' "Passed"
        passed=$((passed+1))
    fi

    rm -rf out_b out_dash
    test_n=$((test_n+1))
    echo
}
###################################

file=file1
check "echo hello >" $file

file=file2
check "if true; then echo hello > " $file

file=file3
check "if true; then echo hello; fi  >" $file

file=file4
check "if true; then echo hello; fi; >" $file

file=file5
check "if true; then echo hello; fi; echo poireau >" $file

file=file6
check "if true; then echo hello > $file; fi; echo poireau >" $file

file=file7
check "if true; then echo hello; fi; echo poireau" $file

file=file8
check "if true > $file; then echo hello; fi; echo poireau >" $file

file=file9
check "if true; then echo hello; fi; echo poireau > $file ; if echo lol; then echo chocolat; fi >" $file

file=file10
check "if true; then echo hello; fi; echo poireau > $file ; for i in 1 2 3 4 5 6  7 8 9; do ! echo poire; done > $file; if echo lol; then echo chocolat; fi >" $file

file=file11
check "  var=0 \
    while for i in a b c; do \
       if [ \$var = \"000\" ]; then \
           not_cmd \
       else \
           echo ok \
       fi \
       done \
    do \
        echo \"in while\" \
       var=0\$var \
    done > \
" $file

file=file12
check "for i in a b c; do echo $i >> $file; >" $file

file=file13
file2=file14
check "if echo true 1> $file; then no_cmd 2> $file2; fi echo test >" $file

check "if echo true 1> $file; then no_cmd 2> $file2; fi test 2>" $file2

file=file15
check "for i in 12 24 42; do \
    if [ $i -ne 42 ]; then \
        echo -n $i 1> $file \
    else \
        echo ' <<' ${i}sh >> $file \
    fi \
done >" $file

file=file17
check "echo hello >|" $file

file=file18
check "if true; then echo hello >| " $file

file=file19
check "if true; then echo hello; fi  >|" $file

file=file20
check "if true; then echo hello; fi; >&" $file

file=file21
check "if true; then echo hello; fi; echo poireau <>" $file

file=file22
check "if true; then echo hello > $file; fi; echo poireau >&" $file

file=file23
check "if true; then echo hello; fi; echo poireau" $file

file=file24
check "if true > $file; then echo hello; fi; echo poireau >|" $file

file=file25
check "if true; then echo hello; fi; echo poireau >| $file ; for i in 1 2 3 4 5 6  7 8 9; do ! echo poire; done >> $file; if echo lol; then echo chocolat; fi >" $file

file=file26
check "if true; then echo hello; fi; echo poireau >& $file ; for i in 1 2 3 4 5 6  7 8 9; do ! echo poire; done >& $file; if echo lol; then echo chocolat; fi >" $file


###################################













echo

printf '\e[1;34m%-6s\e[m' "===========-|Final Results|-==========="
echo
printf '\e[1;32m%-6s\e[m' "       Passed:$passed    "
printf '\e[1;31m%-6s\e[m' "Failed:$failed"
echo
printf '\e[1;34m%-6s\e[m' "======================================="
echo
echo

rm -rf tests/redir_files
