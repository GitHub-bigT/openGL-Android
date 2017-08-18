# Question center

* 未使用 `glClear(GL_COLOR_BUFFER_BIT);`

<div>
  <img src="https://github.com/GitHub-bigT/openGL-Android/blob/master/images/question/question1.png" width="150px"/>
 <img src="https://github.com/GitHub-bigT/openGL-Android/blob/master/images/question/question2.png" width="150px"/>
</div>

* mat可以通过glVertexAttribPointer传给vertex shader吗？
```
glm::mat4 model;    
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
glBindBuffer(GL_ARRAY_BUFFER,Buffers[MatBuffer]);
glBufferData(GL_ARRAY_BUFFER,sizeof(model),&model,GL_DYNAMIC_DRAW);
glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(model),0);
```
glBufferData的第三个参数和glVertexAttribPointer的第二个参数该怎么写？
```
"layout(location=3) in mat4 mattest;\n"
```


