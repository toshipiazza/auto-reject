add_llvm_loadable_module(AutoRejectPlugin AutoReject.cpp)

if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  target_link_libraries(AutoRejectPlugin ${cmake_2_8_12_PRIVATE}
    clangAST
    clangBasic
    clangDriver
    clangAnalysis
    clangFrontend
    clangRewriteFrontend
    clangStaticAnalyzerCore
    clangStaticAnalyzerFrontend
    LLVMSupport)
endif()
