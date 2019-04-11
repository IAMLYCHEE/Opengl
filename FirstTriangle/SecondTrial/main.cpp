#include"base.h"
#include"Shader.h"
class my_application : public sb7::application
{
public:
	void startup() {

		Shader oneShader("vertexShader.vs", "fragmentShader.fs");
   		program = oneShader.ID;


		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void render(double currentTime) {
		static const GLfloat red[] = { 1.0f,0.0f,0.0f,1.0f };
		glClearBufferfv(GL_COLOR, 0, red);

		glUseProgram(program);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

private:
	GLuint          program;
	GLuint          vao;
};

int main() {
	my_application* app = new my_application;
	app->run();
	delete app;
	return 0;

}


