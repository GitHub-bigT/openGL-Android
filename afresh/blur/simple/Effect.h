#pragma once
#include <glad/glad.h>

class Effect
{
public:
	Effect();
	~Effect();
	void GaussianBlurGPU(float radius, GLuint &tex);

private:

};

