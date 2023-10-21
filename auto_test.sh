clear
mkdir -p build
cmake -S. -Bbuild -DBUILD_TESTING=ON
cmake --build build/
make test -C build/

