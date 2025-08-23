file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${moveit_rest_client_SOURCE_DIR}/include/*.h")

add_library(client cli.cpp cpr_http_client.cpp moveit_client.cpp ${HEADER_LIST})

target_include_directories(client PUBLIC ../include)
target_link_libraries(client PUBLIC cpr::cpr nlohmann_json::nlohmann_json)

target_compile_features(client PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
