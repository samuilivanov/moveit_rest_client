add_executable(app app.cpp)
target_compile_features(app PRIVATE cxx_std_17)

target_link_libraries(app PRIVATE client cxxopts)
target_compile_options(app PRIVATE
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