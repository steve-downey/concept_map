include(GNUInstallDirs)

add_executable(main "")

target_sources(main PRIVATE main.cpp)

target_link_libraries(main concept_map)

add_executable(map_example "")

target_sources(map_example PRIVATE map_example.cpp)

target_link_libraries(map_example)

add_executable(cpo_example "")

target_sources(cpo_example PRIVATE cpo_example.cpp)

target_link_libraries(cpo_example)

install(
    TARGETS main map_example cpo_example
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
