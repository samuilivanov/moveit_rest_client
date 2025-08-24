file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${moveit_rest_client_SOURCE_DIR}/include/*.h")

add_library(client cli.cpp cpr_http_client.cpp moveit_client.cpp ${HEADER_LIST})

target_include_directories(client PUBLIC ../include)
target_link_libraries(client PUBLIC cpr::cpr nlohmann_json::nlohmann_json)

target_compile_features(client PUBLIC cxx_std_17)
    target_compile_options(client PRIVATE
        -Wall           # All common warnings
        -Wextra         # Extra warnings
        -Wpedantic      # Standard conformance warnings
        -Wshadow        # Warn on variable shadowing
        -Wnon-virtual-dtor  # Warn when deleting polymorphic objects without virtual destructor
        -Wold-style-cast    # Warn on C-style casts
        -Wcast-align        # Warn on bad alignment casts
        -Wunused            # Warn on unused variables/functions
        -Woverloaded-virtual # Warn if a virtual function is hidden
        -Wformat=2          # More strict printf/scanf checking
        -fno-omit-frame-pointer # Helps debugging/backtraces
        -g                   # Include debug info
    )
# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})