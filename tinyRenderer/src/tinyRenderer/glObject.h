#pragma once
#include "glCommon.h"
#include <memory>
class glObject{
public:
    unsigned int VAO,VBO;
    glm::mat4 modelMatrix=glm::mat4(1.0f);
    virtual void loadGLData()=0;
    virtual void updateGLData()=0;
    virtual void removeGLData()=0;
    virtual void draw(Shader& shader)=0;
};