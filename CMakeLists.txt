cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(pool_allocator VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(pool_allocator main.cpp)

set_target_properties(pool_allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(pool_allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(pool_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror -g
    )
endif()



install(TARGETS pool_allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
