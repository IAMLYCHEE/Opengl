#define Sleep(t) sleep(t)
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include<iostream>
#include <stdio.h>
#include <math.h>
#include"imGUI/imgui.h"
#include"imGUI/imgui_impl_glfw_gl3.h"

namespace sb7 {
	class application
	{
	public:
		struct APPINFO {
			std::string title = "OpenGL window";
			int windowWidth = 800;
			int windowHeight = 600;
			int majorVersion = 4;
			int minorVersion = 5;
			int samples = 0;
			union {
				struct {
					unsigned int fullscreen : 1;
					unsigned int vsync : 1;
					unsigned int cursor : 1;
					unsigned int stereo : 1;
					unsigned int debug : 1;
					unsigned int robust : 1;
				};
				unsigned int all;
			}flags;
		};

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

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_SAMPLES, info.samples);
			glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
			//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title.c_str(), NULL, NULL);
			if (!window)
			{
				fprintf(stderr, "Failed to open window\n");
				return;
			}

			glfwMakeContextCurrent(window);
			//glfwSetWindowSizeCallback(window, glfw_onResize);
			//glfwSetKeyCallback(window, glfw_onKey);
			//glfwSetMouseButtonCallback(window, glfw_onMouseButton);
			//glfwSetCursorPosCallback(window, glfw_onMouseMove);
			//glfwSetScrollCallback(window, glfw_onMouseWheel);
			if (!info.flags.cursor)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}

			{
				// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
				glewExperimental = GL_TRUE;
				// Initialize GLEW to setup the OpenGL Function pointers
				if (GLEW_OK != glewInit())
				{
					std::cout << "Failed to initialize GLEW" << std::endl;
					return;
				}
			}

			//Setup ImGui context
			//=====================================gui================================//
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui_ImplGlfwGL3_Init(window, true);
			ImGui::StyleColorsDark();

			startup();
			do
			{
				ImGui_ImplGlfwGL3_NewFrame();
				render(glfwGetTime());

				running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
				running &= (glfwWindowShouldClose(window) != GL_TRUE);

				ImGui::Render();
				ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(window);
				glfwPollEvents();
			} while (running);

			shutdown();


			//ImGui clean up
			ImGui_ImplGlfwGL3_Shutdown();
			ImGui::DestroyContext();
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		virtual void init()
		{
			info.title = "opengl example";
			info.windowHeight = 600;
			info.windowWidth = 800;
			info.majorVersion = 4;
			info.minorVersion = 5;
			info.samples = 0;
			info.flags.all = 0;
			info.flags.cursor = 1;
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

		void setWindowTitle(const char* title) {
			glfwSetWindowTitle(window, title);
		}

		virtual void onResize(int w, int h) {
			info.windowHeight = h;
			info.windowWidth = w;
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
		APPINFO info;
		GLFWwindow* window;

	};
}
