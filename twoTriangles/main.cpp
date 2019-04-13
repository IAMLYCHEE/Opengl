#include"base.h"
#include"Shader.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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
		const GLuint NumVertice = 6;
		static const GLfloat vertices[NumVertice][2] = {
			{-0.9,-0.9},
			{0.85,-0.9},
			{-0.9,0.85},
			{0.9,-0.85},
			{0.90,0.90},
			{-0.85,0.90}
		};
		glCreateBuffers(NumBuffers, Buffers);
		glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);
		glGenVertexArrays(NumVAOs, VAOs);

		glBindVertexArray(VAOs[Triangles]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
		glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vPosition);

		glUseProgram(program);
	}

	void render(double currentTime) {
		static const GLfloat back[] = { 0.2f,0.2f,0.2f,1.0f };

		//glBindVertexArray(VAOs[Triangles]);
		glClearBufferfv(GL_COLOR, 0, back);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	virtual void shutdown()
	{
		glDeleteBuffers(NumBuffers, Buffers);
		glDeleteProgram(program);
		glDeleteVertexArrays(NumVAOs, VAOs);
	}

private:
	enum VAO_IDs{Triangles, NumVAOs};
	enum Buffer_IDs{ArrayBuffer,NumBuffers};
	enum Attrib_IDs{vPosition = 0};

	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];

	const GLuint NumVertices = 6;

	GLuint program;

};

int main() {
	my_application* app = new my_application;
	app->run();
	delete app;
	return 0;
}


