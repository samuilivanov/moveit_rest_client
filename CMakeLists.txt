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

# set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE) # TODO (samuil) del this
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.12.0)
                         FetchContent_MakeAvailable(cpr)

add_subdirectory(src)

add_subdirectory(app)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
