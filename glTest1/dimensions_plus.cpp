#include "main.h"

using namespace std;

const float pi = 3.1415926535;

int carriage_return = 0;

float screenshiftx = 0.0;
float screenshifty = 0.0;

float aspectRatio = 16.0f / 9.0f;

float renderx = 1280.0f;
float rendery = 720.0f;

float fov = 1000.0;
float zorigin = 250.0;

int mousex_dir = 0;
int mousey_dir = 0;

int charsize = 8 * 1;
char textlist[200][3] = { "A",   "B",   "C",   "D",   "E",   "F",   "G",   "H",   "I",   "J",   "K",    "L",    "M",    "N",    "O",   "P",   "Q",   "R",   "S",   "T",   "U",   "V",   "W",   "X",   "Y",    "Z",    "a",   "b",   "c",   "d",   "e",   "f",   "g",   "h",   "i",   "j",   "k",    "l",    "m",    "n",    "o",   "p",   "q",   "r",   "s",   "t",   "u",   "v",   "w",   "x",   "y",    "z",    "1",   "2",   "3",   "4",   "5",   "6",   "7",   "8",   "9",   "0",   "%%",  "^",   "&",   "*",   "(",   ")",   "_",   "+",   "-",   "'",   "~",    "<",   ">",   ",",   ".",   "?",   "/",   "\\",  "|",   "!",   "{",   "}",   ";",   ":",   "@",   "#",   "$",   "[",   "]",   "=", " " };
float font_loc[200][2] = { { 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 },{ 4, 0 },{ 5, 0 },{ 6, 0 },{ 7, 0 },{ 8, 0 },{ 9, 0 },{ 10, 0 },{ 11, 0 },{ 12, 0 },{ 13, 0 },{ 0, 1 },{ 1, 1 },{ 2, 1 },{ 3, 1 },{ 4, 1 },{ 5, 1 },{ 6, 1 },{ 7, 1 },{ 8, 1 },{ 9, 1 },{ 10, 1 },{ 11, 1 },{ 0, 2 },{ 1, 2 },{ 2, 2 },{ 3, 2 },{ 4, 2 },{ 5, 2 },{ 6, 2 },{ 7, 2 },{ 8, 2 },{ 9, 2 },{ 10, 2 },{ 11, 2 },{ 12, 2 },{ 13, 2 },{ 0, 3 },{ 1, 3 },{ 2, 3 },{ 3, 3 },{ 4, 3 },{ 5, 3 },{ 6, 3 },{ 7, 3 },{ 8, 3 },{ 9, 3 },{ 10, 3 },{ 11, 3 },{ 0, 4 },{ 1, 4 },{ 2, 4 },{ 3, 4 },{ 4, 4 },{ 5, 4 },{ 6, 4 },{ 7, 4 },{ 8, 4 },{ 9, 4 },{ 0, 5 },{ 1, 5 },{ 2, 5 },{ 3, 5 },{ 4, 5 },{ 5, 5 },{ 6, 5 },{ 7, 5 },{ 8, 5 },{ 9, 5 },{ 10, 5 },{ 0, 6 },{ 1, 6 },{ 2, 6 },{ 3, 6 },{ 4, 6 },{ 5, 6 },{ 6, 6 },{ 7, 6 },{ 9, 6 },{ 0, 7 },{ 1, 7 },{ 2, 7 },{ 3, 7 },{ 5, 7 },{ 6, 7 },{ 7, 7 },{ 8, 7 },{ 9, 7 },{ 10, 7 },{ 11, 6 } };
float font_max_x = charsize * 14;
float font_max_y = charsize * 8;

// LOAD A TEXTURE USING SOIL && GENERATE RELATIVE GL TEXTURE

GLuint font1;
GLuint font2;

GLuint texture;
int LoadTexture(char* directory, char* filename)
{

	char file[200] = "";
	strcat_s(file, sizeof(file), directory);
	strcat_s(file, sizeof(file), "/");
	strcat_s(file, sizeof(file), filename);
	texture = SOIL_load_OGL_texture(
		file,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		0
	);
	if (0 == texture)
	{
		printf("SOIL loading error: '%s' for file %s\n", SOIL_last_result(), file);
		return 0;
	}
	else
	{
		//		printf("Texture load successful \n");
		return texture;
	}
}

// ENABLE 2D RENDERING

