#!/bin/bash

mkdir logs
make clean
rm -f logs/*
make lexer >logs/lexer_out_$(date +%F_%H-%M).txt 2>logs/lexer_error_$(date +%F_%H-%M).txt
make clean
make all >logs/all_out_$(date +%F_%H-%M).txt 2>logs/all_error_$(date +%F_%H-%M).txt
