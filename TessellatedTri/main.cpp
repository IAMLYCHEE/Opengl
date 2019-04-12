#include"base.h"
#include"Shader.h"
class my_application : public sb7::application
{
public:
	void startup() {
		Shader oneShader("vertexShader.vs",
						"tessControlShader.tcs",
						"tessEvalShader.tes",
						"geometryShader.gs",
						"fragmentShader.fs");
		program = oneShader.ID;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		//here add the mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void render(double currentTime) {
		static const GLfloat back[] = { 0.2f,0.2f,0.2f,1.0f };
		glClearBufferfv(GL_COLOR, 0, back);
		glUseProgram(program);
		//here we draw patches
		glDrawArrays(GL_PATCHES, 0, 3);
	}
	virtual void shutdown()
	{
		glDeleteVertexArrays(1,&vao);
		glDeleteProgram(program);
	}

private:
	GLuint program;
	GLuint vao;

};

int main() {
	my_application* app = new my_application;
	app->run();
	delete app;
	return 0;
}


