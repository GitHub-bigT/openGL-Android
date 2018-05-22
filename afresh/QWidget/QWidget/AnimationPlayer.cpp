#include "AnimationPlayer.h"

#define STRINGIZE(s)           #s

static    PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),    //上述格式描述符的大小
	1,                                //版本号
	PFD_DRAW_TO_WINDOW |              //格式支持窗口
	PFD_SUPPORT_OPENGL |              //格式必须支持OpenGL
	PFD_DOUBLEBUFFER,                 //必须支持双缓冲
	PFD_TYPE_RGBA,                    //申请RGBA 格式
	24,                               //选定色彩深度
	0, 0, 0, 0, 0, 0, 0, 0,           //忽略RGBA
	0,                                //无累加缓存
	0, 0, 0, 0,                       //忽略聚集位
	32,                               //32位Z-缓存(深度缓存)
	0,                                //无蒙板缓存
	0,                                //无辅助缓存
	PFD_MAIN_PLANE,                   //主绘图层
	0,                                //Reserved
	0, 0, 0                           //忽略层遮罩
};

static GLfloat vertices[] =
{
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

static GLuint indices[] =
{
	0, 1, 2, 0, 2, 3
};

static const char vertexShader[] = "#" STRINGIZE(
version %d\n
attribute vec3 iv_Pos;\n
attribute vec2 iv_TexCoord; \n
varying vec2 ov_TexCoord; \n
void main()\n
{\n
gl_Position = vec4(iv_Pos.x, iv_Pos.y, iv_Pos.z, 1.0f); \n
ov_TexCoord = vec2(iv_TexCoord.x, 1 - iv_TexCoord.y); \n
}\n
);

static const char fragmentShader[] = "#" STRINGIZE(
version %d\n
varying vec2 ov_TexCoord; \n
uniform sampler2D s_texture; \n
void main()\n
{\n
	gl_FragColor = texture(s_texture, ov_TexCoord); \n
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n
}\n
);

AnimationPlayer::AnimationPlayer(QWidget *w, HWND h, QVector<QImage> &v, int fr)
{
	this->isRun = true;
	this->mWinDC = NULL;
	this->mWinRC = NULL;
	this->mVaoId = 0;
	this->mVboId = 0;
	this->mEboId = 0;
	this->mProgramId = 0;
	this->mGLSLVersion = 0;
	this->mHWND = h;
	this->mWidget = w;
	this->mImageArray = v;
	this->mFrameRate = fr;
	this->isOpenDebugInfo = false;
	this->isOpenShaderDebugInfo = false;
}

AnimationPlayer::AnimationPlayer()
{
	this->isRun = true;
	this->mWinDC = NULL;
	this->mWinRC = NULL;
	this->mVaoId = 0;
	this->mVboId = 0;
	this->mEboId = 0;
	this->mProgramId = 0;
	this->mGLSLVersion = 0;
	this->isOpenDebugInfo = false;
	this->isOpenShaderDebugInfo = false;
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::setWidget(QWidget *w)
{
	this->mWidget = w;
}

void AnimationPlayer::setHWND(HWND &h)
{
	this->mHWND = h;
}

void AnimationPlayer::setFrameRate(int fr)
{
	this->mFrameRate = fr;
}

void AnimationPlayer::setImageArray(QVector<QImage> &v)
{
	this->mImageArray = v;
}

void AnimationPlayer::setOpenDebugInfo(bool b)
{
	this->isOpenDebugInfo = b;
}

void AnimationPlayer::setOpenShaderDebugInfo(bool b)
{
	this->isOpenShaderDebugInfo = b;
}

int AnimationPlayer::getGLSLVersion()
{
	char *glsl_version_str = strdup((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Skip past the first period.
	char *ptr = strchr(glsl_version_str, '.');
	assert(ptr != NULL);
	++ptr;

	// Now cut the string off at the next period or space, whatever comes first
	// (unless the string ends first).
	while (*ptr && *ptr != '.' && *ptr != ' ') {
		++ptr;
	}
	*ptr = '\0';

	// Now we have something on the form X.YY. We convert it to a float, and hope
	// that if it's inexact (e.g. 1.30), atof() will round the same way the
	// compiler will.
	float glsl_version = atof(glsl_version_str);
	free(glsl_version_str);

	int version = 0;
	if (glsl_version > 4.45)
	{
		version = 450;
	}
	else if (glsl_version > 4.35)
	{
		version = 440;
	}
	else if (glsl_version > 4.25)
	{
		version = 430;
	}
	else if (glsl_version > 4.15)
	{
		version = 420;
	}
	else if (glsl_version > 4.05)
	{
		version = 410;
	}
	else if (glsl_version > 3.95)
	{
		version = 400;
	}
	else if (glsl_version > 3.25)
	{
		version = 330;
	}
	else if (glsl_version > 1.45)
	{
		version = 150;
	}
	else if (glsl_version > 1.35)
	{
		version = 140;
	}
	else if (glsl_version > 1.25)
	{
		version = 130;
	}
	else if (glsl_version > 1.15)
	{
		version = 120;
	}
	else
	{
		if (isOpenDebugInfo)
			printf("GLSL version %.03f too low!\n", glsl_version);
	}

	return version;
}

void AnimationPlayer::stop()
{
	isRun = false;
	wait(3000);
}

int AnimationPlayer::setupPixelFormat(HDC hDC)
{
	int pixelIdx;
	// get the appropriate pixel format 
	pixelIdx = ChoosePixelFormat(hDC, &pfd);
	if (pixelIdx == 0) {
		printf("ChoosePixelFormat() failed:  Cannot find format specified.");
		return 0;
	}

	/* set the pixel format */
	if (SetPixelFormat(hDC, pixelIdx, &pfd) == FALSE) {
		printf("SetPixelFormat() failed:  Cannot set format specified.");
		return 0;
	}

	return pixelIdx;
}

void AnimationPlayer::printGLInfo() {
	printf("InitGLEW SUCCESS\n");
	printf("\nOpenGL vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("OpenGL GLSL version: %d\n", mGLSLVersion);
}

void AnimationPlayer::timer_start(FILETIME *start)
{
	GetSystemTimeAsFileTime(start);
}

int64_t AnimationPlayer::timer_elapsed_msec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10000;
}

int64_t AnimationPlayer::timer_elapsed_usec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10;
}

GLuint AnimationPlayer::initShader(const char* shaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	if (isOpenShaderDebugInfo)
	{
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("COMPILE SHADER ERRPR,%s\n", std::string(infoLog).c_str());
		}
		else
		{
			printf("COMPILE SHADER SUCCESS\n");
		}
	}
	return shader;
}

GLuint AnimationPlayer::initShaderProgram()
{
	char *vs, *fs;
	vs = (char*)malloc(sizeof(vertexShader) + 32);
	sprintf(vs, vertexShader, mGLSLVersion);
	fs = (char*)malloc(sizeof(fragmentShader) + 32);
	sprintf(fs, fragmentShader, mGLSLVersion);
	GLuint v = initShader(vs, GL_VERTEX_SHADER);
	GLuint f = initShader(fs, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, v);
	glAttachShader(program, f);
	glLinkProgram(program);
	free(vs);
	free(fs);
	glDeleteShader(v);
	glDeleteShader(f);
	if (isOpenShaderDebugInfo)
	{
		int  success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			printf("LINK PROGRAM ERROR %s\n", std::string(infoLog).c_str());
		}
		else
		{
			printf("LINK PROGRAM SUCCESS\n");
		}
	}

	return program;
}

bool AnimationPlayer::initGLEW()
{
	bool ret = true;
	mGLSLVersion = getGLSLVersion();
	GLenum err = glewInit();
	if (isOpenDebugInfo && err != GLEW_OK)
		printf("GLEW Error: %s\n", glewGetErrorString(err));
	if (isOpenDebugInfo)
		printGLInfo();
	return ret;
}

void AnimationPlayer::initGL()
{
	mWinDC = GetDC(mHWND);
	setupPixelFormat(mWinDC);
	mWinRC = wglCreateContext(mWinDC);
	wglMakeCurrent(mWinDC, mWinRC);
	bool ret = initGLEW();
	mProgramId = initShaderProgram();

	glGenVertexArrays(1, &mVaoId);
	glBindVertexArray(mVaoId);

	glGenBuffers(1, &mEboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &mVboId);
	glBindBuffer(GL_ARRAY_BUFFER, mVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	int vLoc = glGetAttribLocation(mProgramId, "iv_Pos");
	glVertexAttribPointer(vLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);
	glEnableVertexAttribArray(vLoc);
	int tLoc = glGetAttribLocation(mProgramId, "iv_TexCoord");
	glVertexAttribPointer(tLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(tLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AnimationPlayer::initImageTextureArray()
{
	for (int i = 0; i < mImageArray.size(); i++)
	{
		QImage image = mImageArray.at(i);
		QImage::Format f = image.format();
		QImage temp = image.convertToFormat(QImage::Format_RGBA8888);
		
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp.width(), temp.height(),
 			0, GL_RGBA, GL_UNSIGNED_BYTE, temp.constBits());

		glBindTexture(GL_TEXTURE_2D, 0);
		mImageTextureArray.push_back(texture);
	}
}

void AnimationPlayer::run()
{
	initGL();
	initImageTextureArray();
	glUseProgram(mProgramId);
	float mBgColor = 0.0f;
	int textureIndex = 0;
	if (mFrameRate == 0)
		mFrameRate = 60;
	while (isRun)
	{
		LPRECT rect = (LPRECT)malloc(sizeof(RECT));
		GetWindowRect(mHWND, rect);
		glViewport(0, 0, rect->right - rect->left, rect->top - rect->bottom);
		free(rect);
		FILETIME renderTimer;
		timer_start(&renderTimer);
		if (mBgColor < 0.5)
			mBgColor = 1.0f;
		else
			mBgColor = 0.0f;
		glClearColor(mBgColor, mBgColor, mBgColor, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(mVaoId);
		GLuint texId = mImageTextureArray.at(textureIndex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glFinish();
		int renderTime = timer_elapsed_msec(&renderTimer);
		int frameTime = 1000 / mFrameRate;
		int sleepTime = frameTime - renderTime;
		if (sleepTime < 0)
			sleepTime = 0;
		Sleep(sleepTime);
		//printf("renderTime = %d, frameTime = %d, sleep time = %d\n", renderTime, frameTime, frameTime - renderTime);
		SwapBuffers(mWinDC);
		textureIndex++;
		if (textureIndex > mImageTextureArray.size() - 1)
			textureIndex = 0;
	}
	releaseGL();
}

void AnimationPlayer::releaseGL()
{
	glDeleteBuffers(1, &mVaoId);
	glDeleteBuffers(1, &mVboId);
	glDeleteBuffers(1, &mEboId);
	glDeleteProgram(mProgramId);
	for (int i = 0; i < mImageTextureArray.size(); i++)
	{
		GLuint t = mImageTextureArray.at(i);
		glDeleteTextures(1, &t);
	}
	wglMakeCurrent(mWinDC, NULL);
}