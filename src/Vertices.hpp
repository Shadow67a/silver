#ifndef VERTICES_HPP
#define VERTICES_HPP

const float triangleVertices[] = {
	-0.5f, -0.5f, 0.0f, 0, 0,
	0.5f, -0.5f, 0.0f, 1, 0,
	0.0f,  0.5f, 0.0f, 0.5, 1
};

const float quadVertices[] = {
        -0.5f, 0.5f, 0.0f, 0, 1,
        -0.5f, -0.5f, 0.0f, 0, 0,
        0.5f, -0.5f, 0.0f, 1, 0,
        0.5f, -0.5f, 0.0f, 1, 0,
        0.5f, 0.5f, 0.0f, 1, 1,
        -0.5f, 0.5f, 0.0f, 0, 1
};

const float viewVertices[] = {
	-1.0, 1.0, 0.0, 1.0,
	-1.0, -1.0, 0.0, 0.0,
	1.0, -1.0, 1.0, 0.0,

	-1.0, 1.0, 0.0, 1.0,
	1.0, -1.0, 1.0, 0.0,
	1.0, 1.0, 1.0, 1.0
};

#endif
