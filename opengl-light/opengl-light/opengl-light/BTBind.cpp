#include "BTGL.h"

BTError *error = new BTError();

void BTBind::init(Shape shape){
	if (shape == this->TRIANGLE)
	{
		this->initTriangle();
	}
}

void BTBind::draw(Shape shape, BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, Camera *camera){
	if (shape == this->TRIANGLE)
	{
		this->drawTriangle(bt_shader,bt_shader_lamp,viewMatrix,camera);
	}
}

void BTBind::initTriangle(){
	//texcoord
	//漫反射贴图纹理图
	int width, height , channels;
	unsigned char* image = stbi_load("sImage/container2.png",&width,&height,&channels,0);
	int width_specular, height_specular, channels_specular;
	unsigned char* image_specular = stbi_load("sImage/container2_specular.png", &width_specular, &height_specular, &channels_specular, 0);
	int width_emission, height_emission, channels_emission;
	unsigned char* image_emission = stbi_load("sImage/matrix.jpg", &width_emission, &height_emission, &channels_emission, 0);

	glGenTextures(NumTEXIds, TEXs);
	glBindTexture(GL_TEXTURE_2D, TEXs[Container]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//镜面贴图纹理图
	glBindTexture(GL_TEXTURE_2D, TEXs[Container_Specular]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_specular, height_specular, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_specular);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_specular);
	glBindTexture(GL_TEXTURE_2D,0);

	//放射光贴图
	glBindTexture(GL_TEXTURE_2D,TEXs[Emission]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_emission, height_emission, 0, GL_RGB, GL_UNSIGNED_BYTE, image_emission);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_emission);
	glBindTexture(GL_TEXTURE_2D, 0);

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
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexs), triangleVertexs, GL_STATIC_DRAW);

	//发送顶点数组数据
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	//发送法线数据
	glVertexAttribPointer(vNormal , 3 , GL_FLOAT , GL_FALSE , sizeof(GLfloat) * 8 ,(const void*)(sizeof(GLfloat) * 3));
	error->glCheckError();
	//发送纹理数据
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (const void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glEnableVertexAttribArray(vTexCoords);
	glBindVertexArray(0);

	//光源
	glBindVertexArray(VAOs[LightVAO]);
	error->glCheckError();
	//check VAO bind status
	GLboolean vao_light_b = glIsVertexArray(VAOs[LightVAO]);
	printf("light vao bind status:%d\n", vao_light_b);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	error->glCheckError();
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);
}

void BTBind::drawTriangle(BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, Camera *camera){

	
	GLfloat currentTime = glfwGetTime();
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//投影矩阵
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	//光源位置
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 5.0f);

	//立方体
	bt_shader->Use();
	//model = glm::rotate(model, glm::radians(currentTime) * 100, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.6f * currentTime,1.4f,1.0f));
	//model = glm::scale(model, glm::vec3(currentTime*0.3f, currentTime, 1.0f));
	
	//视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//投影矩阵
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//观察向量
	glUniform3f(glGetUniformLocation(bt_shader->program, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);
	//纹理单元
	glUniform1i(glGetUniformLocation(bt_shader->program, "material.diffuse"), 5);
	glUniform1i(glGetUniformLocation(bt_shader->program, "material.specular"), 11);
	glUniform1i(glGetUniformLocation(bt_shader->program, "material.emission"), 16);
	glUniform1f(glGetUniformLocation(bt_shader->program, "material.shininess"), 32.0f);
	//光源结构体
	//点光源
	//glUniform3f(glGetUniformLocation(bt_shader->program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
	//定向光
	//glUniform3f(glGetUniformLocation(bt_shader->program,"light.direction"),-0.2f,-1.0f,-0.3f);
	//聚光
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.position"), camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.direction"), camera->Front.x, camera->Front.y, camera->Front.z);
	//不直接传角度 而是传cos的原因：节省开销 http://learnopengl-cn.readthedocs.io/zh/latest/02%20Lighting/05%20Light%20casters/
	glUniform1f(glGetUniformLocation(bt_shader->program,"light.cutoff"),glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(bt_shader->program, "light.othercutoff"), glm::cos(glm::radians(17.5f)));

	glUniform3f(glGetUniformLocation(bt_shader->program, "light.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "light.specular"), 3.0f, 3.0f, 3.0f);
	//衰减值
	glUniform1f(glGetUniformLocation(bt_shader->program,"light.constant"),1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "light.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "light.quadratic"), 0.032f);

	//材质结构体
	//纹理单元  激活 绑定 传值
	//漫反射贴图
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, TEXs[Container]);
	//镜面贴图
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D,TEXs[Container_Specular]);
	//放射光贴图
	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, TEXs[Emission]);

	glBindVertexArray(VAOs[TriangleVAO]);
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	//模型矩阵
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f *i;
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	

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

	glBindVertexArray(VAOs[LightVAO]);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
}