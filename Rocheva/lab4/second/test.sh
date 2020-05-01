#!/bin/bash

FILES=Tests/*
for test in $FILES
do
	echo "INPUT:"
	cat $test
	echo "OUTPUT:"
	./lab4_2 < $test
	echo "========================"
done
