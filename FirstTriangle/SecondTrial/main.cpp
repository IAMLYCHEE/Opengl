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
	}

	void render(double currentTime) {
		static const GLfloat back[] = { 0.2,0.2,0.2,1.0 };
		glClearBufferfv(GL_COLOR, 0, back);
		glUseProgram(program);

		//here to add the attribute value and pass it to the shader
		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
							(float)cos(currentTime) * 0.5f,
							0.0f,0.0f };
		glVertexAttrib4fv(0, attrib);

		//--------------------------------------------------------//
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
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


