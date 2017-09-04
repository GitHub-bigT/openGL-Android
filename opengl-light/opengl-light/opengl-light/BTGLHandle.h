#pragma once

#include "BTGL.h"

class Handle{
public:
	enum Shape{
		TRIANGLE
	};
	BTBind *bind;
	Handle(){
		bind = new BTBind();
	}
	void init(Shape shape){
		if (shape == this->TRIANGLE)
		{
			bind->init(bind->TRIANGLE);
		}
	}
	void draw(Shape shape){
	
	}
};