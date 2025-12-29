cmake_minimum_required(VERSION 3.21)

project(libecs VERSION 0.1.0)

add_library(libecs INTERFACE)

target_include_directories(libecs INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(
    DIRECTORY include/
    DESTINATION include
)

install(
    TARGETS libecs
    EXPORT libecsTargets
)

install(
    EXPORT libecsTargets
    NAMESPACE libecs::
    DESTINATION lib/cmake/libecs
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/libecsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/libecsConfig.cmake
    INSTALL_DESTINATION lib/cmake/libecs
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/libecsConfig.cmake
    DESTINATION lib/cmake/libecs
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libecsConfigVersion.cmake
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/libecsConfigVersion.cmake
    DESTINATION lib/cmake/libecs
)