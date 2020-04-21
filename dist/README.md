# Pre-built Binaries

## Compiled Binaries

|Platform|Environment|Compiler|LibBoost|Static|
|:------:|:-:|:-:|:---:|:-:|
|Windows|msys2-mingw64|gcc 9.3.0|1.72.0|Yes|
|Linux|Debian 10|gcc 8.3.0|1.67.0||
|macOS|10.15.4 Catalina|clang 11.0.3|1.72.0||

## Dependencies

- `ncurses >= 6.0`
- `boost >= 1.67.0`
- `pthread` or msvc thread library

## Prepare Environment

### Windows

PDCurses is not supported because it lacks `menu` and `form` library. You need to use ncurses and build the program with mingw64.

- `CMakeLists.txt` needs to be modified to suit mingw64, or compile error would occur, because `${CURSES_INCLUDE_PATH}` contains files conflicting with libstdc++.
- You need to link extra library `ws2_32` because the program uses sockets.

```CMake
include_directories(${CMAKE_CXX_INCLUDE_PATH} /usr/include/ncursesw/ ${Boost_INCLUDE_DIRS})
target_link_libraries(tank PRIVATE menuw formw ncursesw ws2_32 ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -static)
target_compile_options(tank PRIVATE ${CMAKE_CXX_COMPILE_FLAGS})
```

You need to install `mingw-w64-x86_64-ncurses` and `mingw-w64-x86_64-boost` in msys2.

```shell
$ mkdir build && cd build
$ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../
$ make
```

### Linux

Using socket and `getaddrinfo` requires dynamic linking, so it is not possible to build an executable in a static manner. You need to install `libncurses-dev` and `libboost-dev` packages.

```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../
$ make
```

### macOS

macOS forbids static linking. You need to install `ncurses` and `boost` from brew. You cannot use `ncurses` shipped with macOS.

```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../
$ make
```