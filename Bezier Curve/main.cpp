#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>

#include "../lib/graphics.h"
#pragma comment(lib, "lib/graphics.lib")

#define PI 3.14159265

// 640x480 window

/* 
Colors
0	BLACK
1	BLUE
2	GREEN
3	CYAN
4	RED
5	MAGENTA
6	BROWN
7	LIGHTGRAY
8	DARKGRAY
9	LIGHTBLUE
10	LIGHTGREEN
11	LIGHTCYAN
12	LIGHTRED
13	LIGHTMAGENTA
14	YELLOW
15	WHITE
*/

void getGraph()
{
	int gd = DETECT, gm;
	char pathtodriver[] = "";
	initgraph(&gd, &gm, pathtodriver);
}

void drawBresenhamLine(int x1, int y1, int x2, int y2, int color)
{
	float dx = abs(x2 - x1);
	float dy = abs(y2 - y1);

	float sx = x1 < x2 ? 1 : -1;
	float sy = y1 < y2 ? 1 : -1;

	float error = dx - dy;

	putpixel(x2, y2, color);

	while (x1 != x2 || y1 != y2)
	{
		putpixel(x1, y1, color);
		float error2 = error * 2;

		if (error2 > -dy)
		{
			error -= dy;
			x1 += sx;
		}
		if (error2 < dx)
		{
			error += dx;
			y1 += sy;
		}
	}
}

void drawBresenhamCircle(int x0, int y0, int radius, int color)
{
	int x = 0;
	int y = radius;
	int d = 1 - 2 * radius;
	int error = 0;

	while (y >= 0)
	{
		putpixel(x0 + x, y0 + y, color);
		putpixel(x0 + x, y0 - y, color);
		putpixel(x0 - x, y0 + y, color);
		putpixel(x0 - x, y0 - y, color);
		error = 2 * (d + y) - 1;

		if (d < 0 && error <= 0)
		{
			x++;
			d += 2 * x + 1;
			continue;
		}

		error = 2 * (d - x) - 1;

		if (d > 0 && error > 0)
		{
			y--;
			d += 1 - 2 * y;
			continue;
		}

		x++;
		d += 2 * (x - y);
		y--;
	}
}

double getBinomalCoef(int n, int k)
{
	if (k > n - k) k = n - k;

	double result = 1.0;

	for (int i = 1; i <= k; i++)
	{
		result *= (double)(n - i + 1) / i;
	}

	return result;
}


void getBezierCurve(const std::unique_ptr<int[]>& x, const std::unique_ptr<int[]>& y, int size, int color)
{
	double xu = 0.0;
	double yu = 0.0;

	for (double t = 0.0; t <= 1.0; t += 0.0001)
	{
		xu = 0.0;
		yu = 0.0;

		for (int i = 0; i <= size - 1; i++)
		{
			xu += getBinomalCoef(size - 1, i) * pow(1 - t, (size - 1) - i)
				* pow(t, i) * x[i];
			yu += getBinomalCoef(size - 1, i) * pow(1 - t, (size - 1) - i)
				* pow(t, i) * y[i];
		}

		putpixel((int)xu, (int)yu, color);
	}
}

void rotateBezierCurve(std::unique_ptr<int[]>& x, std::unique_ptr<int[]>& y, 
	int size, int angle)
{
	double newAngle = angle * PI / 180;
	double xnew, ynew;

	double pivotx = 0.0;
	double pivoty = 0.0;

	for (int i = 0; i < size; i++) {
		pivotx += x[i];
		pivoty += y[i];
	}

	pivotx /= size;
	pivoty /= size;

	for (int i = 0; i < size; i++)
	{
		double tempx = x[i] - pivotx;
		double tempy = y[i] - pivoty;

		xnew = tempx * cos(newAngle) - tempy * sin(newAngle);
		ynew = tempx * sin(newAngle) + tempy * cos(newAngle);

		x[i] = xnew + pivotx;
		y[i] = ynew + pivoty;

		std::cout << "[" << x[i] << " ; " << y[i] << "]" << std::endl;
	}
}

void scaleBezierCurve(std::unique_ptr<int[]>& x, std::unique_ptr<int[]>& y, 
	int size, double scaleX, double scaleY)
{
	for (int i = 0; i < size; i++)
	{

		x[i] *= scaleX;
		y[i] *= scaleY;
	}
}

void reflectBezierCurveX(std::unique_ptr<int[]>& x, int size, int width)
{
	for (int i = 0; i < size; i++)
	{
		x[i] = width - x[i];
	}
}

void reflectBezierCurveY(std::unique_ptr<int[]>& y, int size, int height)
{
	for (int i = 0; i < size; i++)
	{
		y[i] = height - y[i];
	}
}

void translateBezierCurve(std::unique_ptr<int[]>& x, std::unique_ptr<int[]>& y, 
	int size, int distanceX, int distanceY)
{
	for (int i = 0; i < size; i++)
	{

		x[i] += distanceX;
		y[i] += distanceY;
	}
}

