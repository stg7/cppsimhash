#!/bin/bash

# TODO(stg7) better command line handling, maybe using python 
# and do it parallel somehow

add_file_without_check() {
    infile="$1"
    idxfile="$2"

    simhash="$(cat "$infile" | ./simhash)"
    echo "$simhash : $infile" >> $idxfile
    echo "$infile done"
}

add_file() {
    infile="$1"
    idxfile="_simidx"

    if [[ ! -f "$infile" ]]; then
        echo "$infile is not valid"
    fi

    if [[ ! -f "$idxfile" ]]; then
        touch "$idxfile"
    fi

    check=$(cat "$idxfile" | grep "$infile" | wc -l)

    if [[ "$check" == "0" ]]; then
        add_file_without_check "$infile" "$idxfile"
    fi
}

if [[ -f "$1" ]]; then
    add_file "$1"
fi

if [[ -d "$1" ]]; then
    res=$(find "$1/" -type f | sort | uniq)

    for i in $res; do
        add_file_without_check "$i" "_simidx"
    done
fi
