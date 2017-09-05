#include "BTGL.h"

BTError *error = new BTError();

void BTBind::init(Shape shape){
	if (shape == this->TRIANGLE)
	{
		this->initTriangle();
	}
}

void BTBind::draw(Shape shape, BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, GLfloat Zoom, glm::vec3 cameraPostion){
	if (shape == this->TRIANGLE)
	{
		this->drawTriangle(bt_shader,bt_shader_lamp,viewMatrix,Zoom,cameraPostion);
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
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexs), triangleVertexs, GL_STATIC_DRAW);
	//发送顶点数组数据
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
	//发送法线数据
	glVertexAttribPointer(vNormal , 3 , GL_FLOAT , GL_FALSE , sizeof(GLfloat) * 6 ,(const void*)(sizeof(GLfloat) * 3));
	error->glCheckError();
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glBindVertexArray(0);

	//光源
	glBindVertexArray(VAOs[LightVAO]);
	error->glCheckError();
	//check VAO bind status
	GLboolean vao_light_b = glIsVertexArray(VAOs[LightVAO]);
	printf("light vao bind status:%d\n", vao_light_b);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
	error->glCheckError();
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);
}

GLboolean isAdd = false;
float ambientStrength = 1.0f;
int specularStrength = 1;

void BTBind::drawTriangle(BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, GLfloat Zoom, glm::vec3 cameraPostion){

	GLfloat currentTime = glfwGetTime();

	specularStrength += 1;
	

	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//投影矩阵
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

	GLfloat radius = 2.0f;
	GLfloat speed = 40.0f;
	//光源位置
	glm::vec3 lightPos = glm::vec3(0.0f, -0.5f, 3.0f);
	//glm::vec3 lightPos = glm::vec3(radius * glm::cos(glm::radians(currentTime) * speed), 0.0f, radius * glm::sin(glm::radians(currentTime) * speed));

	//立方体
	bt_shader->Use();
	
	//模型矩阵
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(currentTime) * 100, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.6f * currentTime,1.4f,1.0f));
	//model = glm::scale(model, glm::vec3(currentTime*0.3f, currentTime, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program,"model"),1,GL_FALSE,glm::value_ptr(model));
	//视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//投影矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//观察向量
	glUniform3f(glGetUniformLocation(bt_shader->program, "viewPos"), cameraPostion.x, cameraPostion.y, cameraPostion.z);

	//材质结构体
	glUniform3f(glGetUniformLocation(bt_shader->program, "material.ambient"), 0.0f, 0.1f, 0.06f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "material.diffuse"), 0.0f, 0.50980392f, 0.50980392f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "material.specular"), 0.50196078f, 0.50196078f, 0.50196078f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "material.shininess"), 32.0f);
	//光源结构体
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
	glm::vec3 lightColor;
	lightColor.x = glm::sin(glm::radians(glfwGetTime() * 20.0f));
	lightColor.y = glm::sin(glm::radians(glfwGetTime() * 20.0f));
	lightColor.z = glm::sin(glm::radians(glfwGetTime() * 20.0f));
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.specular"), 1.0f, 1.0f, 1.0f);

	glBindVertexArray(VAOs[TriangleVAO]);
	glDrawArrays(GL_TRIANGLES,0,36);

	//光源
	bt_shader_lamp->Use();
	//模型矩阵
	glm::mat4 model_light;
	model_light = glm::translate(model_light, lightPos);
	model_light = glm::scale(model_light,glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "model"), 1, GL_FALSE, glm::value_ptr(model_light));
	//视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//投影矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//光源颜色
	glUniform3f(glGetUniformLocation(bt_shader_lamp->program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);

	glBindVertexArray(VAOs[LightVAO]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}