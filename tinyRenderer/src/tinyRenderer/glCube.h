#include "glObject.h"
class glCube : public glObject
{

    void loadGLData();
    void updateGLData();
    void removeGLData();
    void draw(Shader& shader);
};