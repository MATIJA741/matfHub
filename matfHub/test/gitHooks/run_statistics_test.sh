#!/bin/bash
set -e

test_file=matfHub/test/statistics.test.cpp
src_file=matfHub/src/statistics.cpp

echo "Building..."
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/Qt/6.6.0/gcc_64/lib/pkgconfig
#g++ -std=c++17 $(pkg-config --cflags --libs Qt6Widgets) -I$HOME/Qt/6.6.0/gcc_64/include -I$HOME/Qt/6.6.0/gcc_64/include/QtCore -I$HOME/Qt/6.6.0/gcc_64/include/QtWidgets ${test_file} ${src_file} -o test_fmanager.out -fPIC -lQt6Widgets -lQt6Core

g++ -std=c++17 $(pkg-config --cflags --libs Qt6Widgets) -I$HOME/Qt/6.6.0/gcc_64/include -I$HOME/Qt/6.6.0/gcc_64/include/QtCore -I$HOME/Qt/6.6.0/gcc_64/include/QtWidgets ${test_file} ${src_file} -o test_matrix.out -fPIC -L$HOME/Qt/6.6.0/gcc_64/lib -lQt6Widgets -lQt6Core -larmadillo
echo "Compile successful, no Errors detected"

# gitHook ce proveriti da li je uspesno
echo "Running test..."

export LD_LIBRARY_PATH=$HOME/Qt/6.6.0/gcc_64/lib
./test_matrix.out

echo "Testing finished..."
