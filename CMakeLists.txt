cmake_minimum_required(VERSION 3.0)

project(p2t)

set(LEXER_SOURCE lex.l)
set(PARSER_SOURCE p2t_parser.y)


include_directories("${CMAKE_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}")

add_custom_command(
	OUTPUT
		"${CMAKE_BINARY_DIR}/${PARSER_SOURCE}.cc"
		"${CMAKE_BINARY_DIR}/${PARSER_SOURCE}.hh"
		"${CMAKE_BINARY_DIR}/stack.hh"
	COMMAND
		bison -Lc++ -t -d ${PARSER_SOURCE} -o ${CMAKE_BINARY_DIR}/${PARSER_SOURCE}.cc
	DEPENDS
		${PARSER_SOURCE}
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
)

add_custom_command(
	OUTPUT
		"${CMAKE_BINARY_DIR}/${LEXER_SOURCE}.cc"
	COMMAND
		flex -o ${CMAKE_BINARY_DIR}/${LEXER_SOURCE}.cc ${LEXER_SOURCE}
	DEPENDS
		${LEXER_SOURCE}
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
)
add_custom_target(
	p2t_yy_generated
	DEPENDS
	"${CMAKE_BINARY_DIR}/${LEXER_SOURCE}.cc"
	"${CMAKE_BINARY_DIR}/${PARSER_SOURCE}.cc"
	"${CMAKE_BINARY_DIR}/${PARSER_SOURCE}.hh"
	"${CMAKE_BINARY_DIR}/stack.hh"
)

add_library(
	p2t_yy
	SHARED
	"${CMAKE_BINARY_DIR}/${PARSER_SOURCE}.cc"
	"${CMAKE_BINARY_DIR}/${LEXER_SOURCE}.cc"
)
add_dependencies(p2t_yy p2t_yy_generated)

add_executable(
	p2t_app
	p2t_driver.cpp
	api.h
)
add_dependencies(p2t_app p2t_yy)
target_link_libraries(p2t_app p2t_yy)
SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_custom_target(
	p2t
	ALL
	DEPENDS
		p2t_app
		p2t_yy
	SOURCES
		lex.l
		p2t_parser.y
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
)
install(
	TARGETS
		p2t_app
	DESTINATION
		/home/amir/Desktop
)
