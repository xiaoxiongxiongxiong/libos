cmake_minimum_required(VERSION 3.8)
message("CMake version: " ${CMAKE_VERSION})

if(CMAKE_HOST_WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(OS_API_EXPORT=1)
endif()

# 包含路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 链表
file (GLOB OS_LIST_SRC list/*.c)
set (OS_LIST_LIB_SRC ${OS_LIST_SRC})
add_library(libos_list SHARED ${OS_LIST_LIB_SRC})

# 队列
file (GLOB OS_QUEUE_SRC queue/*.c)
set (OS_QUEUE_LIB_SRC ${OS_QUEUE_SRC})
add_library(libos_queue SHARED ${OS_QUEUE_LIB_SRC})

# 树
file (GLOB OS_TREE_SRC tree/*.c)
set (OS_TREE_LIB_SRC ${OS_TREE_SRC})
add_library(libos_tree SHARED ${OS_TREE_LIB_SRC})

# install
INSTALL (FILES ..include/*.h DESTINATION include)

install(TARGETS libos_list
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)
install(TARGETS libos_queue
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)
install(TARGETS libos_tree
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)