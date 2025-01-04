#!/bin/bash

echo "
███████╗██╗██╗    ██╗   ██╗███████╗██████╗ 
██╔════╝██║██║    ██║   ██║██╔════╝██╔══██╗
███████╗██║██║    ██║   ██║█████╗  ██████╔╝
╚════██║██║██║    ╚██╗ ██╔╝██╔══╝  ██╔══██╗
███████║██║███████╗╚████╔╝ ███████╗██║  ██║
╚══════╝╚═╝╚══════╝ ╚═══╝  ╚══════╝╚═╝  ╚═╝"

mkdir ../examples/build

cd ../examples/build

cmake -DExamples=ON ../..

printf "\nBuilding examples into ../examples :)\n\n"

cmake --build .

mv ./examples/window ../

rm -rf *

cd ../

rm -rf build/
