#pragma once
#include "glCommon.h"
#include "glLight.h"
#include "glObject.h"
#include "glCube.h"
#include <learnopengl/camera.h>
#include <vector>
#include <memory>
#include <Eigen/Dense> 
class tinyRenderer{
    int width = 1024, height = 768;
    GLFWwindow *window=NULL;
    std::vector<glObject *> objects;
    Camera cam;
    std::shared_ptr<Shader> defaultShader=NULL;
     glm::mat4 projection;
     bool glInited = false;
    int initGL();  
    void unloadGL();
    void initDefaultShader();void updateShaders();
    public:
    tinyRenderer(int width, int height,float zNear = 0.1f,float zFar = 100.0f );
    ~tinyRenderer();
    glLight light;
    void renderSingleFrame();
    void savePPM(std::string filename);
    void setCamera(Eigen::Vector3f position,Eigen::Vector3f target,Eigen::Vector3f up);
    void setLight(Eigen::Vector3f position,Eigen::Vector3f diffuse= Eigen::Vector3f::Ones(),Eigen::Vector3f specular= Eigen::Vector3f::Ones());
    void addCube(){this->objects.push_back((glObject*)new glCube());};
};