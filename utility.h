#ifndef UTILITY_H
#define UTILITY_H

#include <glad/glad.h>
#include <glm/glm.hpp>

GLfloat   Min        (GLfloat a, GLfloat b);
GLboolean IsBetween  (GLfloat p1, GLfloat p2, GLfloat q);
GLboolean IsNegative (GLfloat v);
GLfloat   Absolute   (GLfloat v);
GLfloat   GetAngle   (GLfloat dy, GLfloat dx);
GLfloat   GetAngle   (glm::vec2 v);
GLfloat   DistanceSq (glm::vec2 p1, glm::vec2 p2);
glm::vec2 UnitVector (glm::vec2 v);
glm::vec2 UnitVector (GLfloat angle);
GLfloat   DotProduct (glm::vec2 u, glm::vec2 v);

#endif
