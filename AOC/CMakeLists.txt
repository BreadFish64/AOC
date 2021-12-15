find_package(fmt REQUIRED)
find_package(glad REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Boost REQUIRED)

function(Problem problem_name)

add_executable (${problem_name}
    ${problem_name}.cpp
    types.hpp
)

target_link_libraries(${problem_name} PRIVATE fmt::fmt)

set_target_properties(${problem_name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${problem_name}"
)

add_custom_command(
        TARGET ${problem_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/input/${problem_name}.txt
                ${CMAKE_BINARY_DIR}/${problem_name}/input.txt)

endfunction()

Problem(report_repair)
Problem(password_philosophy)
Problem(toboggan_trajectory)
Problem(passport_processing)
Problem(binary_boarding)
Problem(custom_customs)
# Problem(handy_haversacks)
Problem(handheld_halting)
Problem(encoding_error)
Problem(adapter_array)
Problem(seating_system)
Problem(rain_risk)
Problem(shuttle_search)
Problem(docking_data)
Problem(rambunctious_recitation)
Problem(conway_cubes)
add_custom_command(
        TARGET conway_cubes POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/conway_cubes.comp
                ${CMAKE_BINARY_DIR}/conway_cubes/conway_cubes.comp)
target_sources(conway_cubes PRIVATE open_gl.hpp sdl_helper.hpp)
target_link_libraries(conway_cubes PRIVATE glad::glad SDL2::SDL2)

Problem(sonar_sweep)
Problem(dive)
Problem(binary_diagnostic)
Problem(giant_squid)
target_link_libraries(giant_squid PRIVATE Boost::boost)
Problem(hydrothermal_venture)
Problem(lanternfish)
Problem(treachery_of_whales)
Problem(seven_segment_search)
Problem(smoke_basin)
Problem(syntax_scoring)
Problem(dumbo_octopus)
Problem(passage_pathing)
Problem(transparent_origami)
Problem(extended_polymerization)
Problem(chiton)
