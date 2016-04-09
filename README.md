# auto-reject
Clang Plugin that rejects any instance of auto for C++11

## How To Build
Assuming you already have llvm and clang checked out

```
cd llvm/tools/clang/tools
git clone https://github.com/toshipiazza/auto-reject
```

Edit the `CMakeLists.txt` in `llvm/tools/clang/tools` to
`add_clang_subdirectory` on auto-reject, them build the
top level project.
