#include "entity.h"

Entity::Entity() : Position(0,0), Size(100,100), Exists(GL_TRUE), Sprite() { }

Entity::Entity(glm::vec2 pos, glm::vec2 size, Texture2D sprite) : Position(pos), Size(size), Exists(GL_TRUE), Sprite(sprite) { }

void Entity::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size);
}
