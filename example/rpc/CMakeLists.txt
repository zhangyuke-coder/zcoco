
add_executable(rpc_server  rpc_server.cpp echo.pb.cc args.pb.cc)
target_link_libraries(rpc_server zcoco)

add_executable(rpc_client  rpc_client.cpp echo.pb.cc args.pb.cc)
target_link_libraries(rpc_client zcoco)


