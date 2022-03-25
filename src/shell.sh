#!/bin/bash

while read var; do
	echo 'inst_assign(0xICH, 0, 0, vac);' | sed "s/ICH/$var/g"
done
