/****************************************************************************
|*  DiffuseLightingShader.cpp
|*
|*  DiffuseLightingShader implements a shader that computes the color using the 
|*  diffuse part of the lighting equation.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#include "DiffuseLightingShader.h"


DiffuseLightingShader::DiffuseLightingShader(void) {
}


DiffuseLightingShader::~DiffuseLightingShader(void) {
}


//Compute color of intersection point
Vector4 DiffuseLightingShader::shade(IntersectionData* iData, Scene * scene) {

  //=== EXERCISE 2.1.2 ===
  Vector4 color_tmp(0,0,0);

  Vector4 Kd = iData->material->diffuse;
  Vector3 N = iData->normal;

  std::vector<ILight*> lights = scene->getLights();
  for(unsigned int i = 0; i < lights.size(); i++) {
    ILight *light = lights[i];

    Vector4 Ip = light->getColor();
    Vector3 L = (light->getPosition() - iData->position).normalize();

    double NdotL = N.dot(L);
    // Only apply diffuse contribution if N and L are not pointing in opposite directions
    if(NdotL >= 0.0) {
      // Formula to compute diffuse lighting contribution:
      // I = IpKd(N dot L)
      color_tmp += Ip.componentMul(Kd) * NdotL;
    }
  }
	
  return color_tmp.clamp01();
}


