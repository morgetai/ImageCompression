#!/bin/sh -e
git clone --recursive https://github.com/Microsoft/vcpkg
cp ./vcpkg.json ./vcpkg
cd vcpkg
./bootstrap-vcpkg.sh
vcpkgroot=$(pwd)
./vcpkg install																	      
	--vcpkg-root $vcpkgroot 			 	\
	--x-manifest-root="$vcpkgroot" 		    \
	--x-install-root="$vcpkgroot/installed" \
	--clean-after-build;

cd ..
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$vcpkgroot/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release