# spadesTDD
Spades library code made with Test-Driven-Development (TDD).

## How to Build

```bash
mkdir -p build
cmake -S. -Bbuild
cd build
# BUILD_TESTING: unit testing ON/OFF (default is ON)
cmake --build . -DBUILD_TESTING=ON
# Run the tests
ctest
```