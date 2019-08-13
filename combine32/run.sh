#!/bin/bash

size=$1

if [ -z "$size" ] #if size not specified
then
    size=5 #default size
fi

/home/root/powercycle.sh
./pool_notify pool_notify.out $size

