# This file is part of the uSTL library, an STL implementation.
#
# Copyright (c) 2014 by Jani Uusi-Rantala <jani.uusi-rantala@live.com>
# This file is free software, distributed under the MIT License.

cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (ustl C CXX)

include (CheckIncludeFile)
include (CheckSymbolExists)
include (CheckTypeSize)

# Basic variables
set (PKG_NAME ustl)
set (PKG_BUGREPORT "Mike Sharov <msharov@users.sourceforge.net>")

# Get version string
set (PKG_VERSTR "v2.1")
if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.git)
    execute_process (
        COMMAND git describe --always
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE PKG_VERSTR)
    string (STRIP "${PKG_VERSTR}" PKG_VERSTR)
endif ()

# Parse version numbers
string (REGEX MATCH "^v([0-9]+)\\.([0-9]+)" MATCH "${PKG_VERSTR}")
set (PKG_MAJOR ${CMAKE_MATCH_1})
set (PKG_MINOR ${CMAKE_MATCH_2})
set (PKG_VERSION "0x${PKG_MAJOR}${PKG_MINOR}0")

set (USTL_DEFINE_CONST "/* #define const */")
set (USTL_DEFINE_INLINE "/* #define inline __inline */")
set (USTL_DEFINE_OFF_T "/* typedef long off_t; */")
set (USTL_DEFINE_SIZE_T "/* typedef long size_t; */")
set (USTL_WANT_ALWAYS_INLINE "#undef WANT_ALWAYS_INLINE")

# Check include files
set (HEADERS
    assert.h ctype.h errno.h fcntl.h float.h inttypes.h limits.h stdio.h locale.h
    alloca.h signal.h stdarg.h stddef.h sys/stat.h sys/types.h stdint.h
    string.h time.h unistd.h math.h stdlib.h sys/mman.h execinfo.h)

set (USTL_DEFINE_HEADERS)
foreach (h ${HEADERS})
    string (TOUPPER "HAVE_${h}" var)
    string (REPLACE "." "_" var "${var}")
    string (REPLACE "/" "_" var "${var}")

    set (${var})
    check_include_file (${h} ${var})

    list (APPEND USTL_DEFINE_HEADERS "/// Define to 1 if you have the <${h}> header file.")
    if (${var})
        list (APPEND USTL_DEFINE_HEADERS "#define ${var} 1;")
    else ()
        list (APPEND USTL_DEFINE_HEADERS "#undef ${var};")
    endif ()
endforeach ()

set (USTL_DEFINE_FUNCTIONS)
function (check_function_exists func headers var)
    check_symbol_exists (${func} ${headers} ${var})
    list (APPEND USTL_DEFINE_FUNCTIONS "// Define to 1 if you have the ${func} function.")
    if (${var})
        list (APPEND USTL_DEFINE_FUNCTIONS "#define ${var} 1;")
    else ()
        list (APPEND USTL_DEFINE_FUNCTIONS "#undef ${var};")
    endif ()

    set (USTL_DEFINE_FUNCTIONS "${USTL_DEFINE_FUNCTIONS}" PARENT_SCOPE)
endfunction ()

# Check certain funtion prototypes exist
set (CMAKE_REQUIRED_FLAGS "-std=c++11")
check_function_exists (__va_copy "stdarg.h" HAVE_VA_COPY)
check_function_exists (strrchr "string.h" HAVE_STTRCHR)
check_function_exists (strsignal "string.h" HAVE_STRSIGNAL)
check_function_exists (rintf "math.h" HAVE_RINTF)

string (REPLACE ";" "\n" USTL_DEFINE_HEADERS "${USTL_DEFINE_HEADERS}")
string (REPLACE ";" "\n" USTL_DEFINE_FUNCTIONS "${USTL_DEFINE_FUNCTIONS}")

check_type_size("char" SIZE_OF_CHAR LANGUAGE CXX)
check_type_size("short" SIZE_OF_SHORT LANGUAGE CXX)
check_type_size("int" SIZE_OF_INT LANGUAGE CXX)
check_type_size("long" SIZE_OF_LONG LANGUAGE CXX)
check_type_size("long long" SIZE_OF_LONG_LONG LANGUAGE CXX)
check_type_size("void*" SIZE_OF_POINTER LANGUAGE CXX)
check_type_size("size_t" SIZE_OF_SIZE_T LANGUAGE CXX)
check_type_size("bool" SIZE_OF_BOOL LANGUAGE CXX)
check_type_size("int64_t" SIZE_OF_INT64_T LANGUAGE CXX)

