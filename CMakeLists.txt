cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk_mtd VERSION ${PROJECT_VESRION})

find_package(Threads REQUIRED)

configure_file(version.h.in version.h)

include_directories(.)
include_directories(./lib)

add_library(async SHARED ./lib/async.cpp)

set_target_properties(async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(async PRIVATE Threads::Threads)

add_executable(bulk_mtd main.cpp)

target_link_libraries(bulk_mtd async)

set_target_properties(bulk_mtd PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(bulk_mtd PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_mtd PRIVATE
        -Wall -Wextra -pedantic -Werror -O3
    )
endif()



install(TARGETS bulk_mtd RUNTIME DESTINATION bin)
install(TARGETS async RUNTIME DESTINATION lib)
install(FILES lib/async.hpp DESTINATION include)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
