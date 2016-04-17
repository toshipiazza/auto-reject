# auto-reject
Clang Plugin that rejects any instance of auto for C++11,
for example in the case of an Academic setting, where the
use of auto might hinder learning.

## How To Build
Assuming you already have llvm and clang checked out,

```
cd llvm/tools/clang/tools
git clone https://github.com/toshipiazza/auto-reject
```

Edit the `CMakeLists.txt` in `llvm/tools/clang/examples` to
`add_clang_subdirectory` on auto-reject, them build the
top level project.

## TODO
Make this project general, to try to catch and fail arbitrary
C++ features, like lambda functions or template metaprogramming.
