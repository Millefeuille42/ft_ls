#!/bin/bash

# TODO Do flags o

MIN_CRUNCH_NUM=$1
MAX_CRUNCH_NUM=$2
CRUNCH_CHARSET=$3

[[ -z "$MIN_CRUNCH_NUM" ]] && MIN_CRUNCH_NUM=1
[[ -z "$MAX_CRUNCH" ]] && MAX_CRUNCH=4
[[ -z "$CRUNCH_CHARSET" ]] && CRUNCH_CHARSET="laRrtGUfgAo"

RESULT_FOLDER="./test_results"
LS_RESULT_FILE="$RESULT_FOLDER/ls_result.log"
FT_LS_RESULT_FILE="$RESULT_FOLDER/ft_ls_result.log"
DICT_FOLDER="$RESULT_FOLDER/dict.txt"

make

echo "Generating with" "$MIN_CRUNCH_NUM" "$MAX_CRUNCH_NUM" "$CRUNCH_CHARSET"

rm -rf "$RESULT_FOLDER"
mkdir -p "$RESULT_FOLDER"

crunch "$MIN_CRUNCH_NUM" "$MAX_CRUNCH_NUM" "$CRUNCH_CHARSET" | grep -v '\(.\).*\1' >| "$DICT_FOLDER"

while IFS="" read -r p || [ -n "$p" ]
do
  LS_RESULT=$(echo $(ls "-$p") | tr ' ' '\n')
  FT_LS_RESULT=$(echo $(./ft_ls "-$p") | tr ' ' '\n')

  echo "$LS_RESULT" >| "$LS_RESULT_FILE"
  echo "$FT_LS_RESULT" >| "$FT_LS_RESULT_FILE"

  DIFF_RESULT=$(diff "$LS_RESULT_FILE" "$FT_LS_RESULT_FILE")

  echo -n "ls -$p"

  if [[ -z "$DIFF_RESULT" ]]; then
    echo -e " \e[32m[OK]\e[0m"
  else
    mkdir -p "$RESULT_FOLDER/$p"
    echo "$LS_RESULT" >| "$RESULT_FOLDER/$p/ls_$p.log"
    echo "$FT_LS_RESULT" >| "$RESULT_FOLDER/$p/ft_ls_$p.log"
    diff "$LS_RESULT_FILE" "$FT_LS_RESULT_FILE" >| "$RESULT_FOLDER/$p/$p.diff"
    echo -e " \e[31m[KO]\e[0m"
  fi

  rm "$LS_RESULT_FILE" "$FT_LS_RESULT_FILE"

done < "$DICT_FOLDER"


