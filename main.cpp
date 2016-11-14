#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include "TetrahedronPuzzle.hpp"

int main()
{
	glfwInit();
	int count;
	GLFWmonitor ** monitors = glfwGetMonitors(&count);
	GLFWmonitor * monitor = monitors[0];
	GLFWvidmode const * mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//GLFWwindow * window = glfwCreateWindow(mode->width, mode->height, "TetrahedronPuzzle", monitor, nullptr);
	GLFWwindow * window = glfwCreateWindow(mode->width, mode->height, "TetrahedronPuzzle", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);
		glPushMatrix();

		//window size chagne
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glLoadIdentity();
		gluPerspective(90, static_cast<double>(width) / static_cast<double>(height), 1, 128);
		gluLookAt(0, 20, 20, 0, 20, 0, 0, 1, 0);

		constexpr GLubyte color[][3] =
		{
			{0xFF, 0xA0, 0x00},
			{0xFF, 0xFF, 0x00},
        	{0x00, 0xFF, 0x00},
			{0x00, 0x00, 0xFF},
			{0xFF, 0xFF, 0xFF},
			{0xFF, 0x00, 0x00}
		};

		constexpr GLdouble radius = 2;
		double const pi = 2 * std::acos(0);

		GLdouble const vertex[4][2][3][3] =
		{
			{
				{
					{std::cos((pi / 3) * 0) + ((std::cos((pi / 3) * 0) - std::cos((pi / 3) * 1)) / 2), std::sin((pi / 3) * 0), std::sin((pi / 3) * 0)},
					{std::cos((pi / 3) * 2) + ((std::cos((pi / 3) * 0) - std::cos((pi / 3) * 1)) / 2), std::sin((pi / 3) * 2), std::sin((pi / 3) * 0)},
					{std::cos((pi / 3) * 4) + ((std::cos((pi / 3) * 0) - std::cos((pi / 3) * 1)) / 2), std::sin((pi / 3) * 4), std::sin((pi / 3) * 0)},
				},
				{
					{std::cos((pi / 3) * 1) + ((std::cos((pi / 3) * 3) - std::cos((pi / 3) * 2)) / 2), std::sin((pi / 3) * 1), std::sin((pi / 3) * 0)},
					{std::cos((pi / 3) * 3) + ((std::cos((pi / 3) * 3) - std::cos((pi / 3) * 2)) / 2), std::sin((pi / 3) * 3), std::sin((pi / 3) * 0)},
					{std::cos((pi / 3) * 5) + ((std::cos((pi / 3) * 3) - std::cos((pi / 3) * 2)) / 2), std::sin((pi / 3) * 5), std::sin((pi / 3) * 0)},
				},
			},
		};

		constexpr int N = 4;
		TetrahedronPuzzle<N> t;
		t.rotate({{1,0,0,0}}, 3, true);
		t.rotate({{1,0,0,0}}, 3, false);
		for (int i = 0; i < TetrahedronPuzzle<N>::SURFACE_LENGTH; ++i)
		{
			for (int row = 0; row < N; ++row)
			{
				for (int column = 0; column <= (row * 2); ++column)
				{
					glPushMatrix();

					//glTranslated(column-row, row*1.6 + i * 10, 0);
					glTranslated(column-row, row *0.5+ i * 10, 0);
					//glTranslated(row, column-row, 0);
					glColor3ubv(color[t.getTile(i,row,column)]);
					glBegin(GL_TRIANGLES);
					for (auto && e : vertex[0][column % 2])
					{
						glVertex3dv(e);
					}
					glEnd();

					glPopMatrix();
				}
			}
		}

		glPopMatrix();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

