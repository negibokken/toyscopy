# set(CMAKE_OSX_SYSROOT macosx10.14)
# set(CMAKE_OSX_DEPLOYMENT_TARGET "10.5")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/googletest-src/googletest/include/)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
cmake_minimum_required(VERSION 3.14)
project(toyscopy)
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


# Gtk3.0
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

# gtkmm
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-3.0)
INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTKMM_CFLAGS_OTHER})

# libcurl
find_package(CURL REQUIRED)
# add_executable (curl-demo convert.cpp)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
LINK_DIRECTORIES(${CURL_LIBRARY_DIR})
ADD_DEFINITIONS(${CURL_CFLAGS_OTHER})
# target_link_libraries(curl-demo ${CURL_LIBRARIES})

set (CMAKE_CXX_STANDARD 14)
# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(main core/test.cc)
add_library(flash flash/html/html_document_parser.cc flash/html/renderer.cc flash/html/tokenizer.cc flash/html/token.cc  utils/simple_http_client.cc  utils/log_util.cc)


## Main
add_executable(main core/main.cc core/app.cc)
TARGET_LINK_LIBRARIES(main ${GTK3_LIBRARIES})
TARGET_LINK_LIBRARIES(main ${GTKMM_LIBRARIES})
TARGET_LINK_LIBRARIES(main ${CURL_LIBRARIES})
TARGET_LINK_LIBRARIES(main flash)

## Test
enable_testing()
add_executable(MyTest test/test_main.cc test/flash/html/test_html_document_parser.cc test/flash/html/test_dom.cc test/flash/html/test_tokenizer.cc test/flash/html/test_html_util.cc  test/utils/test_log_util.cc test/utils/test_simple_http_client.cc)
target_link_libraries(MyTest flash)
target_link_libraries(MyTest ${CURL_LIBRARIES})
INCLUDE_DIRECTORIES(test/flash)
INCLUDE_DIRECTORIES(test/utils)
target_link_libraries(MyTest gtest)
target_link_libraries(MyTest gtest_main)

