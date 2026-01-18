cmake_minimum_required(VERSION 3.19)
project(BasicRenderer)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories for all configurations
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT_DIR}/build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT_DIR}/build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_ROOT_DIR}/build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${PROJECT_ROOT_DIR}/build/bin)

include(FetchContent)
# Avoid warnings by using new policy
cmake_policy(SET CMP0135 NEW)


# ------ BasicRenderer Library ----- #

set (LIBRARY_NAME BasicRendererLib)
file(GLOB SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.cpp" "${PROJECT_SOURCE_DIR}/include/*.h")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/source/main.cpp")

add_library(${LIBRARY_NAME} STATIC ${SOURCES})
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Set the library version (optional)
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)


# ------ Google Test ----- #

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file (GLOB TESTS "test/*.h" "test/*.cpp")
add_executable(BasicRendererTest ${TESTS})
target_include_directories(BasicRendererTest PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(
  BasicRendererTest
  ${LIBRARY_NAME}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(BasicRendererTest)

# ------ BasicRenderer Executable ----- #

add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/source/main.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_NAME})