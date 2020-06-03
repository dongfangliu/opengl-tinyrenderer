#pragma once
#include "glCommon.h"
#include <Eigen/Dense>
class glLight {
    public:
    Eigen::Vector3f position=Eigen::Vector3f(1.2,1.0,2.0);

    Eigen::Vector3f ambient=Eigen::Vector3f(0.1,0.1,0.1);
    Eigen::Vector3f diffuse=Eigen::Vector3f(1.0,1.0,1.0);
    Eigen::Vector3f specular = Eigen::Vector3f(1.0, 1.0, 1.0);
};