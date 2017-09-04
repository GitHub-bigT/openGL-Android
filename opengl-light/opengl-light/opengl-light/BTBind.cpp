#include "BTGL.h"

BTError *error = new BTError();

void BTBind::init(Shape shape){
	if (shape == this->TRIANGLE)
	{
		this->initTriangle();
	}
}

void BTBind::draw(Shape shape, BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, GLfloat Zoom){
	if (shape == this->TRIANGLE)
	{
		this->drawTriangle(bt_shader,bt_shader_lamp,viewMatrix,Zoom);
	}
}

void BTBind::initTriangle(){
	//vao
	glGenVertexArrays(NumVAOIds,VAOs);
	glBindVertexArray(VAOs[TriangleVAO]);
	//check VAO bind status
	GLboolean vao_b = glIsVertexArray(VAOs[TriangleVAO]);
	printf("triangle vao bind status:%d\n",vao_b);

	//vbo
	glGenBuffers(NumVBOIds,VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	//check VBO bind status
	GLboolean vbo_b = glIsBuffer(VBOs[TriangleVBO]);
	printf("triangle vbo bind status:%d\n",vbo_b);
	GLfloat triangleVertexs[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexs), triangleVertexs, GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	error->glCheckError();
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);

	//光源
	glBindVertexArray(VAOs[LightVAO]);
	error->glCheckError();
	//check VAO bind status
	GLboolean vao_light_b = glIsVertexArray(VAOs[LightVAO]);
	printf("light vao bind status:%d\n", vao_light_b);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	error->glCheckError();
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);
}

void BTBind::drawTriangle(BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, GLfloat Zoom){
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//投影矩阵
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

	//立方体
	bt_shader->Use();
	GLfloat currentTime = glfwGetTime();
	//模型矩阵
	glm::mat4 model;
	//model = glm::translate(model,glm::vec3(0.0f,0.0f,0.0f));
	//model = glm::rotate(model, glm::radians(currentTime) * 100, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program,"model"),1,GL_FALSE,glm::value_ptr(model));
	//视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//投影矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//颜色
	glUniform3f(glGetUniformLocation(bt_shader->program, "objectColor"), 1.0f, 0.5f, 0.31f);//珊瑚红
	glUniform3f(glGetUniformLocation(bt_shader->program, "lightColor"), 1.0f, 1.0f, 1.0f);
	glBindVertexArray(VAOs[TriangleVAO]);
	glDrawArrays(GL_TRIANGLES,0,36);

	//光源
	bt_shader_lamp->Use();
	//模型矩阵
	glm::mat4 model_light;
	model_light = glm::translate(model_light, glm::vec3(1.2f,1.0f,2.0f));
	model_light = glm::scale(model_light,glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "model"), 1, GL_FALSE, glm::value_ptr(model_light));
	//视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//投影矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAOs[LightVAO]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}