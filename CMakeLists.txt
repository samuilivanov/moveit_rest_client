cmake_minimum_required(VERSION 3.15...4.0)

project(
  moveit_rest_client
  VERSION 0.1
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  set(CMAKE_CXX_EXTENSIONS OFF)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(CTest)

endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE) # TODO (samuil) del this
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.12.0)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.2.0   # pick a release tag
)

FetchContent_MakeAvailable(cxxopts)
add_subdirectory(src)

add_subdirectory(app)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    # List of source files for cppcheck (could be entire src dir)
    file(GLOB_RECURSE CPP_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
    )

    set(INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )

    set(CPPCHECK_OPTIONS
        --enable=all
        --inconclusive
        --force
        --std=c++17
        --inline-suppr
        --verbose
        --suppress=missingIncludeSystem
    )

    add_custom_target(cppcheck
        COMMAND ${CPPCHECK} ${CPPCHECK_OPTIONS}
                -I"${INCLUDE_DIRS}"
                ${CPP_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running cppcheck..."
        VERBATIM
    )
endif()

find_program(CLANG_TIDY clang-tidy)

if(CLANG_TIDY)
    file(GLOB_RECURSE CPP_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
    )

    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY} -p=${CMAKE_BINARY_DIR} ${CPP_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running clang-tidy on all source files..."
        VERBATIM
    )
endif()