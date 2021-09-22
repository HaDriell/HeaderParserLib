enable_testing()

file(GLOB_RECURSE SOURCES "${HeaderParserLib_SOURCE_DIR}/tests/*.cpp")

add_executable(HeaderParserLibTests ${SOURCES})
target_include_directories(HeaderParserLibTests PUBLIC "${HeaderParserLib_SOURCE_DIR}/tests")
target_link_libraries(HeaderParserLibTests PUBLIC HeaderParserLib)
target_link_libraries(HeaderParserLibTests PUBLIC gtest_main)

include(GoogleTest)
gtest_discover_tests(HeaderParserLibTests)