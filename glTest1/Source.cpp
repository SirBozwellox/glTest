#include "main.h"

int mousex = 0;
int mousey = 0;

bool leftclick;
bool leftclick_flag;
bool rightclick;
bool rightclick_flag;

int fps = 60;
float fpsMult = 60.0f / (float)fps;
int framerate = 1000 / fps;

int windowx = 640;
int windowy = 480;
float screenmultx = 1.0;
float screenmulty = 1.0;

//char homeDirectory[100] = "C:/Users/Admin/Documents/Visual Studio 2015/Sigma/assets/textures";
//char homeDirectory[100] = "../assets/textures";
//char homeDirectory[100] = "assets/textures";

//HWND windowHandle = NULL;

GLuint tex1;

bool checkActiveWindow() {
	return TRUE;
}

void draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	enable2d((int)windowx, (int)windowy);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// Draw background colour
	glColor4f(0.0f, 0.45f, 1.0f, 1.0f);
	placeimageeasy(NULL, 0, 0, renderx, rendery, FALSE);


	// Draw scene
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	placeimageeasy(tex1, 32, 32, 128, 128, FALSE);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glutSwapBuffers();
}

void update(int value)
{
	glutTimerFunc(framerate, update, 0);

	//	printf("UPDATE");

	POINT mousepos;
	GetCursorPos(&mousepos);
	mousepos.x -= glutGet((GLenum)GLUT_WINDOW_X);
	mousepos.y -= glutGet((GLenum)GLUT_WINDOW_Y);
	mousepos.x -= screenshiftx;
	mousepos.y -= screenshifty;
//	mousex = mousepos.x / screenmultx;
//	mousey = mousepos.y / screenmulty;

	//	printf("%i, %i\n", mousex, mousey);

	leftclick = FALSE;
	if ((GetKeyState(VK_LBUTTON) & 0x100) == 0)
	{
		leftclick_flag = FALSE;
	}
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0 && leftclick_flag == FALSE)
	{
		leftclick_flag = TRUE;
		leftclick = TRUE;
	}

	rightclick = FALSE;
	if ((GetKeyState(VK_RBUTTON) & 0x100) == 0)
	{
		rightclick_flag = FALSE;
	}
	if ((GetKeyState(VK_RBUTTON) & 0x100) != 0 && rightclick_flag == FALSE)
	{
		rightclick_flag = TRUE;
		rightclick = TRUE;
	}

	if (!checkActiveWindow())
	{
		rightclick = FALSE;
		leftclick = FALSE;
	}
	else {
	}

	draw();

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(windowx, windowy);
	glutCreateWindow("The Ultra Code");
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutDisplayFunc(draw);
	glutTimerFunc(framerate, update, 0);
	//	glutReshapeFunc(resizeWindow);

//	windowHandle = FindWindow(NULL, "The Ultra Code");

	tex1 = LoadTexture("textures", "tex1.png");

	//	initInterface();

	glutMainLoop();

	exit(0);

	return 0;
}