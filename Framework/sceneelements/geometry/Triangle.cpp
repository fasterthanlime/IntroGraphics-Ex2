/****************************************************************************
|*  Triangle.cpp
|*
|*  Defines a triangle in space with normals defined at the points.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#include "Triangle.h"
#include <utils/Matrix4.h>


Triangle::Triangle(Material* material) {

	// creating a default triangle
	m_p1 = Vector3(1.0, 0.0, 0.0);
	m_p2 = Vector3(0.0, 1.0, 0.0);
	m_p3 = Vector3(1.0, 1.0, 0.0);

	m_n1 = Vector3(0.0, 0.0, 1.0);
	m_n2 = Vector3(0.0, 0.0, 1.0);
	m_n3 = Vector3(0.0, 0.0, 1.0);

	m_material = material;

	m_finite = true;
}


Triangle::~Triangle(void) {
}


void Triangle::setNormals(Vector3 n1, Vector3 n2, Vector3 n3) {
	m_n1 = n1.normalize();
	m_n2 = n2.normalize();
	m_n3 = n3.normalize();
}


//intersect triangle with a ray
bool Triangle::intersect(const Ray &ray, IntersectionData* iData) {

  //=== EXERCISE 1.3.3 ===
  /* Solution from Moeller and Trumbore: Fast, minimum storage ray-triangle intersection 
  http://www.graphics.cornell.edu/pubs/1997/MT97.html */

  double EPSILON = 10e-10;
  double u,v;
  Vector3 qvec;


  /* find vectors for two edges sharing vert0 */
  Vector3 e1 = m_p2-m_p1;
  Vector3 e2 = m_p3-m_p1;

  /* begin calculating determinant - also used to calculate U parameter */
  Vector3 pvec = ray.direction.cross(e2);

  /* if determinant is near zero, ray lies in plane of triangle */
  double det = e1.dot(pvec);

  if (det > EPSILON)
  {
    /* calculate distance from vert0 to ray origin */
    Vector3 tvec=ray.point-m_p1;

    /* calculate U parameter and test bounds */
    u = tvec.dot(pvec);
    if (u < 0.0 || u > det)
      return false;

    /* prepare to testV parameter */
    qvec=tvec.cross(e1);

    /* calculate V parameter and test bounds */
    v = ray.direction.dot(qvec);
    if (v < 0.0 || u + v > det)
      return false;

  }
  else if(det < -EPSILON)
  {
    /* calculate distance from vert0 to ray origin */
    Vector3 tvec= ray.point-m_p1;

    /* calculate U parameter and test bounds */
    u = tvec.dot(pvec);
    if( u > 0.0 || u < det )
      return false;

    /* prepare to test V parameter */
    qvec = tvec.cross(e1);

    /* calculate V parameter and test bounds */
    v = ray.direction.dot(qvec);
    if (v > 0.0 || u + v < det )
      return false;

  }
  else return false;  /* ray is parallel to the plane of the triangle */


  double inv_det = 1.0f / det;

  /* calculate t, ray intersects triangle */
  double t =e2.dot(qvec) * inv_det;
  u *= inv_det;
  v *= inv_det;

  if ( (t>=ray.min_t && t<=ray.max_t) && t<iData->t )
  {
    iData->color=getColor();
    iData->t=t;
    iData->position=ray.getPointOnRay(iData->t);

    //===EXERCISE 2.1.1 ===

    return true;
  }

  return false;



}


Vector2 Triangle::minNmax(double a, double b, double c) {
	double tmp[3];
	tmp[0] = a;
	tmp[1] = b;
	tmp[2] = c;
	
	double m;

	for (int j=0; j<2; j++) {
		for (int i=0; i<(2-j); i++) {
			if (tmp[i] > tmp[i+1]) {
				m = tmp[i+1];
				tmp[i+1] = tmp[i];
				tmp[i] = m;
			}
		}
	}
	return Vector2(tmp[0], tmp[2]);
}



// setter & getter

void Triangle::setPoint1(Vector3 point1) {
	m_p1 = point1;

}
Vector3 Triangle::getPoint1(void) const {
	return m_p1;
}

void Triangle::setPoint2(Vector3 point2) {
	m_p2 = point2;

}
Vector3 Triangle::getPoint2(void) const {
	return m_p2;
}

void Triangle::setPoint3(Vector3 point3) {
	m_p3 = point3;

}
Vector3 Triangle::getPoint3(void) const {
	return m_p3;
}


void Triangle::setNormal1(Vector3 normal1) {
	m_n1 = normal1.normalize();
}
Vector3 Triangle::getNormal1(void) const {
	return m_n1;
}

void Triangle::setNormal2(Vector3 normal2) {
	m_n2 = normal2.normalize();
}
Vector3 Triangle::getNormal2(void) const {
	return m_n2;
}
void Triangle::setNormal3(Vector3 normal3) {
	m_n3 = normal3.normalize();
}
Vector3 Triangle::getNormal3(void) const {
	return m_n3;
}

AABB Triangle::getBB() const {
	//initialize corners of BB with p1
	Vector3 lower = m_p1, upper = m_p1;

	//check components of p2
	for (int i = 0; i < 3; i++) {
		if (lower[i] > m_p2[i])
			lower[i] = m_p2[i];

		if (upper[i] < m_p2[i])
			upper[i] = m_p2[i];
	}

	//check components of p3
	for (int i = 0; i < 3; i++) {
		if (lower[i] > m_p3[i])
			lower[i] = m_p3[i];

		if (upper[i] < m_p3[i])
			upper[i] = m_p3[i];
	}

	return AABB(lower, upper);
}