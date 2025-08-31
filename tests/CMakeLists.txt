cmake_minimum_required(VERSION 4.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6986c2b575f77135401a4e1c65a7a42f20e18fef.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# We need MPI here as well to get launcher variables (MPIEXEC_*)
find_package(MPI REQUIRED)

# ---- Test executable ----
# Provide our own GTest main that wraps MPI_Init/Finalize
add_executable(DMLibTests
  main.cc           # <-- custom main to call MPI_Init/Finalize
  addition_tests.cc
  multiplication_tests.cc
  subtraction_tests.cc
  matrix_multiplication_tests.cc
  matrix_tests.cc
)

target_link_libraries(DMLibTests
  PRIVATE
  DMLib
  MPI::MPI_CXX
  GTest::gtest               # NOTE: not gtest_main (we supply our own main)
)

# Ensure the worker exe is built before tests run
add_dependencies(DMLibTests
  DistributedMultiplication
  DistributedAddition
  DistributedSubtraction
  DMLib
)

# ---- Register the test to run under mpirun -np 1 ----
include(CTest)
include(GoogleTest)

add_test(NAME DMLibTests
  COMMAND ${MPIEXEC_EXECUTABLE}
  #  --oversubscribe
  ${MPIEXEC_NUMPROC_FLAG} 1
  $<TARGET_FILE:DMLibTests>
)

set_tests_properties(DMLibTests PROPERTIES
  ENVIRONMENT
  "DISTRIBUTED_MULTIPLICATION_WORKER=$<SHELL_PATH:$<TARGET_FILE:DistributedMultiplication>>;DISTRIBUTED_ADDITION_WORKER=$<SHELL_PATH:$<TARGET_FILE:DistributedAddition>>;DISTRIBUTED_SUBTRACTION_WORKER=$<SHELL_PATH:$<TARGET_FILE:DistributedSubtraction>>"
)
