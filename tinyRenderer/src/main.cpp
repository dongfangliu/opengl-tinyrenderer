#include "./tinyRenderer/tinyRenderer.h"

int main()
{
    tinyRenderer renderer(1024,768);
    renderer.addCube();
    renderer.setCamera(Eigen::Vector3f(3,3,3),Eigen::Vector3f(0,0,0),Eigen::Vector3f(0,1,0));
    while(true){
    renderer.renderSingleFrame();
    }
}