# auto-reject
Clang Plugin that rejects any instance of auto for C++11,
for example in the case of an Academic setting, where the
use of auto might hinder learning.

## How To Build

### Windows

Note: must use MinGW-w64 to compile, not just MinGW.
MSVC++ doesn't work either

```
$ cd llvm/tools/clang/examples
$ git clone https://github.com/toshipiazza/auto-reject
$ echo "add_subdirectory(auto-reject) >> CMakeLists.txt"
$ mkdir ../../../build && cd ../../../build
$ cmake .. -G"MinGW Makefiles" -DBUILD_SHARED_LIBS=On # play with this...
$ cmake --build . --target AutoRejectPlugin
```

### Linux
... TODO

## How To Run

```
$ cat should-not-ignore-auto.cpp
#include <iostream>

int
main(void)
{
  auto i = 0;
  std::cout << i << std::endl;
  return 0;
}
$ bin\clang -Xclang -load -Xclang bin\AutoRejectPlugin.dll -Xclang -plugin -Xclang -auto-reject -Wall -std=c++11 -c should-not-ignore-auto.cpp
Found declaration of auto at should-not-ignore-auto.cpp 6:2
```
