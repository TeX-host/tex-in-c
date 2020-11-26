cmake_minimum_required(VERSION 3.10)

# set the project name
project(TTeX C)

# set the project name and version
project(TTeX VERSION 3.14159)

# add the executable
set(EXEC_NAME ttex)

set(CSRC_LIST tex.c main_ctrl.c)
list(APPEND CSRC_LIST charset.c mem.c str.c)
list(APPEND CSRC_LIST io.c io_fname.c io_unix.c)
list(APPEND CSRC_LIST eqtb.c eqtb_save.c)
list(APPEND CSRC_LIST texmath.c math_formula.c mmode.c)
list(APPEND CSRC_LIST scan.c lexer.c expand.c)
list(APPEND CSRC_LIST box.c box_copy.c box_destroy.c box_display.c)
list(APPEND CSRC_LIST hyphen.c hyphen_init.c hyphen_post.c hyphen_pre.c)
list(APPEND CSRC_LIST error.c debug.c dump.c extension.c)
list(APPEND CSRC_LIST align.c  fonts.c hash.c print.c printout.c)
list(APPEND CSRC_LIST linebreak.c pack.c page_builder.c pagebreak.c)
list(APPEND CSRC_LIST dviout.c)

add_executable(${EXEC_NAME} ${CSRC_LIST})

### flags
# -std=c11
set_property(TARGET ttex PROPERTY C_STANDARD 11)
if (MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wno-unused-result)
endif()

# link math
if(NOT MSVC)
    target_link_libraries(ttex m)
endif(NOT MSVC)
