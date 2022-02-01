cmake_minimum_required( VERSION 3.5 )

set( MODULE utility )

set( SCL_MODULE ScL::${MODULE} )
set( TARGET scl-${MODULE} )

project( ${TARGET} VERSION 0.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_library( ${TARGET} INTERFACE )
add_library( ${SCL_MODULE} ALIAS ${TARGET} )

target_include_directories( ${TARGET} INTERFACE ${SCL_INCLUDE_DIR} )
