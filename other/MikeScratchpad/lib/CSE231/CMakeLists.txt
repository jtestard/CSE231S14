# CSE231Module
add_llvm_loadable_module(CSE231
  BranchBias.cpp
  CountDynamicInstructions.cpp
  CountStaticInstructions.cpp
  )

#Create Debug symbols as seen here:
#http://www.cs.swarthmore.edu/~adanner/tips/cmake.php
set(CMAKE_CXX_FLAGS "-g -Wall")
