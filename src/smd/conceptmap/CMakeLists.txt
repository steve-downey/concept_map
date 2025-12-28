include(GNUInstallDirs)

target_sources(
    concept_map
    PUBLIC FILE_SET concept_map_headers TYPE HEADERS FILES monoid.h functor.h
)

install(
    TARGETS concept_map
    FILE_SET concept_map_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT concept_map_development
)

install(
    TARGETS concept_map
    EXPORT concept_map_export
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET concept_map_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT concept_map_development
)

# install(
#   EXPORT concept_map_export
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/smd/
#   NAMESPACE SMD::ConceptMap::
#   FILE concept_map.cmake
#   EXPORT_LINK_INTERFACE_LIBRARIES
#   COMPONENT concept_map_development
# )

## Tests
add_executable(monoid_test "")

target_sources(monoid_test PRIVATE monoid.t.cpp functor.t.cpp)

target_link_libraries(monoid_test concept_map)
target_link_libraries(monoid_test GTest::gtest)
target_link_libraries(monoid_test GTest::gtest_main)
target_link_libraries(monoid_test beman::optional)

include(GoogleTest)
gtest_discover_tests(monoid_test)
