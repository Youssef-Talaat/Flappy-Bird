// GraphicsProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GL\glut.h"
#include "GL\freeglut.h"
#include <stdlib.h>
#include <string>
#include <time.h>
#include <thread>
#include <iostream>

using namespace std;

float theta;
float radius = 70;
float twoPI = 2 * 3.142;
int width = 640;
int height = 480;
int groundLv = 100;
int gapHeight = 140;
int pipeX = 80;
int dx = 150;
const int pipeNo = 5;
int heights[pipeNo][2];
int xStart[pipeNo];
int birdPosition[2];
int counter = 0;
bool isClick = true;


unsigned char pixel[3] = { 135, 206, 250 };
unsigned char skyPixelColor[3] = { 135, 206, 250 };

void generateHeights() {
	for (int i = 0; i < pipeNo; i++) {
		heights[i][0] = (rand() % (height - groundLv - gapHeight)) + groundLv + gapHeight; // varies from 160 to 480-1
		heights[i][1] = heights[i][0] - gapHeight;
		//cout << heights[i][0] << " " << heights[i][1] << endl;
	}
}

void output(int x, int y)
{
	string scoreVeiw = "Score : " + to_string(counter);
	glColor3f(1, 0, 0);
	glRasterPos2f(x, y);
	for (int i = 0; i < scoreVeiw.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreVeiw[i]);
	}
}

void moveLeft() {
	for (int i = 0; i < pipeNo; i++) {
		xStart[i] -= 3;
		if ((birdPosition[0] - (xStart[i] + pipeX)) == 1 || (birdPosition[0] - (xStart[i] + pipeX)) == 2 ||
			(birdPosition[0] - (xStart[i] + pipeX)) == 3) {
			counter++;
			//cout << counter << endl;
		}
	}
}

void shiftLeft() {
	for (int i = 1; i < pipeNo; i++) {
		xStart[i - 1] = xStart[i];
		heights[i - 1][0] = heights[i][0];
		heights[i - 1][1] = heights[i][1];
	}
	heights[pipeNo-1][0] = (rand() % (height - groundLv - gapHeight)) + groundLv + gapHeight; // varies from 160 to 480-1
	heights[pipeNo-1][1] = heights[pipeNo-1][0] - gapHeight;

}

void checkPipes() {
	if (xStart[0] + pipeX <= 1) {
		shiftLeft();
	}
}

