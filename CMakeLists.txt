cmake_minimum_required(VERSION 3.5)
project(acgc)

set(CMAKE_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(GTK_INCLUDE_DIRS "C:/msys64/mingw64/include/")
set(GTK_LIBRARY_DIRS "C:/msys64/mingw64/lib")
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})
add_definitions(${GTK2_CFLAGS_OTHER})

add_executable(acgc main.c lettercheck.c trigram_table.c)
target_link_libraries(acgc ${GTK2_LIBRARIES})