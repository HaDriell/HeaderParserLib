file(GLOB_RECURSE SOURCES "${HeaderParserLib_SOURCE_DIR}/src/*.cpp")


add_library(HeaderParserLib ${SOURCES})
target_include_directories(HeaderParserLib PUBLIC "${HeaderParserLib_SOURCE_DIR}/src")
target_compile_features(HeaderParserLib PUBLIC cxx_std_17)