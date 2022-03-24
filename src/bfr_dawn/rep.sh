#!/bin/bash

string='<div style="height: 50px; width: 50px;  border-radius: 50%; background-color: ICH;"> </div> <p> ICH </p>'

while read cval; do 
	echo "$string" | sed "s/ICH/$cval/g"
done
