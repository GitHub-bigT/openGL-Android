# Question center

* 未使用 `glClear(GL_COLOR_BUFFER_BIT);`

<div>
  <img src="https://github.com/GitHub-bigT/openGL-Android/blob/master/images/question/question1.png" width="150px"/>
 <img src="https://github.com/GitHub-bigT/openGL-Android/blob/master/images/question/question2.png" width="150px"/>
</div>

* ig75icd32.dll
```
Unhandled exception at 0x0F399885 (ig75icd32.dll) in opengl-study.exe: 0xC0000005: Access violation reading location 0x00000000.
```

* 关于EBO的问题
1. bindxxx():

绑定的顺序为：VBO  EBO  VAO
```
	//1.绑定VBO
	glGenBuffers(NumVBOIds, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	//2.将数据传给显存当中的ARRAY_BUFFER缓冲块中
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertex)+sizeof(triangleColor),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangleVertex),triangleVertex);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(triangleVertex), sizeof(triangleColor), triangleColor);
	GLboolean b = glIsBuffer(VBOs[TriangleVBO]);
	printf("triangle vbo bind: %d\n", b);
	//(ig75icd32.dll) 崩溃
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//1.绑定EBO or IBO 
	glGenBuffers(NumEBOIds , EBOs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBOs[TriangleEBO]);
	GLboolean b2 = glIsBuffer(EBOs[TriangleEBO]);
	printf("triangle ebo bind: %d\n", b2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triangleIndex),triangleIndex,GL_STATIC_DRAW);
 
	//1.绑定VAO
	glGenVertexArrays(NumVaoIds, VAOs);
	glBindVertexArray(VAOs[TriangleVAO]);
	GLboolean b1 = glIsVertexArray(VAOs[TriangleVAO]);
	printf("triangle vao bind: %d\n", b1);
	//2.设置顶点属性指针
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(triangleVertex));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColor);
	//3.解绑vao,避免在其他地方错误的配置
	glBindVertexArray(0);
```
 draw():
 
 则draw的时候必须先bind VAO在bind EBO
```
	glBindVertexArray(VAOs[TriangleVAO]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[TriangleEBO]);
```
2.建议绑定的顺序为 VAO VBO EBO

以下摘抄自：https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/

原因：


