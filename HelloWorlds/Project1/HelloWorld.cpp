#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <GL\glut.h>
#include "objModel.h"
#define BITMAP_ID 0x4D42
#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 720
const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;
float G_fDistance = 1.5f;
//物体的旋转角度 
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

//设定读入的模型以及显示的方式
bool redraw = true;
ObjModel *obj1 = new ObjModel();
string path = "obj/rubby.obj";
//light0参数
GLfloat Vp0[] = { 0.0,0.0,0.0,1.0 };	 //光源环境光位置
GLfloat Va0[] = { 0.8,0.8,0.8,1 };       //光源环境光强度数组  
GLfloat Vd0[] = { 0.6,0.6,0.6,1 };       //光源散射光强度数组  
GLfloat Vs0[] = { 0.5,0.5,0.5,1 };       //光源镜面反射光强度数组  
GLuint texture[6];
////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);
void skybox();
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
void texload(int i, const char *filename);
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	//设定OPENGL窗口位置和大小
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL");
	myinit();
	glutReshapeFunc(myReshape);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

void myinit(void)
{
	glGenTextures(6, texture); // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	texload(0, "textures/back.bmp");
	texload(1, "textures/front.bmp");
	texload(2, "textures/right.bmp");
	texload(3, "textures/left.bmp");
	texload(4, "textures/top.bmp");
	texload(5, "textures/bottom.bmp");
	//your initialization code
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void myReshape(GLsizei w, GLsizei h){
	//设定视区
	glViewport(0, 0, w, h);

	//设定透视方式
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, 1.0*(GLfloat)w / (GLfloat)h, 0.1, 3000.0);

}


void display(void){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox();
	//设置成模型矩阵模式
	glMatrixMode(GL_MODELVIEW);

	//载入单位化矩阵
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, Vp0);		//设置光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, Va0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Vd0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Vs0);
	glTranslatef(0.0, 0.0, -G_fDistance);

	if (redraw) {		//只读一遍 
		obj1->readobj(path);
		redraw = false;
	}
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);
	
	obj1->showobj();
	//glLoadIdentity();


	//交换前后缓冲区
	glutSwapBuffers();
}

void skybox() {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	float skyHei = 100;                      //天空的高度
	float skyLen = 100;                   //天空的长度
	float skyWid = 100;
	//后
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 0);
	glVertex3f(skyLen, -skyHei, skyWid);
	glTexCoord2f(1, 1);
	glVertex3f(skyLen, +skyHei, skyWid);
	glTexCoord2f(0, 1);
	glVertex3f(-skyLen, +skyHei, skyWid);
	glTexCoord2f(0, 0);
	glVertex3f(-skyLen, -skyHei, skyWid);
	glEnd();
	//前
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 0);
	glVertex3f(-skyLen, -skyHei, -skyWid);
	glTexCoord2f(1, 1);
	glVertex3f(-skyLen, skyHei, -skyWid);
	glTexCoord2f(0, 1);
	glVertex3f(skyLen, skyHei, -skyWid);
	glTexCoord2f(0, 0);
	glVertex3f(skyLen, -skyHei, -skyWid);
	glEnd();
	//右面
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 0);
	glVertex3f(skyLen, -skyHei, -skyWid);
	glTexCoord2f(1, 1);
	glVertex3f(skyLen, skyHei, -skyWid);
	glTexCoord2f(0, 1);
	glVertex3f(skyLen, skyHei, skyWid);
	glTexCoord2f(0, 0);
	glVertex3f(skyLen, -skyHei, skyWid);
	glEnd();

	
	//左面
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(1, 0);
	glVertex3f(-skyLen, -skyHei, skyWid);
	glTexCoord2f(1, 1);
	glVertex3f(-skyLen, +skyHei, skyWid);
	glTexCoord2f(0, 1);
	glVertex3f(-skyLen, +skyHei, -skyWid);
	glTexCoord2f(0, 0);
	glVertex3f(-skyLen, -skyHei, -skyWid);
	glEnd();

    //顶面
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0, 1);
	glVertex3f(-skyLen, skyHei, -skyWid);
	glTexCoord2f(0, 0);
	glVertex3f(-skyLen, skyHei, skyWid);
	glTexCoord2f(1, 0);
	glVertex3f(skyLen, skyHei, skyWid);
	glTexCoord2f(1, 1);
	glVertex3f(skyLen, skyHei, -skyWid);
	glEnd();

	//底面
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0, 1);
	glVertex3f(-skyLen, -skyHei, skyWid);
	glTexCoord2f(0, 0);
	glVertex3f(-skyLen, -skyHei, -skyWid);
	glTexCoord2f(1, 0);
	glVertex3f(skyLen, -skyHei, -skyWid);
	glTexCoord2f(1, 1);
	glVertex3f(skyLen, -skyHei, skyWid);
	glEnd();
	glDisable(GL_TEXTURE_2D);//关闭纹理
	
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		G_fAngle_horizon -= 10.0f;
		break;
	case GLUT_KEY_RIGHT:
		G_fAngle_horizon += 10.0f;
		break;
	case GLUT_KEY_UP:
		G_fAngle_vertical -= 10.0f;
		break;
	case GLUT_KEY_DOWN:
		G_fAngle_vertical += 10.0f;
		break;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 97:	//"a"
		G_fDistance -= 2.0f;
		break;
	case 98:		//"b"
		G_fDistance += 2.0f;
		break;
	case 27:	//"esc"
		exit(0);
	case '1':
		path = "obj/rubby.obj";
		redraw = true; obj1->~ObjModel(); obj1 = new ObjModel(); break;
	case '2':
		path = "obj/bird.obj";
		redraw = true; obj1->~ObjModel(); obj1 = new ObjModel(); break;
	case '3':
		path = "obj/torus.obj";
		redraw = true; obj1->~ObjModel(); obj1 = new ObjModel(); break;
	case '4':
		path = "obj/wan.obj";
		redraw = true; obj1->~ObjModel(); obj1 = new ObjModel(); break;
	}
	glutPostRedisplay();
}

void texload(int i,const char *filename){
	BITMAPINFOHEADER bitmapInfoHeader;// bitmap信息头
	unsigned char*bitmapData;// 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	
	glTexImage2D(GL_TEXTURE_2D,
		0, //mipmap层次(通常为，表示最上层) 
		GL_RGB,//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);//bitmap数据指针  
					// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S方向重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T方向重复
}

unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;// bitmap文件头
	unsigned char*bitmapImage;// bitmap图像数据
	int imageIdx = 0;// 图像位置索引
	unsigned char tempRGB;// 交换变量

								 // 以“二进制+读”模式打开文件filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		printf("%s", filename);
		return NULL;
	}
	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}
