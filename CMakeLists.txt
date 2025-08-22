cmake_minimum_required(VERSION 4.0)
project(DMLib)

# Set Standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED 26)

find_package(MPI REQUIRED)

# --- Library with your production sources ---
add_library(DMLib
        STATIC
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
)

# Expose headers to dependents (tests, apps)
target_include_directories(DMLib
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${MSMPI_INC}"         # PUBLIC if headers need MPI types
)

# Link what dependents also need to link against (PUBLIC if types/macros leak via headers)
target_link_libraries(DMLib
        PUBLIC
        "${MSMPI_LIB}"
)

# --- Your runnable app, linking the library ---
add_executable(MatrixComputation
        main.cpp
)
target_link_libraries(MatrixComputation
        PRIVATE
        DMLib
)

# Enable testing at the top level; tests added via subdir
include(CTest)
if (BUILD_TESTING)
    add_subdirectory(tests)  # contains Googletest and the test targets
endif ()
