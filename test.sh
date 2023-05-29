#!/bin/bash

# TODO Problematic flags R
# TODO Do flags oQp
# crunch 1 3 "laRrtGUfgAoQp" | grep -v '\(.\).*\1' >| dict.tx

RESULT_FOLDER="./test_results"
LS_RESULT_FILE="$RESULT_FOLDER/ls_result.log"
FT_LS_RESULT_FILE="$RESULT_FOLDER/ft_ls_result.log"

rm -rf "$RESULT_FOLDER"
mkdir -p "$RESULT_FOLDER"

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

done < "$1"


