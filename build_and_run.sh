#!/bin/bash
# Build and run Trading-simulator using vcpkg toolchain

# Set vcpkg toolchain path
VCPKG_TOOLCHAIN="C:/Users/Vedant/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Configure the project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN"

# Build the project
cmake --build build

# Run the executable if build succeeds
if [ -f build/Debug/trader.exe ]; then
    ./build/Debug/trader.exe
else
    echo "Build failed or trader.exe not found."
fi
