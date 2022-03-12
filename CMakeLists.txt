# CMakeList.txt : CMake project for libos, include source and define
# project specific logic here.
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
MESSAGE ("CMake version: " ${CMAKE_VERSION})

IF (CTV_BUILD_PLATFORM_32)
    MESSAGE ("using 32bits")
	SET (CTV_BUILD_PLATFORM "x86")
	SET (CTV_BUILD_PLATFORM_CMD "-m32")
ELSE ()
    MESSAGE ("using 64bits")
	SET (CTV_BUILD_PLATFORM "x64")
	SET (CTV_BUILD_PLATFORM_CMD "-m64")
ENDIF ()

# flags
SET (CMAKE_CXX_STANDARD 14)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CTV_BUILD_PLATFORM_CMD}")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CTV_BUILD_PLATFORM_CMD}")

# library
SET (OS_SRC
    ./src/os_utils.c
    ./src/os_queue.c
    ./src/os_list.c
	./src/os_log.c
    ./src/libos.c
)
INCLUDE_DIRECTORIES ("./include")
ADD_LIBRARY (os SHARED ${OS_SRC})

TARGET_LINK_LIBRARIES (os
                       pthread)

# test exe
ADD_EXECUTABLE (os_log_test "./examples/os_log_test.c")
TARGET_LINK_LIBRARIES (os_log_test
					   pthread
					   os)
					   
ADD_EXECUTABLE (os_queue_test "./examples/os_queue_test.c")
TARGET_LINK_LIBRARIES (os_queue_test
					   pthread
					   os)
					   
ADD_EXECUTABLE (os_list_test "./examples/os_list_test.c")
TARGET_LINK_LIBRARIES (os_list_test
					   pthread
					   os)

# Add tests and install targets if needed.
SET (OS_INC
     ./include/libos.h
	 ./include/os_log.h
	 ./include/os_queue.h
	 ./include/os_list.h)
INSTALL (FILES ${OS_INC} DESTINATION include)
INSTALL (TARGETS os DESTINATION lib)
INSTALL (TARGETS os_log_test DESTINATION bin)
INSTALL (TARGETS os_queue_test DESTINATION bin)
INSTALL (TARGETS os_list_test DESTINATION bin)
