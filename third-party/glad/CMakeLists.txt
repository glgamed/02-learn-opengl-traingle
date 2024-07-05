set(LIB glad)
set(SRC glad.c)

add_library(${LIB} STATIC ${SRC})
target_include_directories(${LIB}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)