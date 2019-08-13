#!/bin/bash

size=$1
option=$2

if [ -z "$size" ] #if size not specified
then
    size=5 #default size
fi

if [ -z "$option" ] #if size not specified
then
    option=0 #default size
fi

/home/root/powercycle.sh
./pool_notify pool_notify.out $size $option

