#!/bin/bash

TESTS="test_deliverable/test_cases/*"
COMPILER="bin/c_compiler"
#COMPILER="mips-linux-gnu-gcc"

MA="bin/tests" #mipsAssembly
rm -rf $MA/*
mkdir -p $MA

PASSED=0
CHECKED=0

have_compiler=0
# if [[ ! -f $COMPILER ]] ; then
#     echo "Cannot find compiler. Attempting to make compiler"
#     make all
#     if [[ ! -f $COMPILER ]] ; then
#         echo "Compiler build failed"
#         have_compiler=1
#     fi
#     echo "Compiler successfully built"
# fi

failedtests=()
if [[ ${have_compiler} -eq 0 ]] ; then
        for T in $TESTS
        do
                if [[ $T != *"_driver.c"* ]] ; then
                    TEMP=${T:${#TESTS}-1:-2}
                    echo "TEST $TEMP"
                    $COMPILER -S $T -o $MA/$TEMP.s
                    mips-linux-gnu-gcc -mfp32 -o $MA/$TEMP.o -c $MA/$TEMP.s
                    mips-linux-gnu-gcc -mfp32 -static -o bin/test_program $MA/$TEMP.o ${TESTS:0:-2}/$TEMP"_driver.c"
                    ((CHECKED++))
                    qemu-mips bin/test_program
                    var=$?
                    if ((var == 0)); then
                        ((PASSED++))
                        echo "PASS $var"
                    else
                        failedtests+=("$TEMP")      
                        echo "FAIL $var"
                    fi 
                fi
                echo ""
        done
        
        echo "Tests failed:"
        if [[ ${#failedtests} -ne 0 ]] ; then
            for i in $failedtests
            do
                echo "$i"
            done
        else
            echo "No tests failed"
        fi
        echo "Passed $PASSED out of $CHECKED"
        rm -f bin/test_program
fi
