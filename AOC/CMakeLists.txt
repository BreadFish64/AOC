find_package(fmt REQUIRED)

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
