set(BIN triangle)
set(SRC main.c)

add_executable(${BIN} ${SRC})
target_include_directories(${BIN}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${BIN}
    glad
    glfw
)