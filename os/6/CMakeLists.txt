set(CMAKE_MINIMUM_REQUIRED_VERSION 3.1.0)
set(CMAKE_C_STANDARD_REQUIRED 99)

project(6)
add_executable(task1_creator task1_creator.c)
add_executable(task1_worker task1_worker.c)
add_executable(task2_creator task2_creator.c)
add_executable(task2_worker task2_worker.c)
add_executable(task4_creator task4_creator.c)
add_executable(task4_worker task4_worker.c)

target_link_libraries(task4_creator -lrt -pthread)
target_link_libraries(task4_worker -lrt -pthread)
