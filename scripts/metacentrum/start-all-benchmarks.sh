#!/bin/bash

dirname="$(dirname "$0")"

cd "$dirname" || exit 1

./start-benchmark.sh pro:luna   '' '' '' '' 4:00:00
./start-benchmark.sh pro:lex    '' '' '' '' 4:00:00 backfill
./start-benchmark.sh pro:mandos '' '' '' '' 4:00:00
./start-benchmark.sh pro:zubat  '' '' '' '' 4:00:00
PBS_SERVER=wagap.cerit-sc.cz \
    ./start-benchmark.sh zapat '' '' '' '' 4h
