execute_process(COMMAND ${CMAKE_CTEST_COMMAND} --test-dir build -C Debug -j4 --output-on-failure)
