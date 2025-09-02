execute_process(COMMAND ${CMAKE_COMMAND} -S . -B build)
execute_process(COMMAND ${CMAKE_COMMAND} --build build --parallel)