void enable2d(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void calculateScreenShift() {
	float tempRatio = (float)windowx / (float)windowy;
	if (tempRatio < aspectRatio) {
		screenshiftx = 0.0f;
		screenmultx = windowx / renderx;
		screenmulty = screenmultx;
		screenshifty = (windowy - (rendery * screenmulty)) / 2.0f;
	}
	else if (tempRatio > aspectRatio) {
		screenshifty = 0.0f;
		screenmulty = windowy / rendery;
		screenmultx = screenmulty;
		screenshiftx = (windowx - (renderx * screenmultx)) / 2.0f;
	}
	else {
		screenshiftx = 0.0f;
		screenshifty = 0.0f;
		screenmultx = windowx / renderx;
		screenmulty = screenmultx;
	}

//	printf("SHIFT: %f, %f, %f, %f\n", screenmultx, screenmulty, screenshiftx, screenshifty);
//	printf("X: %f	Y: %f\n", screenshiftx, screenshifty);
}


// CONVERT INTEGER TO STANDARD STRING

std::string int2string(int x)
{
	std::stringstream str;
	str << x;
	return str.str();
}


// ROTATE AROUND X AXIS, PASS Y && Z VALUES BY REFERENCE

void xrotate(float &valy, float &valz, float rotval)
{
	float temp = valy;
	valy = (valy * cos(rotval)) - (valz * sin(rotval));
	valz = (temp * sin(rotval)) + (valz * cos(rotval));
}


// ROTATE AROUND Y AXIS, PASS Z && X VALUES BY REFERENCE

void yrotate(float &valz, float &valx, float rotval)
{
	float temp = valz;
	valz = (valz * cos(rotval)) - (valx * sin(rotval));
	valx = (temp * sin(rotval)) + (valx * cos(rotval));
}


// ROTATE AROUND Z AXIS, PASS X && Y VALUES BY REFERENCE

void zrotate(float &valx, float &valy, float rotval)
{
	float temp = valx;
	valx = (valx * cos(rotval)) - (valy * sin(rotval));
	valy = (temp * sin(rotval)) + (valy * cos(rotval));
}



float round1000(float f)
{
	return floor(f * 100 + 0.05) / 100;
}

float toRadians(float degrees)
{
	return (degrees / 180.0f) * pi;
}

void drawQuad(int textureimage, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool smoothflag)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//	glTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, textureimage);
	if (smoothflag == FALSE)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glDisable(GL_POLYGON_SMOOTH);
	//	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(x1, y1);
	glTexCoord2d(1, 0); glVertex2f(x2, y2);
	glTexCoord2d(1, 1); glVertex2f(x3, y3);
	glTexCoord2d(0, 1); glVertex2f(x4, y4);
	glEnd();
}

void placeimagetile(int textureimage, float x1, float y1, float width, float height, float tilex, float tiley, bool smoothflag)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glBindTexture(GL_TEXTURE_2D, textureimage);
	if (smoothflag == FALSE)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
	glTexCoord2d(tilex, 0); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
	glTexCoord2d(tilex, tiley); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
	glTexCoord2d(0, tiley); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
	glEnd();
}

void placeimageeasy(int textureimage, float x1, float y1, float width, float height, bool smoothflag)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glBindTexture(GL_TEXTURE_2D, textureimage);
	if (smoothflag == FALSE)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
	glTexCoord2d(1, 0); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
	glTexCoord2d(1, 1); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
	glTexCoord2d(0, 1); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
	glEnd();
}

void placeimage(int textureimage, int mask, float x1, float y1, float width, float height, bool smoothflag)
{
	if (mask == NULL)
	{
		placeimageeasy(textureimage, x1, y1, width, height, smoothflag);
	}
	else
	{
		//		glBlendFunc(GL_ONE, GL_ONE);
		//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		if (smoothflag == FALSE)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//		glBlendFunc(GL_ONE, GL_ZERO);
		//		glBlendFunc(GL_SRC_COLOR, GL_ZERO);
		glBindTexture(GL_TEXTURE_2D, textureimage);
		//		glBindTexture(GL_TEXTURE_2D, mask);
		glTexEnvi(GL_TEXTURE_2D, GL_RGB, GL_REPLACE);
		glTexEnvi(GL_TEXTURE_2D, GL_SRC_COLOR, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_2D, GL_RGB, GL_SRC_COLOR);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
		glTexCoord2d(1, 0); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
		glTexCoord2d(1, 1); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
		glTexCoord2d(0, 1); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
		glEnd();

		//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		//		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, mask);
		//		glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		//		glBindTexture(GL_TEXTURE_2D, textureimage);
		glTexEnvi(GL_TEXTURE_2D, GL_ALPHA, GL_SRC_ALPHA);

		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
		glTexCoord2d(1, 0); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1)* screenmulty) + screenshifty);
		glTexCoord2d(1, 1); glVertex2f(((x1 + width) * screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
		glTexCoord2d(0, 1); glVertex2f(((x1)* screenmultx) + screenshiftx, ((y1 + height) * screenmulty) + screenshifty);
		glEnd();
	}
}

