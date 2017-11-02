# fbo

http://blog.csdn.net/pizi0475/article/details/7932960

两种类型的帧缓冲对象：纹理图像（渲染到纹理）、渲染缓存图像（离线渲染）

一个帧缓冲对象中有多个颜色关联点和一个深度关联点和一个模板关联点 注：FBO没有任何图像存储区，只有多个关联点

glReadBuffer http://www.cnblogs.com/yxnchinahlj/archive/2010/11/19/1881781.html

```
// bind the FBO (for both, READ_FRAMEBUFFER_EXT and DRAW_FRAMEBUFFER_EXT)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

			// Attach the Input texture to the color buffer in our frame buffer - note texturetarget 
			glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, TextureTarget, TextureID, 0);
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			// Attach target texture (the shared texture we write into) to second attachment point
			glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, m_glTexture, 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
//Invert
glBlitFramebufferEXT(0, 0,			// srcX0, srcY0, 
											 width, height, // srcX1, srcY1
											 0, height,		// dstX0, dstY0,
											 width, 0,		// dstX1, dstY1,
											 GL_COLOR_BUFFER_BIT, GL_NEAREST); // GLbitfield mask, GLenum filter
else
  glBlitFramebufferEXT(0, 0,			// srcX0, srcY0, 
											 width, height,	// srcX1, srcY1
											 0, 0,			// dstX0, dstY0,
											 width, height,	// dstX1, dstY1,
											 GL_COLOR_BUFFER_BIT, GL_NEAREST); // GLbitfield mask, GLenum filter
```
