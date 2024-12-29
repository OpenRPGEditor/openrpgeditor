#!/bin/bash

for i in {1..24} ;
do
    width=$(expr $i \* 64)
    height=$(expr $i \* 80)
    filepath="assets/images/tilemarkers_${width}x${height}.png"
    echo "Rendering ${filepath}"
    inkscape -w $width -h $height "assets/images/tilemarkers.svg" -o $filepath
done
