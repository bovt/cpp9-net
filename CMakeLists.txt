cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VESRION})

file(GLOB_RECURSE SRC src/main.cpp
                      src/bulk_reader.cpp
                      src/async.cpp
                      src/async_server.cpp
                      src/async_session.cpp
)
file(GLOB_RECURSE H "include/*.h")

add_executable(${PROJECT_NAME} ${SRC} ${H})
target_link_libraries(${PROJECT_NAME} pthread)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_PREFIX_PATH}/include
)
link_directories(
    ${CMAKE_PREFIX_PATH}/lib
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
