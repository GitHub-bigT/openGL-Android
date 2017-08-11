#include "main.h"

Tools *tools;
//GLProcess *process;
GLuint program;
	//VAO
	enum{VAOBall,NumVAOs};
	GLuint VAOs[NumVAOs];
	//VBO
	enum{VBOBall,NumVBOs};
	GLuint VBOs[NumVBOs];
	
	int w , h;
	
const char* fragmentShaderSource = "#version 300 es\n"
								"out vec4 fColor;\n"
								"void main() {\n"
								"fColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
								"}\n";

const char* vertexShaderSource = "#version 300 es\n"
									"layout(location=1) in vec3 vPosition;\n"
									"void main() {\n" 
									"gl_Position = vec4(vPosition,1.0f);\n"
									"}\n";
									//顶点数据
	float vertexData[129600][3];

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_init(JNIEnv* , jclass , int width, int height){
	w = width;
	h = height;
}

void  generateBallVertexCoord(int width , int height){
	LOGE("generateBallVertexCoord ::: %d,%d",width,height);
	/**
	* 球的坐标
	* x = R*cos(a)*sin(b)
	* y = R*sin(a)
	* z = R*cos(a)*cos(b)
	*/
	//宽高比例
	float whProportion = width/height;
	LOGE("generateBallVertexCoord ::: %d,%d",width,height);
	LOGE("whProportion ::: %f",whProportion);
	LOGE("width/height fffffffff::: %f",width/height);
	LOGE("width/height ddddddddd::: %d",width/height);
	//球的半径
	//float radius[3] = {1.0f , 1.0f * whProportion, 1.0f};
	//球坐标
	float x1,x2,y1,y2,z1,z2;
	//x z轴上的斜边(R*cos(a))
	GLfloat hypotenuse1 , hypotenuse2;
	//x y轴夹角的变换角度大小。值越小越像趋于球体
	int step = 1;
	/**画球的思想
	* 纬度方向 -90度 - 90度旋转出一个扇形
	* 经度方向旋转360度画出球
	* 这里采用的办法是每两条相邻的纬度画出一个体
	*/
	int n = 0 ;
	for(int angleA = -90 ; angleA<90 ; angleA+=step){	
		//纬度方向
		//x z轴上的斜边(R*cos(a))
		hypotenuse1 = cos(M_PI * angleA / 180.0f);
		hypotenuse2 = cos(M_PI * (angleA + step) / 180.0f);
		y1 = 1080.0f/1704.0f * sin(M_PI * angleA / 180.0f);
		y2 = 1080.0f/1704.0f * sin(M_PI * (angleA + step) / 180.0f);
		for(int angleB = 0 ; angleB<360 ; angleB+=step){
			//经度方向
			x1 = hypotenuse1 * sin(M_PI * angleB / 180.0f);
			x2 = hypotenuse2 * sin(M_PI * angleB / 180.0f);
			z1 = hypotenuse1 * cos(M_PI * angleB / 180.0f);
			z2 = hypotenuse2 * cos(M_PI * angleB / 180.0f);
			//LOGI("index:%d,latitude:%d,longitude:%d ,ball coord:x1,y1,z1: %f,%f,%f" , n,angleA,angleB,x1,y1,z1);
			//LOGI("index:%d,latitude:%d,longitude:%d ,ball coord:x2,y2,z2: %f,%f,%f" , n,angleA+step,angleB+step,x2,y2,z2);	
			vertexData[n][0] = x1;
			vertexData[n][1] = y1;
			vertexData[n][2] = z1;
			vertexData[n+1][0] = x2;
			vertexData[n+1][1] = y2;
			vertexData[n+1][2] = z2;
			n+=2;
		}
	}
	for(int i = 0 ; i < sizeof(vertexData)/(sizeof(float)*3);i++) {
		//LOGE("index %d , ball coord:x,y,z: %f,%f,%f",i,vertexData[i][0],vertexData[i][1],vertexData[i][2]);
	}
}

GLuint createProgram(){
	program = glCreateProgram();
	GLuint vertexShader = loadShder(GL_VERTEX_SHADER,vertexShaderSource);
	GLuint fragmentShader = loadShder(GL_FRAGMENT_SHADER,fragmentShaderSource);
	
	//LOGI("vertexShader: %d", vertexShader);
	//LOGI("fragmentShader: %d", fragmentShader);
	
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	glLinkProgram(program);
	
	
	GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		
		LOGE("program ---- linkStatus: %d", linkStatus);
		
       if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			
			LOGE("program ---- 得到logo日志的长度: %d", bufLength);
			
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
				
				LOGE("program ---- buf: %s", buf);
				
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
	return program;
}

GLuint loadShder(GLenum type , const char* source){
	GLuint shader = glCreateShader(type);
	glShaderSource(shader,1,&source,NULL);
	glCompileShader(shader);
	
	//LOGE("shader ---- : %d", shader);
	
	//check
	GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	
	LOGE("shader ---- shader编译status: %d",compiled);
	
    if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		
		LOGE("shader ---- 得到logo日志的长度: %d", infoLen);
		
		if (infoLen) {
			char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("shader ---- Could not compile shader %d:\n%s\n" , type, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
	return shader;
}

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_step(JNIEnv* , jclass){
	//LOGI("width: %d , height: %d",width,height);
	tools = new Tools();
	tools->printGLString("Version", GL_VERSION); 
    //tools->printGLString("Vendor", GL_VENDOR);
    //tools->printGLString("Renderer", GL_RENDERER);
    //tools->printGLString("Extensions", GL_EXTENSIONS);
    //tools->printGLString("Shader language version", GL_SHADING_LANGUAGE_VERSION);
	//process = new GLProcess();
	//process->init(width,height);
	//GLShader * glshader;
	//GLProgram * glprogram;
	//glshader = new GLShader();
	//glprogram = new GLProgram();
	

	
	//generate with bind VAO VBO
	glGenVertexArrays(NumVAOs,VAOs);
	glGenBuffers(NumVBOs,VBOs);
	glBindVertexArray(VAOs[VAOBall]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[VBOBall]);
	
	//check bind
	tools->checkIsBindVertexArray(VAOs[VAOBall]);
	tools->checkIsBindBuffer(VBOs[VBOBall]);
	
	//generate ball vertex coord
	generateBallVertexCoord(w,h);
	for(int i = 0 ; i < sizeof(vertexData)/(sizeof(float)*3);i++) {
		LOGE("index %d , ball coord:x,y,z: %f,%f,%f",i,vertexData[i][0],vertexData[i][1],vertexData[i][2]);
	}
	
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	//顶点着色器attr索引
	enum{vPosition=1};
	//transmission data
	glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,0);
	//enable
	glEnableVertexAttribArray(vPosition);
	
	//
	//create program
	program = createProgram();
	glUseProgram(program);
	//LOGI("here is step");
	//process->step();
	glClear(GL_COLOR_BUFFER_BIT);
	//LOGI("current %d",current);
	//glBindVertexArray(VAOs[VAOBall]);
	glDrawArrays(GL_LINE_STRIP,0,129600);
	glFlush();
}