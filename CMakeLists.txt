cmake_minimum_required(VERSION 3.13)
project(tank)

file(GLOB CMAKE_CXX_SOURCES src/*.cpp src/**/*.cpp)
file(GLOB CMAKE_CXX_HEADERS src/headers/*.h src/headers/**/*.h)
set(CMAKE_CXX_INCLUDE_PATH src/headers)
set(CMAKE_CXX_STANDARD 17)

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
# see https://github.com/tony/NCURSES-Programming-HOWTO-examples/blob/master/CMakeLists.txt
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Ncurses and Boost have same ID redefined, we need to avoid this macro in ncurses.
# see https://stackoverflow.com/questions/55265851/getting-errors-when-trying-to-compile-program-using-boost-and-ncurses-libraries
# and https://stackoverflow.com/questions/13467072/c-boost-undefined-reference-to-boostsystemgeneric-category
add_definitions(-DNCURSES_NOMACROS -DBOOST_ERROR_CODE_HEADER_ONLY)

find_package(Curses REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
add_executable(tank ${CMAKE_CXX_HEADERS} ${CMAKE_CXX_SOURCES})

include_directories(${CMAKE_CXX_INCLUDE_PATH} ${CURSES_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(tank PRIVATE menu ncursesw ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(tank PRIVATE ${CMAKE_CXX_COMPILE_FLAGS})
