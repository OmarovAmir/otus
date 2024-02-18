cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(adapter VERSION ${PROJECT_VESRION})

include(FetchContent)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost REQUIRED COMPONENTS system program_options)

configure_file(version.h.in version.h)

include_directories(.)
include_directories(./adapter)

add_executable(adapter main.cpp)

target_link_libraries(adapter
                ${Boost_LIBRARIES}
                Threads::Threads
                fmt::fmt-header-only)

set_target_properties(adapter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(adapter PRIVATE
        /W4
    )
else ()
    target_compile_options(adapter PRIVATE
        -Wall -Wextra -pedantic -Werror -O3
    )
endif()

install(TARGETS adapter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
