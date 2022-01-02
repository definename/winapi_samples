project(null_gui)

set(SOURCE_FILES src/main.cpp)
set(HEADER_FILES src/project_headers.h)
set(RESOURCE_FILES res/HELLOWIN.WAV)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME}
	PRIVATE
		src/project_headers.h)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		Winmm.lib
		)

add_compile_definitions(UNICODE _UNICODE)

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_LIST_DIR}/res/HELLOWIN.WAV
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/HELLOWIN.WAV
	COMMENT
		"Copying *.WAV file..."
	VERBATIM
	)