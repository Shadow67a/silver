add_library(silver STATIC)

target_sources(silver 
PRIVATE
	./glad.c
	./Lib.cpp
	./App.cpp
	./View.cpp
	./Shader.cpp
	./Elements.cpp
	./Text.cpp
	./Animation.cpp
	./State.cpp
)

target_include_directories(silver
PRIVATE
	#Internal library headers and dependencies.
	${CMAKE_CURRENT_SOURCE_DIR}/src
#	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include
#	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm
#	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include
PUBLIC
	#Library's headers for top-level projects.
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>

	#Library's headers for external projects.
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(silver glfw ${GLFW_LIBRARIES} glm::glm Freetype::Freetype)