bool Collision(){
	for (int i = 0; i < pipeNo; i++)
	{
		for (int j = heights[i][0]; j < height - 1; j++) // top Tube left side
		{
			glReadPixels(xStart[i] - 1, j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

			if (pixel[0] != skyPixelColor[0] || pixel[1] != skyPixelColor[1] || pixel[2] != skyPixelColor[2])
			{
				return true;
			}

		}
		for (int j = xStart[i]; j < xStart[i] + pipeX; j++) // top Tube bottom side
		{
			glReadPixels(j, heights[i][0] - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			if (pixel[0] != skyPixelColor[0] || pixel[1] != skyPixelColor[1] || pixel[2] != skyPixelColor[2])
			{
				return true;
			}
		}
		for (int j = xStart[i]; j < xStart[i] + pipeX; j++) // bottom tube top side
		{
			glReadPixels(j, heights[i][1] + 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			if (pixel[0] != skyPixelColor[0] || pixel[1] != skyPixelColor[1] || pixel[2] != skyPixelColor[2])
			{
				return true;
			}
		}
		for (int j = groundLv + 1; j < heights[i][1]; j++) // bottom tube left side
		{
			glReadPixels(xStart[i] - 1, j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

			if (pixel[0] != skyPixelColor[0] || pixel[1] != skyPixelColor[1] || pixel[2] != skyPixelColor[2])
			{
				return true;
			}
		}

	}

	return false;
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window.
	glColor3ub(135, 206, 250);//sky
	glBegin(GL_POLYGON);
	glVertex2i(0, groundLv);
	glVertex2i(0, height);
	glVertex2i(width, height);
	glVertex2i(width, groundLv);
	glEnd();

	glColor3ub(34, 139, 34);//grass
	glBegin(GL_POLYGON);
	glVertex2i(0, 0);
	glVertex2i(0, groundLv);
	glVertex2i(width, groundLv);
	glVertex2i(width, 0);
	glEnd();

	glColor3f(1, 1, 0);//bird
	glBegin(GL_POLYGON);//big circle
	for (int i = 0; i < 360; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] + 20 * cos(theta), birdPosition[1] + 20 * sin(theta));
	}
	glEnd();
	glFlush();
	glColor3f(0, 0, 0);
	glLineWidth(2);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);//big black circle
	for (int i = 0; i < 360; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] + 20 * cos(theta), birdPosition[1] + 20 * sin(theta));

	}
	glEnd();
	glFlush();
	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);//black arc
	for (int i = 60; i < 240; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] + 10 + 9 * cos(theta), birdPosition[1] + 5 + 9 * sin(theta));
	}
	glEnd();
	glFlush();
	glColor3f(1, 0.5, 0);
	glBegin(GL_POLYGON); //orange eye
	for (int i = 0; i < 360; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] + 11 + 5 * cos(theta), birdPosition[1] + 4 + 5 * sin(theta));
	}
	glEnd();
	glFlush();

	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_POLYGON); //black eye
	for (int i = 0; i < 360; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] + 11 + 3 * cos(theta), birdPosition[1] + 4 + 3 * sin(theta));

	}
	glEnd();
	glFlush();

	glColor3f(1, 0.5, 0);
	glLineWidth(2);
	glBegin(GL_POLYGON);//Orange wings
	for (int i = 0; i < 360; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] - 18 + 12 * cos(theta), birdPosition[1] + 1 + 8 * sin(theta));

	}
	glEnd();
	glFlush();

	glColor3f(0, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);//black wings
	for (int i = 0; i < 360; i++)
	{
		theta = i * 3.142 / 180;
		glVertex2f(birdPosition[0] - 18 + 12 * cos(theta), birdPosition[1] + 1 + 8 * sin(theta));

	}
	glEnd();
	glFlush();

	glColor3f(1, 0, 0);
	glPointSize(2);
	glBegin(GL_POLYGON);//red lips
	glVertex2d(birdPosition[0] + 5, birdPosition[1] - 10);
	glVertex2d(birdPosition[0] + 25, birdPosition[1] - 10);
	glVertex2d(birdPosition[0] + 25, birdPosition[1] - 2.5);
	glVertex2d(birdPosition[0] + 5, birdPosition[1] - 2.5);

	glEnd();
	glFlush();
	glColor3f(0, 0, 0);
	glPointSize(1);
	glBegin(GL_LINE_LOOP); //black lips
	glVertex2d(birdPosition[0] + 5, birdPosition[1] - 10);
	glVertex2d(birdPosition[0] + 25, birdPosition[1] - 10);
	glVertex2d(birdPosition[0] + 25, birdPosition[1] - 2.5);
	glVertex2d(birdPosition[0] + 5, birdPosition[1] - 2.5);

	glEnd();
	glFlush();

	glColor3f(0, 0, 0);
	glPointSize(1);
	glBegin(GL_LINE_STRIP); //black lips
	glVertex2d(birdPosition[0] + 10, birdPosition[1] - 7);
	glVertex2d(birdPosition[0] + 25, birdPosition[1] - 6);

	glEnd();
	glFlush();

	/////////////////////////////////////////////////////////////

	for (int i = 0; i < pipeNo; i++) {
		if (i > 0) {
			xStart[i] = xStart[i - 1] + pipeX + dx;
		}

		glColor3ub(255, 165, 0);
		glBegin(GL_POLYGON);//upper pipe
		glVertex2i(xStart[i], height);
		glVertex2i(xStart[i], heights[i][0]);
		glVertex2i(xStart[i] + pipeX, heights[i][0]);
		glVertex2i(xStart[i] + pipeX, height);
		glEnd();

		glBegin(GL_POLYGON);//lower pipe
		glVertex2i(xStart[i], groundLv);
		glVertex2i(xStart[i], heights[i][1]);
		glVertex2i(xStart[i] + pipeX, heights[i][1]);
		glVertex2i(xStart[i] + pipeX, groundLv);
		glEnd();
	}
	
	output(290, 50);
	glFlush();


	moveLeft();
	checkPipes();
	/*if (Collision())
		system("pause");
	if (birdPosition[1] - 20 == groundLv) {
		cout << "collide 5" << endl;
		system("pause");
	}
	else*/
	if (Collision() || birdPosition[1] - 20 <= groundLv)
	{
		char s[32];
		sprintf_s(s, "Your Score is : %d", counter);
		MessageBoxA(NULL, s, "Game Over", MB_OK | MB_ICONEXCLAMATION);
		exit(0);

	}
		birdPosition[1] -= 2;
	if (GetAsyncKeyState(VK_UP) && isClick) {
		if (birdPosition[1] < 460) {
			birdPosition[1] += 40;
			isClick = false;
		}
	}
	if (!GetAsyncKeyState(VK_UP) && !isClick) {
		isClick = true;
	}
}

void moveScene() {
	while (true)
	{
		drawScene();
		Sleep(10);
	}
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv); //The parameters are passed from those of the main function.
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(50, 50); //The initial position of the window from the top left corner of the screen.
	glutInitWindowSize(width, height); //The size of the window.
	glutCreateWindow("Testing"); //Creates the window and sets its name.
	gluOrtho2D(0, width, 0, height); //Resolution
	xStart[0] = 300;
	birdPosition[0] = 100;
	birdPosition[1] = 186;
	generateHeights();
	glutDisplayFunc(moveScene); //Calls the function with the instructions of what is to be displayed on the window created.
	glutMainLoop(); //Hands the glut library the execution control.
	return 0;
}


