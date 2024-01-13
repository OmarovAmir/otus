cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VESRION})

include(FetchContent)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package (SQLite3)

configure_file(version.h.in version.h)

include_directories(.)
include_directories(./lib)
include_directories(./server)

add_library(async SHARED ./lib/async.cpp)

set_target_properties(async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(async PRIVATE 
                    Threads::Threads
                    fmt::fmt-header-only)

add_executable(join_server main.cpp)

target_link_libraries(join_server
                async
                ${Boost_LIBRARIES}
                SQLite::SQLite3
                Threads::Threads
                fmt::fmt-header-only)

set_target_properties(join_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_options(async PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror -O3
    )
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror -O3
    )
endif()



install(TARGETS join_server RUNTIME DESTINATION bin)
install(TARGETS async RUNTIME DESTINATION lib)
install(FILES lib/async.hpp DESTINATION include)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
