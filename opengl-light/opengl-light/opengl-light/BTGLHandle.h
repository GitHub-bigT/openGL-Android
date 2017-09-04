#pragma once

#include "BTGL.h"

class Handle{
public:
	enum Shape{
		TRIANGLE
	};
	BTBind *bt_bind;
	BTProgram *bt_program;

	Handle(){
		bt_bind = new BTBind();
		bt_program = new BTProgram();
	}

	void init(Shape shape){
		if (shape == this->TRIANGLE)
		{
			bt_bind->init(bt_bind->TRIANGLE);
			bt_program->createProgram();
			bt_program->useProgram();
		}
	}
	void draw(Shape shape){
		bt_bind->draw(bt_bind->TRIANGLE,bt_program->programId);
	}
};