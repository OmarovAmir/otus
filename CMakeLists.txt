cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VESRION})

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

configure_file(version.h.in version.h)

add_executable(print_ip main.cpp)
add_executable(test_print_ip test_print_ip.cpp)

set_target_properties(print_ip test_print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(test_print_ip
    GTest::gtest_main
)

if (MSVC)
    target_compile_options(print_ip PRIVATE
        /W4
    )
    target_compile_options(test_print_ip PRIVATE
        /W4
    )
else ()
    target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_print_ip test_print_ip)
