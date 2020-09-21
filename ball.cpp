#include "ball.h"

#include <iostream>

const GLfloat RESTITUTION = 0.75f, DRAG_COEFF = 0.0025f;

GLfloat LaunchTime, CurrentTime;

Ball::Ball() : Entity(), Radius(0.0f), Velocity(0.0f, 0.0f), free(false), Angle(0.0f), TimeOfFlight(0.0f) { }

Ball::Ball(glm::vec2 pos, glm::vec2 size, GLfloat radius, Texture2D sprite) : Entity(pos, size, sprite), 
    Radius(radius), free(false), Velocity(0.0f, 0.0f), Angle(0.0f), TimeOfFlight(0.0f) { }

glm::vec2 Ball::Update(GLfloat dt, GLuint window_width, GLuint window_height)
{
    if(this->free)
    {
        this->Position += this->Velocity * dt;

        if(this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x * RESTITUTION;
            this->Position.x = 0.0f;
        }
        else if(this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x * RESTITUTION;
            this->Position.x = window_width - this->Size.x;
        }

        if(this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y * RESTITUTION;
            this->Position.y = 0.0f;
        }
        else if(this->Position.y + this->Size.y >= window_height)
        {
            this->Velocity.y = -this->Velocity.y * RESTITUTION;
            this->Position.y = window_height - this->Size.y;
        }

        this->Angle = GetAngle(this->Velocity.y, this->Velocity.x);
        
        CurrentTime = glfwGetTime();
        TimeOfFlight = CurrentTime - LaunchTime;
        
        this->Velocity -= this->Velocity * DRAG_COEFF;
    }

    return this->Position;
}


void Ball::Shoot()
{
    this->free = true;
    this->Velocity = glm::vec2(this->setSpeed * glm::cos(this->setAngle), setSpeed * glm::sin(this->setAngle));
    LaunchTime = glfwGetTime();
}

void Ball::setParams(GLfloat angle, GLfloat speed)
{
    this->setAngle = angle;
    this->setSpeed = speed;
}

void Ball::Reset(glm::vec2 position)
{
    this->Position = position;
    this->Velocity = glm::vec2(0.0f, 0.0f);
    TimeOfFlight = 0.0f;
    this->free = false;
}

void Ball::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Angle);
}
