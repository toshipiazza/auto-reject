# auto-reject
Clang Plugin that rejects any instance of auto for C++11, for example in the case of an
Academic setting, where the use of auto might hinder learning.

# Installation and Usage
Requires clang and python-clang bindings. Note that because libclang cannot guess the
proper include directories, it is necessary to manually apply them:

`$ ./auto.py --auto --rfor -isystem /usr/lib/clang/3.9/include/ test/main.cpp`
