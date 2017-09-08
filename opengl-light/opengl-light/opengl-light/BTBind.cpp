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
	//��������ͼ����ͼ
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

	//������ͼ����ͼ
	glBindTexture(GL_TEXTURE_2D, TEXs[Container_Specular]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_specular, height_specular, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_specular);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_specular);
	glBindTexture(GL_TEXTURE_2D,0);

	//�������ͼ
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

	//���Ͷ�����������
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	//���ͷ�������
	glVertexAttribPointer(vNormal , 3 , GL_FLOAT , GL_FALSE , sizeof(GLfloat) * 8 ,(const void*)(sizeof(GLfloat) * 3));
	error->glCheckError();
	//������������
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (const void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glEnableVertexAttribArray(vTexCoords);
	glBindVertexArray(0);

	//��Դ
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

	//ͶӰ����
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	//��Դλ��
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 10.0f);

	//������
	bt_shader->Use();
	//model = glm::rotate(model, glm::radians(currentTime) * 100, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.6f * currentTime,1.4f,1.0f));
	//model = glm::scale(model, glm::vec3(currentTime*0.3f, currentTime, 1.0f));
	
	//��ͼ����
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//ͶӰ����
	glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//�۲�����
	glUniform3f(glGetUniformLocation(bt_shader->program, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);
	//����
	glUniform1i(glGetUniformLocation(bt_shader->program, "material.diffuse"), 5);
	glUniform1i(glGetUniformLocation(bt_shader->program, "material.specular"), 11);
	glUniform1i(glGetUniformLocation(bt_shader->program, "material.emission"), 16);
	glUniform1f(glGetUniformLocation(bt_shader->program, "material.shininess"), 32.0f);
	glm::vec3 pointLightLocation[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};
	//���Դ
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[0].position"), pointLightLocation[0].x, pointLightLocation[0].y, pointLightLocation[0].z);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[0].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[0].ambient"), 1.0f,0.0f,0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[0].diffuse"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	//���Դ2
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[1].position"), pointLightLocation[1].x, pointLightLocation[1].y, pointLightLocation[1].z);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[1].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[1].ambient"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[1].diffuse"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	//���Դ3
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[2].position"), pointLightLocation[2].x, pointLightLocation[2].y, pointLightLocation[2].z);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[2].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	//���Դ4
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[3].position"), pointLightLocation[3].x, pointLightLocation[3].y, pointLightLocation[3].z);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "pointLights[3].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[3].ambient"), 0.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[3].diffuse"), 0.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	//�����
	glUniform3f(glGetUniformLocation(bt_shader->program,"dirLight.direction"),100.0f,0.0f,0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
	//�۹�
	glUniform3f(glGetUniformLocation(bt_shader->program, "spotLight.position"), camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(glGetUniformLocation(bt_shader->program, "spotLight.direction"), camera->Front.x, camera->Front.y, camera->Front.z);
	glUniform3f(glGetUniformLocation(bt_shader->program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(bt_shader->program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(bt_shader->program, "spotLight.linear"), 0.09);
	glUniform1f(glGetUniformLocation(bt_shader->program, "spotLight.quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(bt_shader->program, "spotLight.cutOff"), glm::cos(glm::radians(5.5f)));
	glUniform1f(glGetUniformLocation(bt_shader->program, "spotLight.outerCutOff"), glm::cos(glm::radians(7.5f)));

	//���ʽṹ��
	//����Ԫ  ���� �� ��ֵ
	//��������ͼ
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, TEXs[Container]);
	//������ͼ
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D,TEXs[Container_Specular]);
	//�������ͼ
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
	//ģ�;���
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f *i;
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(bt_shader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	

	//��Դ
	bt_shader_lamp->Use();
	//��ͼ����
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//ͶӰ����
	glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAOs[LightVAO]);

	//ģ�;���
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, pointLightLocation[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(bt_shader_lamp->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glDrawArrays(GL_TRIANGLES, 0, 36);
}