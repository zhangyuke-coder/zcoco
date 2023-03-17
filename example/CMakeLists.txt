add_subdirectory(rpc)
add_executable(sudoku sudoku.cpp)
target_link_libraries(sudoku zcoco)



add_executable(sudoku_pipeline_test sudoku_pipeline_test.cpp)
target_link_libraries(sudoku_pipeline_test zcoco)



add_executable(sudoku_batch_test sudoku_batch_test.cpp)
target_link_libraries(sudoku_batch_test zcoco)



# add_executable(PrintABC PrintABC.cpp)
# target_link_libraries(PrintABC zcoco)



# add_executable(log_test log_test.cpp)
# target_link_libraries(log_test zcoco)



# add_executable(Address_test Address_test.cpp)
# target_link_libraries(Address_test zcoco)





# add_executable(Coroutine_test Coroutine_test.cpp)
# target_link_libraries(Coroutine_test zcoco)


# add_executable(EventLoopThreadPool_test EventLoopThreadPool_test.cpp)
# target_link_libraries(EventLoopThreadPool_test zcoco)


# add_executable(Hook_test Hook_test.cpp)
# target_link_libraries(Hook_test zcoco)


# add_executable(TcpServer_test TcpServer_test.cpp)
# target_link_libraries(TcpServer_test zcoco)


# add_executable(TcpClient_test TcpClient_test.cpp)
# target_link_libraries(TcpClient_test zcoco)

# add_executable(Timer_test Timer_test.cpp)
# target_link_libraries(Timer_test zcoco)