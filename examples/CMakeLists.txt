cmake_minimum_required(VERSION 3.8)
message("CMake version: " ${CMAKE_VERSION})

if(CMAKE_HOST_WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(../include)

# 单链表
set(SLIST_EAXMPLES_SRC "os_slist_test.c")
add_executable(os_slist_test ${SLIST_EAXMPLES_SRC})
target_link_libraries(os_slist_test libos_list)

# 双链表
set(DLIST_EAXMPLES_SRC "os_dlist_test.c")
add_executable(os_dlist_test ${DLIST_EAXMPLES_SRC})
target_link_libraries(os_dlist_test libos_list)

# 队列
set(QUEUE_EXAMPLES_SRC "os_queue_test.c")
add_executable(os_queue_test ${QUEUE_EXAMPLES_SRC})
target_link_libraries(os_queue_test libos_queue)

# 定义安装路径
install(TARGETS os_slist_test DESTINATION bin)
install(TARGETS os_dlist_test DESTINATION bin)
install(TARGETS os_queue_test DESTINATION bin)