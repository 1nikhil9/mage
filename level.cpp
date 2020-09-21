#include "level.h"

#include <fstream>
#include <sstream>

void Level::Load(const GLchar *file,  GLuint levelWidth, GLuint levelHeight)
{
    this->Destructible.clear();
    this->Enchanted.clear();

    GLuint tileCode;

    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint>> tileData;

    if(fstream)
    {
        while(std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;

            while(sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if(tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void Level::Draw(SpriteRenderer &renderer)
{
    for(Entity &tile : this->Enchanted)
        if(tile.Exists)
            tile.Draw(renderer);

    GLuint drawn = 0;
    for(Entity &tile : this->Destructible)
        if(tile.Exists)
        {
            tile.Draw(renderer);
            drawn++;
        }

    Cleared = this->Destructible.size() - drawn;
    Remaining = drawn;
}

void Level::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
    this->Tries = 3;

    GLuint height = tileData.size();
    GLuint width = tileData[0].size();

    GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / static_cast<GLfloat>(height);

    for(GLuint y = 0; y < height; ++y)
    {
        for(GLuint x = 0; x < width; ++x)
        {
            glm::vec2 pos(unit_width * x + 1, unit_height * y + 1);
            glm::vec2 size(unit_width, unit_height);
            
            if(tileData[y][x] == 1)
            {
                Entity obj(pos, size, ResourceManager::GetTexture("break"));
                this->Destructible.push_back(obj);
            }
            else if(tileData[y][x] == 2)
            {
                Entity obj(pos, size, ResourceManager::GetTexture("notbreak"));
                this->Enchanted.push_back(obj);
            }
        }
    }
}