void placeimagerotatecentre(int textureimage, float x1, float y1, float width, float height, float rotation, bool smoothflag)
{
	float xvals[4] = { -width * 0.5, width * 0.5, width * 0.5, -width * 0.5 };
	float yvals[4] = { -height * 0.5, -height * 0.5, height * 0.5, height * 0.5 };
	for (int counter = 0; counter < 4; counter++)
	{
		zrotate(xvals[counter], yvals[counter], rotation);
		xvals[counter] += x1; yvals[counter] += y1;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureimage);
	if (smoothflag == FALSE)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f((xvals[0] * screenmultx) + screenshiftx, (yvals[0] * screenmulty) + screenshifty);
	glTexCoord2d(1, 0); glVertex2f((xvals[1] * screenmultx) + screenshiftx, (yvals[1] * screenmulty) + screenshifty);
	glTexCoord2d(1, 1); glVertex2f((xvals[2] * screenmultx) + screenshiftx, (yvals[2] * screenmulty) + screenshifty);
	glTexCoord2d(0, 1); glVertex2f((xvals[3] * screenmultx) + screenshiftx, (yvals[3] * screenmulty) + screenshifty);
	glEnd();
}

void placeimagerotatetile(int textureimage, float x1, float y1, float width, float height, float tilex, float tiley, float rotation, bool smoothflag)
{
	float xvals[4] = { -width * 0.5, width * 0.5, width * 0.5, -width * 0.5 };
	float yvals[4] = { -height * 0.5, -height * 0.5, height * 0.5, height * 0.5 };
	for (int counter = 0; counter < 4; counter++)
	{
		zrotate(xvals[counter], yvals[counter], rotation);
		xvals[counter] += x1; yvals[counter] += y1;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureimage);
	if (smoothflag == FALSE)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f((xvals[0] * screenmultx) + screenshiftx, (yvals[0] * screenmulty) + screenshifty);
	glTexCoord2d(tilex, 0); glVertex2f((xvals[1] * screenmultx) + screenshiftx, (yvals[1] * screenmulty) + screenshifty);
	glTexCoord2d(tilex, tiley); glVertex2f((xvals[2] * screenmultx) + screenshiftx, (yvals[2] * screenmulty) + screenshifty);
	glTexCoord2d(0, tiley); glVertex2f((xvals[3] * screenmultx) + screenshiftx, (yvals[3] * screenmulty) + screenshifty);
	glEnd();
}

void placeimagerotatetile(int textureimage, float x1, float y1, float width, float height, float tilex, float tiley, float tilexoffset, float tileyoffset, float rotation, bool smoothflag)
{
	float xvals[4] = { -width * 0.5, width * 0.5, width * 0.5, -width * 0.5 };
	float yvals[4] = { -height * 0.5, -height * 0.5, height * 0.5, height * 0.5 };
	for (int counter = 0; counter < 4; counter++)
	{
		zrotate(xvals[counter], yvals[counter], rotation);
		xvals[counter] += x1; yvals[counter] += y1;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureimage);
	if (smoothflag == FALSE)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	glTexCoord2d(tilexoffset, tileyoffset); glVertex2f((xvals[0] * screenmultx) + screenshiftx, (yvals[0] * screenmulty) + screenshifty);
	glTexCoord2d(tilex + tilexoffset, tileyoffset); glVertex2f((xvals[1] * screenmultx) + screenshiftx, (yvals[1] * screenmulty) + screenshifty);
	glTexCoord2d(tilex + tilexoffset, tiley + tileyoffset); glVertex2f((xvals[2] * screenmultx) + screenshiftx, (yvals[2] * screenmulty) + screenshifty);
	glTexCoord2d(tilexoffset, tiley + tileyoffset); glVertex2f((xvals[3] * screenmultx) + screenshiftx, (yvals[3] * screenmulty) + screenshifty);
	glEnd();
}

