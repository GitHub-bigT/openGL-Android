# pbo

## unpack:ram->pbo

## pacl:pbo->ram

获取图像数据的时间 图像到pbo的时间 消费pbo的时间

readPointer writePointer

保证获取最新帧r = w -1;，抖动（渲染完新的之后渲染旧的造成抖动）以及防止pbo同时读写 r != w  

```
glBindBuffer(GL_PIXEL_UNPACK_BUFFER, self->pbos[i]->pbo);
(uint8_t *) pbo_addr = (uint8_t *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
```

注：衹需一次
