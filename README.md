# spadesTDD
Spades library code made (partially) with Test-Driven-Development (TDD).

## How to Build

```bash
mkdir -p build
cd build
# BUILD_TESTING: unit testing ON/OFF (default is usually ON)
cmake -S.. -B. -DBUILD_TESTING=ON
cmake --build .
# Run the tests
ctest
```

## Attributions

[JSON for Modern C++](https://github.com/nlohmann/json)
