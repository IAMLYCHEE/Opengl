#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SkeletonMesh.h"
#include "RenderPart/background.h"
#include "ARpart/FindPoseARUCO.h"
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "AssetsPart/AssetsPrograms.h"
#include "RenderPart/RigidModel.h"
#include <math.h>

using namespace glm;

const int imageWidth = 640;
const int imageHeight = 480;

int InitCam(cv::VideoCapture& cap) {
	cap = cv::VideoCapture(0);

	if (cap.isOpened()) {
		std::cout << "width:" << cap.get(CV_CAP_PROP_FRAME_WIDTH) <<
			"height:" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) <<
			"nframes" << cap.get(CV_CAP_PROP_FRAME_COUNT);

		cap.set(CV_CAP_PROP_FRAME_WIDTH, imageWidth);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, imageHeight);
		cap.set(CV_CAP_PROP_FPS, 30);

		std::cout << "width:" << cap.get(CV_CAP_PROP_FRAME_WIDTH) <<
			"height:" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) <<
			"nframes" << cap.get(CV_CAP_PROP_FRAME_COUNT);
		return 1;
	}
	else {
		std::cout << "camera init failure" << std::endl;
		return 0;
	}
}

GLFWwindow* window;

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	cv::VideoCapture cap;
	int initCap = InitCam(cap);
	if (!initCap) {
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(imageWidth, imageHeight, "Playground", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//=========================init assets folder===================//
	AssetsPrograms* mAssetsPrograms = new AssetsPrograms();
	mAssetsPrograms->programsInit();
	mAssetsPrograms->texturesInit();



	//=========================models================================//
	//skeleton man
	SkeletonMesh* mySkeletonMesh = new SkeletonMesh;
	mySkeletonMesh->initAnimatedMan("fbxModels/Skeleton/gedou.fbx");

	//dragon
	RigidModel* dragon = new RigidModel;
	dragon->init("fbxModels/Rigid/dragon.fbx", "fbx");

	//glasses
	RigidModel* glasses = new RigidModel;
	glasses->init("fbxModels/Rigid/glasses.fbx", "fbx");

	//solar system
	RigidModel* sphere[10];
	for (int i = 0; i < 10; i++) {
		sphere[i] = new RigidModel;
		sphere[i]->init("fbxModels/Rigid/sphere.fbx", "fbx");
	}
	//=========================background===========================//
	Background* background = new Background;
	background->init();
	
	int width = imageWidth;
	int height = imageHeight;
	//glViewport(0, 0, width, height);

	float currentTime = 1.0f;
	//background
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);


	//scene info
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 7, 0.025, 0.0065, dictionary);

	float currentDegree = 0.0f;

	std::vector<float> orbitRadius = { 0.0f,8.0f,16.0f,24.0f,32.0f,40.0f,48.0f,56.0f,64.0f };
	std::vector<float> planetRadius = { 4.0f,1.0f,1.50f,2.0f,2.5f,3.0f,3.5f,3.75f,3.0f };
	std::vector<float> thetaSelf = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	std::vector<float> thetaPublic = {0.0f, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	std::vector<float> velocitySelf = {0.2f, 0.2f,0.3f,0.4f,0.3f,0.1f,0.5f,0.9f,0.7f };
	std::vector<float> velocityPublic = { 0.02f,0.02f,0.03f,0.04f,0.03f,0.01f,0.05f,0.025f,0.035f };


	do {
		for (int i = 0; i < 9; i++) {
			thetaSelf[i] += velocitySelf[i];
			thetaPublic[i] += velocityPublic[i];
		}

		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		//======================draw background====================//
		//cv::Mat image;
		//cap >> image;
		glm::mat4 proj;
		glm::mat4 view;
		glm::vec3 cameraPosition;
		//int mode = 0; //single marker


		////get camera Pose from aruco
		//int findPose = FindPoseARUCO::getMatrix(image, proj, view, board, dictionary,mode);
		//background->render(image);

		//=======================draw object======================//
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);

		int findPose = 0;
		//scene setting
		if (!findPose) {
			cameraPosition = glm::vec3(-56.229, 49.053, 59.461);
			glm::vec3 cameraRotation = glm::vec3(-30.938, -43.4, 0);
			view = cameraViewMatrix(cameraPosition, cameraRotation);
			proj = glm::perspective(45.0f, (float)width / (float)height, 1.0f, 1000.0f);
		}
		else {
			glm::mat4 inverseView = glm::inverse(view);
			cameraPosition = glm::vec3(inverseView[3][0], inverseView[3][1], inverseView[3][2]);
		}
		////dragon
		//GLuint dragonProgram;
		//int shaderMode = 2;


		//switch (shaderMode) {
		//case 0:
		//	dragonProgram = mAssetsPrograms->getProgramId("Basic");
		//	glUseProgram(dragonProgram);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("White"));
		//	break;
		//case 1:
		//	dragonProgram = mAssetsPrograms->getProgramId("Diffuse");
		//	glUseProgram(dragonProgram);

		//	break;
		//case 2:
		//	dragonProgram = mAssetsPrograms->getProgramId("SkyboxReflect");
		//	glUseProgram(dragonProgram);
		//	glm::vec3 materialColor = glm::vec3(0.5f, 0.5f, 0.5f);
		//	float reflectColor = 0.85f;
		//	setUniformVec3(dragonProgram, "CameraPosition", cameraPosition);
		//	setUniformVec3(dragonProgram, "MaterialColor", materialColor);
		//	setUniformFloat(dragonProgram, "ReflectFactor", reflectColor);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, mAssetsPrograms->getTextureId("CubePisa"));
		//	break;
		//}

		//glm::vec3 dragonTranslation = glm::vec3(-2.45, 0, 38.428);
		//currentDegree += 0.2;
		//if (currentDegree >= 360.0) {
		//	currentDegree = 0.0f;
		//}
		//glm::vec3 dragonRotation = glm::vec3(0, currentDegree, 0);
		//glm::vec3 dragonScale = glm::vec3(1.0, 1.0, 1.0);
		//glm::mat4 dragonModel = modelMatrix(dragonTranslation, dragonRotation, dragonScale);
		//dragon->render(view, proj, dragonModel, dragonProgram, 0);

		////glasses
		//GLuint glassFrameProgram = mAssetsPrograms->getProgramId("Basic");
		//glUseProgram(glassFrameProgram);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("glassFrame"));
		//glm::vec3 glassesTranslation = glm::vec3(0, 0, 0);
		//glm::vec3 glassesRotation = glm::vec3(0, currentDegree, 0);
		//glm::vec3 glassesScale = glm::vec3(0.31, 0.31, 0.31);
		//glm::mat4 glassesModel = modelMatrix(glassesTranslation, glassesRotation, glassesScale);
		//glasses->render(view, proj, glassesModel, glassFrameProgram, 0);

		//GLuint glassesEyeProgram = mAssetsPrograms->getProgramId("SkyboxReflect");
		//glUseProgram(glassesEyeProgram);
		//glm::vec3 materialColor = glm::vec3(0.5f, 0.5f, 0.5f);
		//float reflectColor = 0.85f;
		//setUniformVec3(glassesEyeProgram, "CameraPosition", cameraPosition);
		//setUniformVec3(glassesEyeProgram, "MaterialColor", materialColor);
		//setUniformFloat(glassesEyeProgram, "ReflectFactor", reflectColor);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, mAssetsPrograms->getTextureId("CubePisa"));
		//glasses->render(view, proj, glassesModel, glassesEyeProgram, 1);

		//GLuint glassesLegProgram = mAssetsPrograms->getProgramId("Basic");
		//glUseProgram(glassesLegProgram);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("glassLeg"));
		//glasses->render(view, proj, glassesModel, glassesLegProgram, 2);

		//solar
		for (int i = 0; i < 9; i++) {
			GLuint program = mAssetsPrograms->getProgramId("Basic");
			glUseProgram(program);
			glActiveTexture(GL_TEXTURE0);
			switch (i) {
			case 0:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("sun"));
				break;
			case 1:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("mercury"));
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("venus"));
				break;
			case 3:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("earth"));
				break;
			case 4:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("mars"));
				break;
			case 5:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("jupiter"));
				break;
			case 6:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("saturn"));
				break;
			case 7:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("uranus"));
				break;
			case 8:
				glBindTexture(GL_TEXTURE_2D, mAssetsPrograms->getTextureId("neptune"));
				break;
			}
			glm::vec3 sphereTranslation = glm::vec3(orbitRadius[i]*cos(thetaPublic[i]), 0, orbitRadius[i] * sin(thetaPublic[i]));
			glm::vec3 sphereRotation = glm::vec3(0, thetaSelf[i], 0);
			glm::vec3 sphereScale = glm::vec3(planetRadius[i], planetRadius[i], planetRadius[i]);
			glm::mat4 sphereModel = modelMatrix(sphereTranslation, sphereRotation, sphereScale);
			sphere[i]->render(view, proj, sphereModel, program, 0);
		}


		//==============================================================//
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}