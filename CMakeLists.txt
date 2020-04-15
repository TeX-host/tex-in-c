cmake_minimum_required(VERSION 3.10)

# set the project name
project(TTeX C)
# set the project name and version
project(TTeX VERSION 3.14159)

# add the executable
set(CSRC_LIST tex.c funcs.c inipool.c inputln.c printout.c str.c fonts.c dviout.c pure_func.c)
add_executable(ttex ${CSRC_LIST})

### flags
# -std=c11
set_property(TARGET ttex PROPERTY C_STANDARD 11)

# link math
target_link_libraries(ttex m)
