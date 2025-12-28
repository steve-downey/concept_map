# etc/clang-22-toolchain.cmake                                      -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include_guard(GLOBAL)

set(CMAKE_C_COMPILER clang-22)
set(CMAKE_CXX_COMPILER clang++-22)
set(GCOV_EXECUTABLE "llvm-cov-22 gcov" CACHE STRING "GCOV executable" FORCE)

include("${CMAKE_CURRENT_LIST_DIR}/clang-flags.cmake")
