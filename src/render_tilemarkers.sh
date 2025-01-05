#!/bin/bash

for i in {1..24} ;
do
    width=$(expr $i \* 64)
    height=$(expr $i \* 80)
    physicalWidth==$(inkscape -W "assets/images/tilemarkers.svg")
    dpi=$(expr $width \/ physicalWidth)
    filepath="assets/images/tilemarkers_${width}x${height}.png"
    echo "Rendering ${filepath}"
    inkscape -d $dpi "assets/images/tilemarkers.svg" -o $filepath
done
