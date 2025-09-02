cmake_minimum_required(VERSION 4.0)
project(DistributedMatrixLibrary CXX)

# Set Standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED 26)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6986c2b575f77135401a4e1c65a7a42f20e18fef.zip
)
FetchContent_MakeAvailable(googletest)

# =========== Workers ===========
add_executable(DistributedMultiplication
  distributed_workers/distributed_multiplication_worker.cpp
  include/mpi_type_templates.hpp
)
target_link_libraries(DistributedMultiplication PRIVATE MPI::MPI_CXX)
target_include_directories(DistributedMultiplication PRIVATE include)

add_executable(DistributedAddition
  distributed_workers/distributed_addition_worker.cpp
  include/mpi_type_templates.hpp
)
target_link_libraries(DistributedAddition PRIVATE MPI::MPI_CXX)
target_include_directories(DistributedAddition PRIVATE include)

add_executable(DistributedSubtraction
  distributed_workers/distributed_subtraction_worker.cpp
  include/mpi_type_templates.hpp
)
target_link_libraries(DistributedSubtraction PRIVATE MPI::MPI_CXX)
target_include_directories(DistributedSubtraction PRIVATE include)

# =========== Distributed Matrix Library ===========

add_library(DistributedMatrixLibrary STATIC
  include/naive_matrix_multiplication.hpp
  include/distributed_multiplication.hpp
  include/matrix.hpp
  include/naive_addition.hpp
  include/naive_matrix_multiplication.hpp
  include/naive_multiplication.hpp
  include/naive_subtraction.hpp
  source/linker_dummy.cpp
  include/mpi_type_templates.hpp
  include/distributed_addition.hpp
  include/distributed_subtraction.hpp
  include/worker_path.hpp
)

add_dependencies(DistributedMatrixLibrary
  DistributedMultiplication
  DistributedAddition
  DistributedSubtraction
)

target_link_libraries(DistributedMatrixLibrary PRIVATE MPI::MPI_CXX)
target_include_directories(DistributedMatrixLibrary PUBLIC include)

# ==========Tests==========
enable_testing()

add_executable(DistributedMatrixLibraryTests
  tests/main.cc
  tests/addition_tests.cc
  tests/multiplication_tests.cc
  tests/subtraction_tests.cc
  tests/matrix_multiplication_tests.cc
  tests/matrix_tests.cc
)

target_link_libraries(DistributedMatrixLibraryTests PRIVATE DistributedMatrixLibrary MPI::MPI_CXX GTest::gtest)

add_dependencies(DistributedMatrixLibraryTests DistributedMatrixLibrary)

include(GoogleTest)

# TODO: Understand AI generated code
# Prefix discovery and execution with mpiexec -n 1
set(_mpi_launcher "${MPIEXEC_EXECUTABLE};${MPIEXEC_NUMPROC_FLAG};1")
# e.g. if you need oversubscribe: list(APPEND _mpi_launcher --oversubscribe)
set_property(TARGET DistributedMatrixLibraryTests PROPERTY TEST_LAUNCHER "${_mpi_launcher}")

# Discover one CTest test per gtest (all run via mpiexec)
gtest_discover_tests(DistributedMatrixLibraryTests
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DISCOVERY_MODE PRE_TEST
  PROPERTIES TIMEOUT 90
)
# End of AI Generated code.

# Benchmarks
add_executable(Benchmarks benchmarks/addition_benchmark.cpp)
target_link_libraries(Benchmarks PRIVATE DistributedMatrixLibrary MPI::MPI_CXX)

add_dependencies(Benchmarks
  DistributedMatrixLibrary
)

add_custom_target(run_benchmark
  COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:Benchmarks>
  DEPENDS Benchmarks DistributedAddition DistributedMultiplication DistributedSubtraction
  WORKING_DIRECTORY $<TARGET_FILE_DIR:Benchmarks>
)

# Uniform build-tree layout
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS DistributedMatrixLibraryTests DistributedAddition DistributedMultiplication DistributedSubtraction
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # bin
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # lib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS DistributedMatrixLibrary DistributedAddition DistributedMultiplication DistributedSubtraction
  EXPORT DistributedMatrixLibraryTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)