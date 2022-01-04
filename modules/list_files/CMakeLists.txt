project(list_files)

set(SOURCE_FILES src/main.cpp)
set(HEADER_FILES 
	src/project_headers.h
	src/comparator.h
	src/cplusplus.h
	src/data.h
	)
set(RESOURCE_FILES
	res/resource.h
	res/graphicList.rc
	)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
	${RESOURCE_FILES}
	)
target_precompile_headers(${PROJECT_NAME}
	PRIVATE
		src/project_headers.h
		)
target_include_directories(${PROJECT_NAME}
	PRIVATE
		res
		)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		Comctl32.lib
		)
add_compile_definitions(UNICODE _UNICODE)
