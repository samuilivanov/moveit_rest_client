add_executable(test_moveit_client test_moveit_client.cpp)

target_compile_features(test_moveit_client PRIVATE cxx_std_17)

target_link_libraries(test_moveit_client PRIVATE client)

add_test(NAME test_moveit_client COMMAND test_moveit_client) # Command can be a target
