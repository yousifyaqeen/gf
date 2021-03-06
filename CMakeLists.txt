cmake_minimum_required(VERSION 3.0)

project(GF
  VERSION 0.10.0
  LANGUAGES CXX C
)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

option(GF_DEBUG "Activate debug build" ON)
option(GF_BUILD_WITH_OPENGL3 "Build with OpenGL 3.3 instead of OpenGL ES 2.0" OFF)
option(GF_BUILD_EXAMPLES "Build examples" ON)
option(GF_BUILD_TESTS "Build tests" ON)
option(GF_BUILD_DOCUMENTATION "Build documentation (needs Doxygen)" ON)
option(GF_BUILD_GAMES "Build games" OFF)
option(GF_BUILD_TOOLS "Build tools" OFF)
option(GF_SINGLE_COMPILTATION_UNIT "Build the library in a single file (unity build)" OFF)
option(GF_SHARED "Build the library as a shared library" ON)

if(UNIX)
  option(GF_DEV_ENABLE_ASAN "Enable Address Sanitizer" OFF)
endif()

set(BUILD_SHARED_LIBS ${GF_SHARED})

include(GNUInstallDirs)
set(GF_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)

if(APPLE)
  # on macos, always use OpenGL 3.3
  set(GF_BUILD_WITH_OPENGL3 ON)
  find_package(OpenGL REQUIRED)
endif()

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4 /utf-8 /std:c++14 /permissive-")
  link_directories(${Boost_LIBRARY_DIRS})
  # for more information on the last two switches:
  # - https://blogs.msdn.microsoft.com/vcblog/2016/06/07/standards-version-switches-in-the-compiler/
  # - https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS                 "-Wall -Wextra -std=c++14 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG           "-O0 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
endif()

if(UNIX)
  if(GF_DEV_ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endif()

add_subdirectory(library)

if(GF_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(GF_BUILD_TESTS)
  add_subdirectory(tests)
endif()

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/gf"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}"
  PATTERN "CMakeLists.txt" EXCLUDE
)

if (GF_BUILD_DOCUMENTATION)
  find_package(Doxygen)

  if(DOXYGEN_FOUND)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  endif(DOXYGEN_FOUND)

  add_subdirectory(docs/snippets)
endif()

if(GF_BUILD_GAMES)
  add_subdirectory(games/bank_robbery_for_dummies)
  add_subdirectory(games/gf_2048)
  add_subdirectory(games/gf_pong)
  add_subdirectory(games/huaca)
  add_subdirectory(games/islands)
  add_subdirectory(games/krokodile)
  add_subdirectory(games/lux)
endif()

if(GF_BUILD_TOOLS)
  add_subdirectory(tools/gf_info)
  add_subdirectory(tools/gf_noise)
  add_subdirectory(tools/gf_dungeons)

  install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/gf_tools"
    DESTINATION ${CMAKE_INSTALL_DATADIR}
  )
endif()
