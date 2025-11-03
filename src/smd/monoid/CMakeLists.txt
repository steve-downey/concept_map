add_library(monoid STATIC "")

target_sources(monoid PRIVATE monoid.cpp)

include(GNUInstallDirs)

target_include_directories(
    monoid
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}> # <prefix>/include/scratch
)

install(
    TARGETS monoid
    EXPORT ${TARGETS_EXPORT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
    FILES_MATCHING
    PATTERN "*.h"
)

target_link_libraries(monoid)

## Tests
add_executable(monoid_test "")

target_sources(monoid_test PRIVATE monoid.t.cpp)

target_link_libraries(monoid_test monoid)
target_link_libraries(monoid_test gtest)
target_link_libraries(monoid_test gtest_main)

include(GoogleTest)
gtest_discover_tests(monoid_test)
