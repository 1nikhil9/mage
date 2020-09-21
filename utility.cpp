#include "utility.h"

GLfloat Min(GLfloat a, GLfloat b)
{
    if(a < b)
        return a;
    return b;
}

GLboolean IsBetween(GLfloat p1, GLfloat p2, GLfloat q)
{
    if(p1 <= q and q <= p2)
        return GL_TRUE;
    return GL_FALSE;
}

GLboolean IsNegative(GLfloat v)
{
    if(v < 0.0f)
        return GL_TRUE;
    return GL_FALSE;
}

GLfloat Absolute(GLfloat v)
{
    if(IsNegative(v))
        return -v;
    return v;
}

GLfloat GetAngle(GLfloat dy, GLfloat dx)
{
    GLfloat angle = atan(Absolute(dy)/Absolute(dx));
    
    if(IsNegative(dy) and !IsNegative(dx))
        angle = -angle;
    else if(IsNegative(dy) and IsNegative(dx))
        angle -= glm::radians(180.0f);
    else if(!IsNegative(dy) and IsNegative(dx))
    {
        angle = -angle;
        angle += glm::radians(180.0f);
    }

    return angle;
}

GLfloat GetAngle(glm::vec2 v)
{
    return GetAngle(v.y, v.x);
}

GLfloat DistanceSq(glm::vec2 p1, glm::vec2 p2)
{
    GLfloat dx = (p1.x - p2.x);
    GLfloat dy = (p1.y - p2.y);

    return (dx*dx)+(dy*dy);
}

glm::vec2 UnitVector(glm::vec2 v)
{
    return (v / glm::length(v) );
}

glm::vec2 UnitVector(GLfloat angle)
{
    return glm::vec2( glm::cos(angle), glm::sin(angle) );
}

GLfloat DotProduct(glm::vec2 u, glm::vec2 v)
{
    return u.x*v.x + u.y*v.y;
}
