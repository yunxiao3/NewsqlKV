# This is hacky :(
file(GLOB_RECURSE newsql_kv_sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*/*.cpp)
add_library(newsql_kv_shared SHARED ${newsql_kv_sources})

######################################################################################################################
# THIRD-PARTY SOURCES
######################################################################################################################

# murmur3
file(GLOB_RECURSE murmur3_sources
        ${PROJECT_SOURCE_DIR}/third_party/murmur3/*.cpp ${PROJECT_SOURCE_DIR}/third_party/murmur3/*.h)
add_library(thirdparty_murmur3 SHARED ${murmur3_sources})
target_link_libraries(newsql_kv_shared thirdparty_murmur3)