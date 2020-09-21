#ifndef BALL_H
#define BALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "entity.h"
#include "utility.h"

class Ball : public Entity
{
public:
    GLfloat Radius;
    glm::vec2 Velocity;
    
    GLboolean free;
    GLfloat setAngle;
    GLfloat setSpeed;

    GLfloat Angle;
    GLfloat TimeOfFlight;

    Ball();
    Ball(glm::vec2 pos, glm::vec2 size, GLfloat radius, Texture2D sprite);
    
    void setParams(GLfloat angle, GLfloat speed);
    void Shoot();
    
    glm::vec2 Update(GLfloat dt, GLuint window_width, GLuint window_height);

    void Reset(glm::vec2 position);

    virtual void Draw(SpriteRenderer &renderer);
};

#endif
