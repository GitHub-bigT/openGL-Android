#pragma once

#define GLEW_STATIC
#include <../GL/glew.h>
#include <../GLFW/glfw3.h>
#include <../glm/gtc/matrix_transform.hpp>
#include <iostream>


class BTBind{
public:
	enum Shape{
		TRIANGLE
	};
	void init(Shape shape);
private:
	void initTriangle();
};