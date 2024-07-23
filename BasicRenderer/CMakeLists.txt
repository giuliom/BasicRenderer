cmake_minimum_required(VERSION 3.19)
project(BasicRenderer)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
# Avoid warnings by using new policy
cmake_policy(SET CMP0135 NEW)

# ------ BasicCppLibrary ----- #

# BasicCppLibrary
FetchContent_Declare(
  BasicCppLibrary
  GIT_REPOSITORY https://github.com/giuliom/BasicCppLibrary.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(BasicCppLibrary)

# ------ BasicRenderer ----- #

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/source/*.cpp" "${PROJECT_SOURCE_DIR}/include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC BasicCppLibrary m)
add_dependencies(${PROJECT_NAME} BasicCppLibrary)

# ------ Google Test ----- #

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file (GLOB TESTS "test/*.h" "test/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp")
add_executable(BasicRendererTest ${TESTS} ${SOURCES})
target_include_directories(BasicRendererTest PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(
  BasicRendererTest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(BasicRendererTest)