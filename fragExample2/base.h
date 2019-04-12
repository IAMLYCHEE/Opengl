#ifndef __SB7_H__
#define __SB7_H__

#define Sleep(t) sleep(t)


#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include<iostream>

#include <stdio.h>
#include <string.h>
#include <math.h>

namespace sb7
{

	class application
	{
	public:
		application() {}
		virtual ~application() {}
		virtual void run()
		{
			bool running = true;

			if (!glfwInit())
			{
				return;
			}

			init();

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			{
				window = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
				if (!window)
				{
					fprintf(stderr, "Failed to open window\n");
					return;
				}
			}

			glfwMakeContextCurrent(window);

			// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
			glewExperimental = GL_TRUE;
			// Initialize GLEW to setup the OpenGL Function pointers
			if (GLEW_OK != glewInit())
			{
				std::cout << "Failed to initialize GLEW" << std::endl;
				return ;
			}
			startup();
			do
			{
				render(glfwGetTime());

				glfwSwapBuffers(window);
				glfwPollEvents();

				running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
				running &= (glfwWindowShouldClose(window) != GL_TRUE);
			} while (running);

			shutdown();

			glfwDestroyWindow(window);
			glfwTerminate();
		}

		virtual void init()
		{
		}

		virtual void startup()
		{

		}

		virtual void render(double currentTime)
		{

		}

		virtual void shutdown()
		{

		}


		virtual void onKey(int key, int action)
		{

		}

		virtual void onMouseButton(int button, int action)
		{

		}

		virtual void onMouseMove(int x, int y)
		{

		}

		virtual void onMouseWheel(int pos)
		{

		}

		void getMousePosition(int& x, int& y)
		{
			double dx, dy;
			glfwGetCursorPos(window, &dx, &dy);

			x = static_cast<int>(floor(dx));
			y = static_cast<int>(floor(dy));
		}


	protected:
		GLFWwindow* window;
	};

};

/*
#define DECLARE_MAIN(a)                             \
sb7::application *app = 0;                          \
int CALLBACK WinMain(HINSTANCE hInstance,           \
                     HINSTANCE hPrevInstance,       \
                     LPSTR lpCmdLine,               \
                     int nCmdShow)                  \
{                                                   \
    a *app = new a;                                 \
    app->run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}
*/
#endif /* __SB7_H__ */

