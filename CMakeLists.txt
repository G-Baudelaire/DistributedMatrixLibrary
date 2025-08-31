cmake_minimum_required(VERSION 4.0)
project(DMLib)

# Set Standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED 26)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Worker
add_executable(DistributedMultiplication distributed_workers/distributed_multiplication_worker.cpp)
target_link_libraries(DistributedMultiplication PRIVATE DMLib MPI::MPI_CXX)

# --- Library with your production sources ---
add_library(DMLib STATIC
  include/naive_matrix_multiplication.hpp
  include/distributed_multiplication.hpp
  include/Matrix.hpp
  include/naive_addition.hpp
  include/naive_matrix_multiplication.hpp
  include/naive_multiplication.hpp
  include/naive_subtraction.hpp
  source/linker_dummy.cpp
  include/mpi_type_templates.hpp
)

# Expose headers to dependents (tests, apps)
target_include_directories(DMLib
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Enable testing at the top level; tests added via subdir
include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)  # contains Googletest and the test target
endif ()

set_target_properties(DMLib PROPERTIES
  ENVIRONMENT "DISTRIBUTED_MULTIPLICATION_WORKER=$<TARGET_FILE:DistributedMultiplication>"
)
