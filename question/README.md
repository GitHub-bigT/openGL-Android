# Question center

* 未使用 `glClear(GL_COLOR_BUFFER_BIT);`

<div>
  <img src="https://github.com/GitHub-bigT/openGL-study/blob/master/question/images/question1.png" width="150px"/>
 <img src="https://github.com/GitHub-bigT/openGL-study/blob/master/question/images/question2.png" width="150px"/>
</div>

* -----ig75icd32.dll

win7 vs2013

```
Unhandled exception at 0x0F399885 (ig75icd32.dll) in opengl-study.exe: 0xC0000005: Access violation reading location 0x00000000.
```

错误原因：仔细检查代码  233333

* 关于EBO的问题  drawElements();
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

draw()的时候就只可以bind VAO

以下摘抄自：https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/

原因：

`glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引。`

`这意味着我们必须在每次要用索引渲染一个物体时绑定相应的EBO，这还是有点麻烦。`

`不过顶点数组对象同样可以保存索引缓冲对象的绑定状态。`

`VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象。`

`绑定VAO的同时也会自动绑定EBO。`

* glTexImage2D 指定二维纹理图像时崩溃

```
Unhandled exception at 0x779497AD (ig75icd32.dll) in opengl-study.exe: 0xC0000005: Access violation reading location 0x0619D000.
```

解决办法：

https://stackoverflow.com/questions/7380773/glteximage2d-segfault-related-to-width-height
```
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
glTexImage2D前设置
```

2. awesomeface.png 有黑背景

* 正射投影的使用

* stb_image的使用

下载这一个头文件，将它以stb_image.h的名字加入你的工程，并另创建一个新的C++文件，输入以下代码：

```
int width, height, nrChannels;
unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
```

通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个

.cpp 文件了。现在只需要在你的程序中包含stb_image.h并编译就可以了。

摘抄自：https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/

