# auto-reject
Clang Plugin that rejects any instance of auto for C++11, for example in the case of an
Academic setting, where the use of auto might hinder learning.

# Installation and Usage
Requires clang-3.9 and python-clang bindings.

`$ ./auto.py --auto --rfor test/main.cpp`