if (NOT DEFINED USTL_HAVE_THREE_CHAR_TYPES)
    set (USTL_HAVE_THREE_CHAR_TYPES TRUE)
endif ()

if (NOT DEFINED USTL_WANT_UNROLLED_COPY)
    set (USTL_WANT_UNROLLED_COPY TRUE)
endif ()

if (NOT DEFINED USTL_WANT_MMX)
    set (USTL_WANT_MMX TRUE)
endif()

if (NOT DEFINED USTL_CPU_HAS_FPU)
    set (USTL_CPU_HAS_FPU TRUE)
endif ()


function (define_code var has)
    if (has OR ${has})
        set (USTL_${var}_CODE "#define ${var} 1" PARENT_SCOPE)
    else ()
        set (USTL_${var}_CODE "#undef ${var}" PARENT_SCOPE)
    endif ()
endfunction()

define_code (HAVE_LONG_LONG HAVE_SIZE_OF_LONG_LONG)
define_code (HAVE_INT64_T HAVE_SIZE_OF_INT64_T)
define_code (HAVE_THREE_CHAR_TYPES USTL_HAVE_THREE_CHAR_TYPES)
define_code (WANT_UNROLLED_COPY USTL_WANT_UNROLLED_COPY)
define_code (WANT_MMX USTL_WANT_MMX)
define_code (SIZE_T_IS_LONG TRUE)
define_code (CPU_HAS_FPU USTL_CPU_HAS_FPU)

# Generate config.h
set (GENERATED ${CMAKE_CURRENT_BINARY_DIR}/generated)
set (CONFIG_H ${GENERATED}/config.h)
set (CONFIG_H_IN ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in)

execute_process (COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED})
execute_process (COMMAND ${CMAKE_COMMAND} -E
    create_symlink ${CMAKE_CURRENT_SOURCE_DIR} ${GENERATED}/ustl)

configure_file (${CONFIG_H_IN} ${CONFIG_H})



# Build static and shared libraries
set (USTL_SOURCES
    bktrace.cc cmemlink.cc fstream.cc memblock.cc memlink.cc mistream.cc ofstream.cc
    sistream.cc sostream.cc ualgobase.cc ubitset.cc uexception.cc unew.cc ustdxept.cc
    ustring.cc)

set (USTL_HEADERS
    bktrace.h cmemlink.h fstream.h memblock.h memlink.h metamac.h mistream.h mostream.h
    ofstream.h simd.h sistream.h sostream.h strmsize.h traits.h typeinfo.h typelist.h
    typet.h ualgobase.h ualgo.h uarray.h ubitset.h uctralgo.h uctrstrm.h uexception.h
    ufunction.h uheap.h uiosfunc.h uios.h uiterator.h ulaalgo.h ulimits.h ulist.h
    umap.h umatrix.h umemory.h umultimap.h umultiset.h unew.h unumeric.h upair.h
    upredalgo.h uqueue.h uset.h uspecial.h ustack.h ustdxept.h ustring.h
    utf8.h utuple.h utypes.h uutility.h uvector.h ${CONFIG_H})

add_library (ustl        SHARED ${USTL_SOURCES} ${USTL_HEADERS})
add_library (ustl-static STATIC ${USTL_SOURCES} ${USTL_HEADERS})

set (USTL_WARN_OPTS
    "-Wall -Wextra -Woverloaded-virtual -Wpointer-arith -Wshadow -Wredundant-decls -Wcast-qual")

set (USTL_COMPILE_FLAGS "-std=c++11 -Os ${USTL_WARN_OPTS} ${USTL_COMPILE_FLAGS}")
set_target_properties (ustl         PROPERTIES COMPILE_FLAGS "${USTL_COMPILE_FLAGS}")
set_target_properties (ustl-static  PROPERTIES COMPILE_FLAGS "${USTL_COMPILE_FLAGS}")

target_include_directories (ustl PUBLIC ${GENERATED})
target_include_directories (ustl-static PUBLIC ${GENERATED})

install (TARGETS ustl ustl-static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
