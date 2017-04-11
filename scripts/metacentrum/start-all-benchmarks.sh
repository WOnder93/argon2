#!/bin/bash

dirname="$(dirname "$0")"

cd "$dirname" || exit 1

./start-benchmark.sh pro:luna   '' '' '' '' bench 4:00:00
./start-benchmark.sh pro:lex    '' '' '' '' bench 4:00:00 backfill
./start-benchmark.sh pro:mandos '' '' '' '' bench 4:00:00
./start-benchmark.sh pro:zubat  '' '' '' '' bench 4:00:00
PBS_SERVER=wagap.cerit-sc.cz \
    ./start-benchmark.sh zapat '' '' '' '' bench 4h
