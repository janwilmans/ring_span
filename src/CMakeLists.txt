
add_executable(${PROJECT_NAME} catch_main.cpp fixed_string_tests.cpp)
target_link_libraries(${PROJECT_NAME} fixed_string)

target_compile_options(${PROJECT_NAME} PRIVATE ${PEDANTIC_COMPILE_FLAGS} ${WERROR_FLAG})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
