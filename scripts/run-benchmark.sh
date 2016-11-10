#!/bin/bash

dirname="$(dirname "$0")"

cd "$dirname/.." || exit 1

echo "t_cost,m_cost,lanes,ms_i,ms_d"
stdbuf -oL ./argon2-bench2 | stdbuf -oL tail -n +2 | while read LINE; do
    PRINT_COMMA=0
    for x in $LINE; do
        if [ $PRINT_COMMA -eq 1 ]; then
            echo -n ","
        else
            PRINT_COMMA=1
        fi
        echo -n "$x"
    done
    echo
done
