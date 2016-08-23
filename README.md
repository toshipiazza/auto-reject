# auto-reject
Clang Plugin that rejects any instance of auto for C++11, for example in the case of an
Academic setting, where the use of auto might hinder learning.

# Installation and Usage
Requires clang-3.9 and python-clang bindings. Note that when using a version other than
clang 3.9 the global variable `GLOBAL_INCLUDE_DIRECTORY` must be changed appropriately. It
is likely also that a version earler than clang 3.9 might fail to run.

`$ ./auto.py --auto --rfor test/main.cpp`
