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
mv ./examples/test ../

rm -rf *

cd ../

rm -rf build/
