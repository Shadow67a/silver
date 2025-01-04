
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} 
PRIVATE
	./glad.c
	./App.cpp
	./View.cpp
	./Lib.cpp
)

target_include_directories(${PROJECT_NAME}
PRIVATE
	#Internal library headers and dependencies.
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm
PUBLIC
	#Library's headers for top-level projects.
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>

	#Library's headers for external projects.
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} glm)
