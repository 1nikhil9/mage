#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "entity.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

class Level
{
public:
    std::vector<Entity> Destructible, Enchanted;
    GLuint Cleared, Remaining, Tries;

    Level() { }

    void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
    void Draw(SpriteRenderer &renderer);
private:
    void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif
