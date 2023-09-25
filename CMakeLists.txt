cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(fast_allocator VERSION ${PROJECT_VESRION})

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

configure_file(version.h.in version.h)

add_executable(fast_allocator main.cpp)
add_executable(test_fast_allocator test_fast_allocator.cpp)
add_library(fast_allocator_lib fast_allocator.cpp)

set_target_properties(fast_allocator test_fast_allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(fast_allocator_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(fast_allocator
    fast_allocator_lib
)

target_link_libraries(test_fast_allocator
    GTest::gtest_main
    fast_allocator_lib
)

if (MSVC)
    target_compile_options(fast_allocator PRIVATE
        /W4
    )
    target_compile_options(test_fast_allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(fast_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_fast_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS fast_allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_fast_allocator test_fast_allocator)
