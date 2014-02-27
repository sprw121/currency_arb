#!/bin/bash

echo Compiling main.
make
echo Getting exchange rates.
python get_exchange_rates_async.py
echo Finding arbitrage cycles.
./main
echo Verifing results
python verify_results.py
