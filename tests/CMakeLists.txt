file(GLOB_RECURSE SOURCES "${HeaderParserLib_SOURCE_DIR}/tests/*.cpp")


add_executable(Example ${SOURCES})
target_include_directories(HeaderParserLib PUBLIC "${HeaderParserLib_SOURCE_DIR}/tests")
target_link_libraries(Example HeaderParserLib)