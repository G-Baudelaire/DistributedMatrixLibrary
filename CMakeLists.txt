cmake_minimum_required(VERSION 4.0)
project(DMLib)

# Set Standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED 26)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Manager
add_executable(ParallelScalingManager manager.cpp)
target_link_libraries(ParallelScalingManager PRIVATE DMLib MPI::MPI_CXX)

# Worker
add_executable(ParallelScalingWorker worker.cpp)
target_link_libraries(ParallelScalingWorker PRIVATE DMLib MPI::MPI_CXX)

# --- Library with your production sources ---
add_library(DMLib STATIC
  source/CalculatorFactory.cpp
  source/NaiveAddition.cpp
  source/NaiveSubtraction.cpp
  source/NaiveMultiplication.cpp
  source/NaiveMatrixMultiplication.cpp
  include/MatrixMultiplication.h
  include/NaiveAddition.h
  include/NaiveSubtraction.h
  include/NaiveMatrixMultiplication.h
  include/NaiveMultiplication.h
  include/Calculator.h
  include/CalculatorFactory.h
  include/IAddition.h
  include/IMatrixMultiplication.h
  include/IMultiplication.h
  include/ISubtraction.h
  include/Matrix.hpp
  source/DistributedMultiplication.cpp
  include/DistributedMultiplication.h
)

# Expose headers to dependents (tests, apps)
target_include_directories(DMLib
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


# Enable testing at the top level; tests added via subdir
include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)  # contains Googletest and the test targets
endif ()
