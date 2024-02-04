#!/bin/bash

# Install Vulkan
yes | rm -r ./lib/vulkan/* &
curl "https://sdk.lunarg.com/sdk/download/latest/linux/vulkan-sdk.tar.gz" --output ./lib/vulkan/vulkan-sdk.tar.gz
tar --extract -f ./lib/vulkan/vulkan-sdk.tar.gz -C ./lib/vulkan
./lib/vulkan/*/vulkansdk

# Install Eigen
yes | rm -r ./lib/eigen/* &
curl "https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz" --output ./lib/eigen/eigen.tar.gz
tar --extract -f ./lib/eigen/eigen.tar.gz -C ./lib/eigen
rm ./lib/eigen/eigen.tar.gz
mv ./lib/eigen/eigen*/* ./lib/eigen/
rm -r ./lib/eigen/eigen-*/
mkdir ./lib/eigen/build/
cmake -S ./lib/eigen/ -B ./lib/eigen/build/
pushd ./
cd ./lib/eigen/build
make install
popd

# Install Catch

# Install Rapidxml

# Install libpng