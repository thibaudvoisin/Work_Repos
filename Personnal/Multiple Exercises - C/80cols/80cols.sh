#!/bin/sh

if [ $# -ne 1 ]; then
    exit 1
fi

if ! [ -f "$1" ]; then
    exit 1
fi

input="$1"
while IFS= read -r line
do
  if [ "${#line}" -gt 79 ]; then
      printf "%s\n" "$line"
  fi
done < "$input"

exit 0
