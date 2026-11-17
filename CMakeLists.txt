cmake_minimum_required(VERSION 3.14)
project(happly VERSION 1.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(happly INTERFACE)
add_library(happly::happly ALIAS happly)

target_include_directories(happly INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS happly
    EXPORT happlyTargets
)

install(EXPORT happlyTargets
    FILE happlyTargets.cmake
    NAMESPACE happly::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/happly
)

install(FILES happly.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_package_config_file(cmake/happlyConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/happlyConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/happly
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/happlyConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/happlyConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/happlyConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/happly
)
