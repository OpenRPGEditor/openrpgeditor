#!/bin/bash
function min() {
  if [ $1 -lt $2 ]; then
    echo $1
  else
    echo $2
  fi
}

function multiply_floats() {
  echo "$1 * $2" | bc -l
}

function divide_floats() {
  echo "scale=2; $1 / $2" | bc
}

baseWidth=16
baseHeight=16
baseDPI=96

for i in {8..8} ;
do
    width=$((i * baseWidth))
    height=$((i * baseWidth))
    dpi=$(divide_floats $width $baseWidth)
    dpi=$(multiply_floats $dpi $baseDPI)
    dpi=$(printf %.0f $dpi)
    inkscape -d $dpi "map.svg" -o "map_${width}x${height}.png"
    inkscape -d $dpi "event.svg" -o "event_${width}x${height}.png"
    inkscape -d $dpi "preview.svg" -o "preview_${width}x${height}.png"
    inkscape -d $dpi "preview-off.svg" -o "preview-off_${width}x${height}.png"
    inkscape --select AllLayers --actions "unhide-all" -d $dpi "layers.svg" -o "layers_all_${width}x${height}.png" --export-id-only --export-id=AllLayers
    for j in {1..4};
    do
      inkscape --select Layer${j} --actions "unhide-all" -d $dpi "layers.svg" -o "layers_${j}_${width}x${height}.png" --export-id-only --export-id=Layer${j}
    done
done