float letterOffset = 0.25;

void drawtext(char* textval, int xstart, int ystart, int letterwidth, int letterheight)
{
	float widthmult = 1.0;
	float heightmult = 1.0;
	xstart -= letterwidth / 2;
	ystart -= letterwidth / 2;
	letterwidth *= 1.7;
	letterheight *= 1.7;
	widthmult *= 0.6;
	heightmult *= 0.6;
	int xval = 0;
	int yval = 0;
	carriage_return = 0;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, font2);

	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		for (int subcount = 0; subcount < 93; subcount++)
		{
			if (textval[counter] == textlist[subcount][0])
			{
				if (textval[counter] == '  ')
				{
					xval += 1;
				}
				else if (textval[counter] == '#')
				{
					yval -= 1;
					xval = 0;
					carriage_return += 1;
				}
				else
				{
					float xdis1 = (font_loc[subcount][0] * charsize);
					float ydis1 = ((font_loc[subcount][1]) * charsize);
					float xdis2 = (xdis1 + charsize) / font_max_x;
					float ydis2 = ((ydis1 + charsize) / font_max_y);
					xdis1 /= font_max_x;
					ydis1 /= font_max_y;
					glBegin(GL_QUADS);
					glTexCoord2f(xdis1, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
					glTexCoord2f(xdis1, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
					glEnd();
					xval += 1;
				}
			}
		}
	}

}

void drawtext(char* textval, int xstart, int ystart, int letterwidth, int letterheight, float widthmult)
{
	float heightmult = 1.0;
	xstart -= letterwidth / 2;
	ystart -= letterwidth / 2;
	letterwidth *= 1.7;
	letterheight *= 1.7;
	widthmult *= 0.6;
	heightmult *= 0.6;
	int xval = 0;
	int yval = 0;
	carriage_return = 0;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, font2);

	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		for (int subcount = 0; subcount < 93; subcount++)
		{
			if (textval[counter] == textlist[subcount][0])
			{
				if (textval[counter] == '  ')
				{
					xval += 1;
				}
				else if (textval[counter] == '#')
				{
					yval -= 1;
					xval = 0;
					carriage_return += 1;
				}
				else
				{
					float xdis1 = (font_loc[subcount][0] * charsize);
					float ydis1 = ((font_loc[subcount][1]) * charsize);
					float xdis2 = (xdis1 + charsize) / font_max_x;
					float ydis2 = ((ydis1 + charsize) / font_max_y);
					xdis1 /= font_max_x;
					ydis1 /= font_max_y;
					glBegin(GL_QUADS);
					glTexCoord2f(xdis1, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
					glTexCoord2f(xdis1, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
					glEnd();
					xval += 1;
				}
			}
		}
	}

}

void drawtext_centre(char* textval, int xstart, int ystart, int letterwidth, int letterheight)
{
	float widthmult = 0.9;
	float heightmult = 1.3;

	int xval = 0;
	int yval = 0;
	carriage_return = 0;
	int lengthval = 0;
	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		lengthval += 1;
	}
	float xdisplace = -((lengthval + 0.5f) * letterwidth * widthmult) / 2.0f;
	xstart += xdisplace;

	xstart -= letterwidth / 4;
//	ystart -= letterwidth / 2;
	letterwidth *= 2;
	letterheight *= 2;
	widthmult *= 0.5;
	heightmult *= 0.5;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, font2);

	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		for (int subcount = 0; subcount < 93; subcount++)
		{
			if (textval[counter] == textlist[subcount][0])
			{
				if (textval[counter] == '  ')
				{
					xval += 1;
				}
				else if (textval[counter] == '#')
				{
					yval += 1;
					xval = 0;
					carriage_return += 1;
				}
				else
				{
					float xdis1 = (font_loc[subcount][0] * charsize);
					float ydis1 = ((font_loc[subcount][1]) * charsize);
					float xdis2 = (xdis1 + charsize) / font_max_x;
					float ydis2 = ((ydis1 + charsize) / font_max_y);
					xdis1 /= font_max_x;
					ydis1 /= font_max_y;
					glBegin(GL_QUADS);
					glTexCoord2f(xdis1, ydis1); glVertex2f(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) - (letterheight / 2)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis1); glVertex2f(((xstart + (xval * letterwidth * widthmult) + (letterwidth)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) - (letterheight / 2)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis2); glVertex2f(((xstart + (xval * letterwidth * widthmult) + (letterwidth)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + (letterheight / 2)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis1, ydis2); glVertex2f(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + (letterheight / 2)) * screenmulty) + screenshifty);
					glEnd();
					xval += 1;
				}
			}
		}
	}
}

