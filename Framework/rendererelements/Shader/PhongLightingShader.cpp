/****************************************************************************
|*  PhongLightingShader.cpp
|*
|*  Returns phong shaded color.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#include "PhongLightingShader.h"



PhongLightingShader::PhongLightingShader(void) {
	m_dC = 1; // constant
	m_dL = 0; // linear
	m_dQ = 0; // quadratic
}


PhongLightingShader::PhongLightingShader(double m_distC, double m_distL, double m_distQ) {
	m_dC = m_distC; // constant
	m_dL = m_distL; // linear
	m_dQ = m_distQ; // quadratic
}


PhongLightingShader::~PhongLightingShader(void) {
}


//Compute color of intersection point
Vector4 PhongLightingShader::shade(IntersectionData* iData, Scene* scene) {

  //===EXERCISE 2.1.3 ===
  Vector4 color_tmp(0,0,0);

  Vector4 Ka = iData->material->ambient;
  Vector4 Kd = iData->material->diffuse;
  Vector4 Ks = iData->material->specular;
  Vector3 N = iData->normal;
  
  Vector4 Ia = scene->getAmbient();

  // Ambient contribution formula
  // I = IaKa
  color_tmp += Ia.componentMul(Ka);

  std::vector<ILight*> lights = scene->getNonOccludedLights(iData->position);
  for(unsigned int i = 0; i < lights.size(); i++) {
    ILight *light = lights[i];

    Vector4 Ip = light->getColor();
    Vector3 L = (light->getPosition() - iData->position).normalize();
    double NdotL = N.dot(L);

    // Only apply diffuse contribution if N and L are pointing in the same direction
    if(NdotL >= 0.0) {
      // Formula to compute diffuse lighting contribution:
      // I = IpKd(N dot L)
      color_tmp += Ip.componentMul(Kd) * NdotL;

      Vector3 V = (iData->sourcePosition - iData->position).normalize();
      Vector3 R = ((N * NdotL * 2) - L).normalize();
      
      double VdotR = V.dot(R);
      double n = iData->material->shininess;

      // Only apply specular contribution if N and L are pointing
      // in the same direction and if V and R are pointing in
      // the same direction
      if(VdotR >= 0.0) {
        // Formula to compute diffuse lighting contribution:
        // I = IpKs(V dot R)^n
        color_tmp += Ip.componentMul(Ks) * pow(VdotR, n);
      }
    }
  }

	return color_tmp.clamp01();
}


