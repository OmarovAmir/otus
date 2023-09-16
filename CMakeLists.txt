cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VESRION})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

configure_file(version.h.in version.h)

add_executable(ip_filter ip_filter.cpp)
add_executable(test_version_googletest test_version_googletest.cpp)

set_target_properties(ip_filter test_version_googletest PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(ip_filter PRIVATE
    ip_filter
)

target_link_libraries(test_version_googletest
    GTest::gtest_main
    helloworld
)

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    target_compile_options(test_version_googletest PRIVATE
        /W4
    )
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version_googletest PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version_googletest test_version_googletest)