void drawtext_centre(char* textval, int xstart, int ystart, int letterwidth, int letterheight, float widthmult)
{
	float heightmult = 1.3;

	int xval = 0;
	int yval = 0;
	carriage_return = 0;
	int lengthval = 0;
	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		lengthval += 1;
	}
	float xdisplace = -((lengthval + 0.5f) * letterwidth * widthmult) / 2.0f;
	xstart += xdisplace;

	xstart -= letterwidth / 4;
	//	ystart -= letterwidth / 2;
	letterwidth *= 2;
	letterheight *= 2;
	widthmult *= 0.5;
	heightmult *= 0.5;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, font2);

	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		for (int subcount = 0; subcount < 93; subcount++)
		{
			if (textval[counter] == textlist[subcount][0])
			{
				if (textval[counter] == '  ')
				{
					xval += 1;
				}
				else if (textval[counter] == '#')
				{
					yval += 1;
					xval = 0;
					carriage_return += 1;
				}
				else
				{
					float xdis1 = (font_loc[subcount][0] * charsize);
					float ydis1 = ((font_loc[subcount][1]) * charsize);
					float xdis2 = (xdis1 + charsize) / font_max_x;
					float ydis2 = ((ydis1 + charsize) / font_max_y);
					xdis1 /= font_max_x;
					ydis1 /= font_max_y;
					glBegin(GL_QUADS);
					glTexCoord2f(xdis1, ydis1); glVertex2f(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) - (letterheight / 2)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis1); glVertex2f(((xstart + (xval * letterwidth * widthmult) + (letterwidth)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) - (letterheight / 2)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis2, ydis2); glVertex2f(((xstart + (xval * letterwidth * widthmult) + (letterwidth)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + (letterheight / 2)) * screenmulty) + screenshifty);
					glTexCoord2f(xdis1, ydis2); glVertex2f(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart - (yval * letterheight * heightmult) + (letterheight / 2)) * screenmulty) + screenshifty);
					glEnd();
					xval += 1;
				}
			}
		}
	}
}

