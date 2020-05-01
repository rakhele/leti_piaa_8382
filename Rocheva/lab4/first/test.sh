#!/bin/bash

FILES=Tests/*
for test in $FILES
do
	echo "INPUT:"
	cat $test
	echo "OUTPUT:"
	./lab4_1 < $test
	echo "========================"
done
