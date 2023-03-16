file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/src/*.h)
add_library(zcoco STATIC ${srcs})
target_link_libraries(zcoco ${CMAKE_DL_LIBS}) 	#dlsym
target_link_libraries(zcoco ${PROTOBUF_LIBRARIES}) #protobuf
target_link_libraries(zcoco ${ZLIB_LIBRARIES})	#zlib