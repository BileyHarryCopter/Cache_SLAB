#!/bin/bash

# colors for tracking
green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

# parametrs of environment
tests_src_for_result="../../tests/"
tests_src_for_answer="../../../tests/"
tests_dir="tests/"
gen_tests_dir="gen_tests/"
based_tests_dir="based_tests/"
answer_dir="answers"
result_dir="results"
build_dir="build/"
test_version_dir="test/"

# number of basic tests
number_basic_tests=9

function Build
{
    cmake -B build
    cd build
    cmake --build .
}

function Mkdir 
{
    rm -rf $1
    mkdir $1
}

if [ $# -ne 2 ] 
then
    echo "Testing is required 2 arguments"
else
    if [ $2 -le 0 ]
    then
        echo "Number of test should be more than 1"
    else
        cache=$1

        if [ "$cache" = "lfuda" ] || [ "$cache" = "belady" ]; then

            mode=$2

            if [ "$mode" = "based" ]
            then
                # launch basic tests
                echo "Testing on based tests..."

                Mkdir "${answer_dir}_${cache}"
                Mkdir "${result_dir}_${cache}"

                # test_genering
                cd ../$cache

                # building slow_test_version
                echo "Building answers..."
                cd test
                Build
                echo -ne "\n"

                # testing slow_version for answers
                echo "Generating answers"
                for ((i = 0; i < $number_basic_tests; i++))
                do
                    ./$cache < ${tests_src_for_answer}${based_tests_dir}test_${i} > ${tests_src_for_answer}${answer_dir}_${cache}/answer_${i}
                done
                echo -ne "\n"

                cd ../../

                # building fast_version
                echo "Building fast version of $cache..."
                Build
                echo -ne "\n"

                # testing lfuda with answers
                echo "Testing..."
                for ((i = 0; i < $number_basic_tests; i++))
                do
                    ./$cache < ${tests_src_for_result}${based_tests_dir}test_${i} > ${tests_src_for_result}${result_dir}_${cache}/result_${i}

                    echo -n "Test $((i + 1)): "
                    if diff -Z ${tests_src_for_result}${answer_dir}_${cache}/answer_${i} ${tests_src_for_result}${result_dir}_${cache}/result_${i} > /dev/null
                    then 
                        echo -e "${green}passed${default}"
                    else 
                        echo -e "${red}failed${default}"
                    fi
                done

                cd ../../

                rm -rf ${tests_dir}${result_dir}_${cache}
                rm -rf ${tests_dir}${answer_dir}_${cache}

                echo "Cleaning unnecessery build directories..."
                rm -rf ${cache}/${build_dir}
                rm -rf ${cache}/${test_version_dir}/${build_dir}

            else
                Mkdir $gen_tests_dir
                echo "Generating tests..."

                python3 testgen.py $gen_tests_dir $mode
                echo -en "\n"

                Mkdir "${answer_dir}_${cache}"
                Mkdir "${result_dir}_${cache}"

                # test_genering
                cd ../$cache

                # building slow_test_version
                echo "Building answers..."
                cd test
                Build
                echo -ne "\n"

                # testing slow_version for answers
                echo "Generating answers"
                for ((i = 0; i < $mode; i++))
                do
                    ./$cache < ${tests_src_for_answer}${gen_tests_dir}test_${i} > ${tests_src_for_answer}${answer_dir}_${cache}/answer_${i}
                done
                echo -ne "\n"

                cd ../../

                # building fast_version
                echo "Building fast version of $cache..."
                Build
                echo -ne "\n"

                # testing lfuda with answers
                echo "Testing..."
                for ((i = 0; i < $mode; i++))
                do
                    ./$cache < ${tests_src_for_result}${gen_tests_dir}test_${i} > ${tests_src_for_result}${result_dir}_${cache}/result_${i}

                    echo -n "Test $((i + 1)): "
                    if diff -Z ${tests_src_for_result}${answer_dir}_${cache}/answer_${i} ${tests_src_for_result}${result_dir}_${cache}/result_${i} > /dev/null
                    then 
                        echo -e "${green}passed${default}"
                    else 
                        echo -e "${red}failed${default}"
                    fi
                done

                cd ../../

                read -p "Do you wanna check directories for testing? (y / n) " answer
                case $answer in
                y) echo "Ok, check this out in \"tests/\" ";;
                *) echo "Delete testing derictories... "
                    rm -rf ${tests_dir}${gen_tests_dir}
                    rm -rf ${tests_dir}${result_dir}_${cache}
                    rm -rf ${tests_dir}${answer_dir}_${cache};;
                esac

                echo "Cleaning unnecessery build directories..."
                rm -rf ${cache}/${build_dir}
                rm -rf ${cache}/${test_version_dir}/${build_dir}
            fi

        else
            echo "There is no cache with such name"
        fi
    fi
fi
