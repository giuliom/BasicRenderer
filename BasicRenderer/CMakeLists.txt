cmake_minimum_required(VERSION 3.19)
project(BasicRenderer)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
# Avoid warnings by using new policy
cmake_policy(SET CMP0135 NEW)


# ------ BasicRenderer Library ----- #

set (LIBRARY_NAME BasicRendererLib)
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/source/*.cpp" "${PROJECT_SOURCE_DIR}/include/*.h")
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
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file (GLOB TESTS "test/*.h" "test/*.cpp")
add_executable(BasicRendererTest ${TESTS} ${SOURCES})
target_include_directories(BasicRendererTest PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(
  BasicRendererTest
  ${LIBRARY_NAME}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(BasicRendererTest)

# ------ BasicRenderer Executable ----- #

add_executable(${PROJECT_NAME} ${SOURCES} "${PROJECT_SOURCE_DIR}/source/main.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_NAME})