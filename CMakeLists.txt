cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

# Set the default CMAKE_BUILD_TYPE to Release, should be done before the project command since the latter can set
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

project(ring_span_test)

message(STATUS "${PROJECT_NAME} CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
  string (REPLACE "/W3" "" CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}"  )
  string (REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string (REGEX REPLACE ".INCREMENTAL[:NO]*" "/INCREMENTAL:NO" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
  set(PEDANTIC_COMPILE_FLAGS /W4 /permissive-)
  set(WERROR_FLAG /WX)
else()
  set(PEDANTIC_COMPILE_FLAGS -pedantic-errors -Wall -Wextra -pedantic)
  set(WERROR_FLAG -Werror)
endif ()

add_executable(${PROJECT_NAME} src/test.cpp)

target_compile_options(${PROJECT_NAME} PUBLIC ${PEDANTIC_COMPILE_FLAGS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)