void initArray(std::unique_ptr<int[]>& arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cin >> arr[i];
	}
}

void equalArrays(const std::unique_ptr<int[]>& arr, std::unique_ptr<int[]>& tmp, int size)
{
	for (int i = 0; i < size; i++)
	{
		tmp[i] = arr[i];
	}
}

bool isEqual(const std::unique_ptr<int[]>& a, const std::unique_ptr<int[]>& b, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (a[i] != b[i])
			return false;
	}

	return true;
}

void drawBezierCurve(const std::unique_ptr<int[]>& x, const std::unique_ptr<int[]>& y,
	int size, int colorBezier, int colorCircle, int colorLine)
{
	for (int i = 0; i < size - 1; i++)
	{
		drawBresenhamCircle(x[i], y[i], 5, colorCircle);
		drawBresenhamLine(x[i], y[i], x[i + 1], y[i + 1], colorLine);
	}
	drawBresenhamCircle(x[size - 1], y[size - 1], 5, colorCircle);
	getBezierCurve(x, y, size, colorBezier);
}

void menu()
{
	system("cls");
	std::cout << "1. Draw Bezier Curve\n"
		<< "2. Rotate\n"
		<< "3. Scale\n"
		<< "4. Reflect by x\n"
		<< "5. Reflect by y\n"
		<< "6. Translate\n"
		<< "7. Change coords and size\n\n";
}

int main()
{
	std::cout << "Choose number of control points: ";
	size_t size;
	std::cin >> size;

	std::unique_ptr<int[]> x = std::make_unique<int[]>(size);
	std::unique_ptr<int[]> y = std::make_unique<int[]>(size);
	
	std::unique_ptr<int[]> tmpX = std::make_unique<int[]>(size);
	std::unique_ptr<int[]> tmpY = std::make_unique<int[]>(size);

	std::cout << "X[" << size << "]: ";
	initArray(x, size);
	std::cout << "Y[" << size << "]: ";
	initArray(y, size);

	equalArrays(x, tmpX, size);
	equalArrays(y, tmpY, size);

	std::cout << "Bezier Curve color: ";
	int colorBezier;
	std::cin >> colorBezier;

	std::cout << "Line color: ";
	int colorLine;
	std::cin >> colorLine;

	int colorCircle = 3; // cyan

	while (1)
	{
		menu();
		int choice;
		std::cin >> choice;

		switch (choice)
		{
		case 1: // draw
			getGraph();
			drawBezierCurve(x, y, size, colorBezier, colorCircle, colorLine);

			if (!isEqual(x, tmpX, size) || !isEqual(y, tmpY, size))
			{
				drawBezierCurve(tmpX, tmpY, size, 15, 15, 15);
			}

			getch();
			closegraph();
			break;

		case 2: // rotate
			equalArrays(x, tmpX, size);
			equalArrays(y, tmpY, size);

			std::cout << "Rotation angle(in degrees): ";
			int angle;
			std::cin >> angle;

			rotateBezierCurve(x, y, size, angle);

			std::cout << "\n\n[Successful]";
			delay(500);
			break;

		case 3: // scale
			equalArrays(x, tmpX, size);
			equalArrays(y, tmpY, size);

			std::cout << "Scale X: ";
			double scaleX;
			std::cin >> scaleX;

			std::cout << "Scale Y: ";
			double scaleY;
			std::cin >> scaleY;

			scaleBezierCurve(x, y, size, scaleX, scaleY);

			std::cout << "\n\n[Successful]";
			delay(1000);
			break;

		case 4: // reflect by x
			equalArrays(x, tmpX, size);
			equalArrays(y, tmpY, size);
			reflectBezierCurveX(x, size, 640);

			std::cout << "\n\n[Successful]";
			delay(1000);
			break;

		case 5: // reflect by y
			equalArrays(x, tmpX, size);
			equalArrays(y, tmpY, size);
			reflectBezierCurveY(y, size, 480);

			std::cout << "\n\n[Successful]";
			delay(1000);
			break;

		case 6: // translate
			equalArrays(x, tmpX, size);
			equalArrays(y, tmpY, size);
			std::cout << "Distance for X offset: ";
			int offsetX;
			std::cin >> offsetX;

			std::cout << "Distance for Y offset: ";
			int offsetY;
			std::cin >> offsetY;

			translateBezierCurve(x, y, size, offsetX, offsetY);

			std::cout << "\n\n[Successful]";
			delay(1000);
			break;

		case 7:
			std::cout << "Choose new number of control points: ";
			std::cin >> size;

			std::cout << "X[" << size << "]: ";
			initArray(x, size);

			std::cout << "Y[" << size << "]: ";
			initArray(y, size);

			equalArrays(x, tmpX, size);
			equalArrays(y, tmpY, size);

			std::cout << "\n\n[Successful]";
			delay(1000);
			break;

		default:
			continue;
		}
	}
}
