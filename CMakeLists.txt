cmake_minimum_required(VERSION 3.27)

project(ConceptMap VERSION 0.0.0 LANGUAGES CXX)

# Includes
include(CTest)
include(FetchContent)

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

option(
    OPTIONAL_ENABLE_TESTING
    "Enable building tests and test infrastructure"
    ${PROJECT_IS_TOP_LEVEL}
)

set(CMAKE_VERIFY_INTERFACE_HEADER_SETS ON)

if(OPTIONAL_ENABLE_TESTING)
    find_package(GTest QUIET)
endif()

# Create the library target and named header set for beman_optional
add_library(concept_map INTERFACE)
target_sources(
  concept_map
  PUBLIC FILE_SET concept_map_headers TYPE HEADERS BASE_DIRS src
)

# add_subdirectory(extern)
add_subdirectory(src)

include(GNUInstallDirs)

# set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

# install(
#     EXPORT ${TARGETS_EXPORT_NAME}
#     NAMESPACE ${CMAKE_PROJECT_NAME}
#     DESTINATION ${INSTALL_CONFIGDIR}
# )

# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
#     ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

# configure_package_config_file(
#     "cmake/Config.cmake.in"
#     ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
#     INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
# )

# install(
#     FILES
#         ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
#         ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
#     DESTINATION ${INSTALL_CONFIGDIR}
# )

# Coverage
configure_file("cmake/gcovr.cfg.in" gcovr.cfg @ONLY)

add_custom_target(
    process_coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running gcovr to process coverage results"
    COMMAND mkdir -p coverage
    COMMAND gcovr --config gcovr.cfg .
)
