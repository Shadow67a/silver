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

mv ./examples/iphone ../
mv ./examples/crt ../
mv ./examples/wave ../
mv ./examples/coverflow ../
mv ./examples/watch ../

rm -rf *

cd ../

rm -rf build/
