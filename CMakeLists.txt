cmake_minimum_required(VERSION 4.0..4.2)

project(ConceptMap VERSION 0.0.0 LANGUAGES CXX)

# Enable experimental feature!!
set(CMAKE_EXPERIMENTAL_INSTRUMENTATION ec7aa2dc-b87f-45a3-8022-fe01c5f59984)

# Instrumentation query
# find_package(Python COMPONENTS Interpreter)
cmake_instrumentation(
  API_VERSION 1
  DATA_VERSION 1
  HOOKS postGenerate preBuild postBuild preCMakeBuild postCMakeBuild postCMakeInstall postCTest
  OPTIONS trace
  CALLBACK ${CMAKE_SOURCE_DIR}/scripts/instrument.sh
)

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
    find_package(GTest CONFIG QUIET)
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

# Coverage
configure_file("cmake/gcovr.cfg.in" gcovr.cfg @ONLY)

add_custom_target(
    process_coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running gcovr to process coverage results"
    COMMAND mkdir -p coverage
    COMMAND gcovr --config gcovr.cfg .
)
