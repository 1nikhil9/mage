#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

class Entity
{
public:
    glm::vec2  Position, Size;
    
    GLboolean Exists;
    
    Texture2D Sprite;
    
    Entity();
    Entity(glm::vec2 pos, glm::vec2 size, Texture2D sprite);

    virtual void Draw(SpriteRenderer &renderer);
};

#endif
