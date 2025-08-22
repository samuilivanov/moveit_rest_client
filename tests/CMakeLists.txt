enable_testing()

function(add_doctest name lib)
add_executable(${name} ${name}.cpp)
target_compile_features(${name} PRIVATE cxx_std_17)
target_link_libraries(${name} PRIVATE ${lib})
endfunction()

add_doctest(test_moveit_client client)
add_doctest(test_cpr_http_client client)

add_test(NAME test_moveit_client COMMAND test_moveit_client)
add_test(NAME test_cpr_http_client COMMAND test_cpr_http_client)
