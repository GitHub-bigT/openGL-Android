#pragma once

#include "BTGL.h"
#include "BTShader.h"

class Handle{
public:
	enum Shape{
		TRIANGLE
	};
	BTBind *bt_bind;
	BTShader *bt_shader;
	BTShader *bt_shader_lamp;

	Handle(){
		bt_bind = new BTBind();	
	}

	void init(Shape shape){
		if (shape == this->TRIANGLE)
		{
			bt_bind->init(bt_bind->TRIANGLE);
			bt_shader = new BTShader("v_shader_triangle.vert", "f_shader_triangle.frag");
			bt_shader_lamp = new BTShader("v_shader_lamp.vert", "f_shader_lamp.frag");
		}
	}
	void draw(Shape shape , glm::mat4 viewMatrix){
		bt_bind->draw(bt_bind->TRIANGLE, bt_shader, bt_shader_lamp,viewMatrix);
	}
};