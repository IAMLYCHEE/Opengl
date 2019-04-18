#include"base.h"
#include"Shader.h"
#include"loadObj.h"
#include"Texture.h"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

class my_application : public sb7::application
{
public:
	void init() {
		static std::string title = "OpenGL indexBuffer";
		sb7::application::init();
		info.title = title;
		info.windowHeight = 900;
		info.windowWidth = 900;
		
	}

	void startup() {
		oneShader = Shader("vertexShader.vs",
						"tessControlShader.tcs",
						"tessEvalShader.tes",
						"geometryShader.gs",
						"fragmentShader.fs");
		program = oneShader.ID;
		//use the program
		glUseProgram(program);
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//=================datas=======================//
		//vertex positions 
		float positions[] = {
			-0.5f, -0.5f, 0.0f,1.0f,// 0
			 0.5f, -0.5f, 0.0f,1.0f,// 1
			 0.5f,  0.5f, 0.0f,1.0f,// 2
			-0.5f,  0.5f,0.0f,1.0f  // 3
		};

		float tex_coordinate[] = {
			0.0f,0.0f, //0
			1.0f,0.0f, //1
			1.0f,1.0f, //2
			0.0f,1.0f  //3
		};

		//vertex indices
		unsigned int indices[] = {
			  0, 1, 2,
			  2, 3, 0
		};
		numPositionData = 16;
		numTexCoord = 8;
		numIndicesData = 6;
		numCompoAttri0 = 4;
		numCompoAttri1 = 2;

		glCreateBuffers(1, &vbo0);
		//select theis buffer,declare its usage
		glBindBuffer(GL_ARRAY_BUFFER, vbo0);
		//put data into the buffer 
		glBufferData(GL_ARRAY_BUFFER,
			numPositionData * sizeof(float),
			positions,
			GL_STATIC_DRAW);
		//define the layout of the data
		glVertexAttribPointer(0,                 //Attribute position
			numCompoAttri0,                 //number of components per attribute
			GL_FLOAT,          //type of the attribute data
			GL_FALSE,          //nomalized or not
			numCompoAttri0 * sizeof(float), //stride
			0);                //where exactly start the data
//enable this vertex attribute
		glEnableVertexAttribArray(0);            //attribute position


		//create an array buffer to store the texture coordinates
		glCreateBuffers(1, &vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferData(GL_ARRAY_BUFFER,
			numTexCoord * sizeof(float),
			tex_coordinate,
			GL_STATIC_DRAW);
		glVertexAttribPointer(1,
			numCompoAttri1,
			GL_FLOAT,
			GL_FALSE,
			numCompoAttri1 * sizeof(float),
			0);
		glEnableVertexAttribArray(1);


		//now the index buffer
		glCreateBuffers(1, &ibo);
		//select the buffer and declare the usage
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		//put the data into the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,   //declare the type of the buffer
			numIndicesData * sizeof(unsigned int),   //declare the size of the buffer
			indices,                    //put the data into the buffer
			GL_STATIC_DRAW);            //how to use the data


		Texture texture = Texture("opengl.jpg");
		texture.bind(0);
		oneShader.setInt("u_texture", 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		transVec = glm::vec3(0.0f, 0.0f, 0.0f);
		angle_y = 1.0f;
		scale = 1.0f;
	}

	void render(double currentTime) {
		glClear(GL_COLOR_BUFFER_BIT);

		//because we are specifying drawing triangles, so the opengl knows
		//that every three indices form a triangle and it will link every three 
		//indices together . so we do not need to specify the lay out of
		//the indices
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), transVec);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::radians(angle_y),glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0));
		glm::mat4 model =  rotation*translation*scaleMat;
		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);
		glm::vec3 trvec = glm::vec3(-0.05f, 0, 0);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), trvec);
		mvp = proj * view * model;
		oneShader.setUniformMat4f("u_mvp", mvp);
		ImGui::SliderFloat3("Translation", &transVec.x, 0.0f, 1.0f);
		ImGui::SliderFloat("rotation", &angle_y, 0.0f, 20000.0f);
		ImGui::SliderFloat("scale", &scale, 0.5f, 5.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		glDrawElements(GL_TRIANGLES,numIndicesData , GL_UNSIGNED_INT, nullptr);
	}
	virtual void shutdown()
	{
		//delete all the buffer we just created
		glDeleteBuffers(1, &vbo0);
		glDeleteBuffers(1, &vbo1);
		glDeleteBuffers(1, &ibo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

private:
	GLuint program = 0;
	GLuint vao = 0;
	GLuint vbo0 = 0;
	GLuint vbo1 = 0;
	GLuint ibo = 0;
	int numPositionData = 0;
	int numTexCoord = 0;
	int numIndicesData = 0;
	int numCompoAttri0 = 0;
	int numCompoAttri1 = 0;
	glm::mat4 mvp ;
	glm::vec3 transVec;
	Shader oneShader;
	float angle_y;
	float scale;
};

int main() {
	my_application* app = new my_application;
	app->run();
	delete app;
	return 0;
}


