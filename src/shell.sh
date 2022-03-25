#!/bin/bash

while read var; do
	echo 'void ICH();' | sed "s/ICH/$var/g"
done