void drawtextCropped2(char* textval, int xstart, int ystart, int letterwidth, int letterheight, int linelength, int font)
{
	float widthmult = 1.0;
	float heightmult = 1.0;
	xstart -= letterwidth;
	ystart -= letterwidth;
	letterwidth *= 1.7;
	letterheight *= 1.7;
	widthmult *= 0.6;
	heightmult *= 0.6;

	char calctext[2000][100];
	int lword = 0;
	int length = sizeof(textval);
	int xplace = 0;
	int yplace = 0;
	for (int counter = 0; !textval[counter] == '\0'; counter++)
	{
		calctext[yplace][xplace] = textval[counter];
		if (textval[counter] == '#')
		{
			xplace = -1;
			yplace += 1;
			lword = 0;
		}
		if (textval[counter] != ' ' && textval[counter] != '#')
		{
			if (textval[counter - 1] == ' ' || counter == 0)
			{
				lword = xplace;
			}
		}
		if (xplace > linelength)
		{
			for (int subcount = lword; subcount < linelength + 1; subcount++)
			{
				calctext[yplace][subcount] = '#';
			}
			counter -= (linelength - lword) + 2;
			xplace = -1;
			yplace += 1;
			lword = 0;
		}
		xplace += 1;
	}
	calctext[yplace][xplace] = '\0';
	int xval = 0;
	int yval = 0;
	for (int counter = 0; !calctext[yval][xval] == '\0'; counter++)
	{
		for (int subcount = 0; subcount < 93; subcount++)
		{
			if (calctext[yval][xval] == textlist[subcount][0])
			{
				if (calctext[yval][xval] == '  ')
				{
					xval += 1;
				}
				else if (calctext[yval][xval] == '#')
				{
					yval += 1;
					xval = 0;
				}
				else
				{
					if (xval < linelength)
					{
						float xdis1 = (font_loc[subcount][0] * charsize);
						float ydis1 = ((font_loc[subcount][1]) * charsize);
						float xdis2 = (xdis1 + charsize) / font_max_x;
						float ydis2 = ((ydis1 + charsize) / font_max_y);
						xdis1 /= font_max_x;
						ydis1 /= font_max_y;
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, font);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glBegin(GL_QUADS);
						glTexCoord2f(xdis1, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
						glTexCoord2f(xdis2, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
						glTexCoord2f(xdis2, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
						glTexCoord2f(xdis1, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
						glEnd();
					}
					xval += 1;
				}
				if (xval > linelength)
				{
					xval = 0;
					yval += 1;
				}
			}
		}
	}
}

void drawtextCropped(char* textval, int xstart, int ystart, int letterwidth, int letterheight, int linelength, float widthmult)
{
	float heightmult = 1.0;
	xstart -= letterwidth / 2;
	ystart -= letterwidth / 2;
	letterwidth *= 1.7;
	letterheight *= 1.7;
	widthmult *= 0.6;
	heightmult *= 0.6;
	char calctext[2000][100];
	int lword = 0;
	int length = sizeof(textval);
	int xplace = 0;
	int yplace = 0;
	for (int counter = 0; textval[counter] != '\0'; counter++)
	{
		calctext[yplace][xplace] = textval[counter];
		if (textval[counter] == '#')
		{
			xplace = -1;
			yplace += 1;
			lword = 0;
		}
		if (textval[counter] != ' ' && textval[counter] != '#')
		{
			if (textval[counter - 1] == ' ' || counter == 0)
			{
				lword = xplace;
			}
		}
		if (xplace >= linelength)
		{
			for (int subcount = lword; subcount < linelength + 1; subcount++)
			{
				calctext[yplace][subcount] = '#';
			}
			counter -= (linelength - lword) + 1;
			xplace = -1;
			yplace += 1;
			lword = 0;
		}
		xplace += 1;
	}
	calctext[yplace][xplace] = '\0';
	int xval = 0;
	int yval = 0;
	for (int counter = 0; !calctext[yval][xval] == '\0'; counter++)
	{
		for (int subcount = 0; subcount < 93; subcount++)
		{
			if (calctext[yval][xval] == textlist[subcount][0])
			{
				if (calctext[yval][xval] == '  ' && xval != 0)
				{
					xval += 1;
				}
				else if (calctext[yval][xval] == '#')
				{
					yval += 1;
					xval = 0;
				}
				else
				{
					if (xval < linelength)
					{
						float xdis1 = (font_loc[subcount][0] * charsize);
						float ydis1 = ((font_loc[subcount][1]) * charsize);
						float xdis2 = (xdis1 + charsize) / font_max_x;
						float ydis2 = ((ydis1 + charsize) / font_max_y);
						xdis1 /= font_max_x;
						ydis1 /= font_max_y;
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, font2);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glBegin(GL_QUADS);
						glTexCoord2f(xdis1, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
						glTexCoord2f(xdis2, ydis1); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult)) * screenmulty) + screenshifty);
						glTexCoord2f(xdis2, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult) + letterwidth) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
						glTexCoord2f(xdis1, ydis2); glVertex2i(((xstart + (xval * letterwidth * widthmult)) * screenmultx) + screenshiftx, ((ystart + (yval * letterheight * heightmult) + letterheight) * screenmulty) + screenshifty);
						glEnd();
					}
					xval += 1;
				}
				if (xval > linelength)
				{
					xval = 0;
					yval += 1;
				}
			}
		}
	}
}

int getintsize(int number)
{
	int rtnsize = 1;
	if (number >= 10)
	{
		rtnsize = 2;
	}
	if (number >= 100)
	{
		rtnsize = 3;
	}
	if (number >= 1000)
	{
		rtnsize = 4;
	}
	if (number >= 10000)
	{
		rtnsize = 5;
	}
	if (number >= 100000)
	{
		rtnsize = 6;
	}
	if (number >= 1000000)
	{
		rtnsize = 7;
	}
	if (number >= 10000000)
	{
		rtnsize = 8;
	}
	if (number >= 100000000)
	{
		rtnsize = 9;
	}
	return rtnsize;
}
