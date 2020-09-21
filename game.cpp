#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "ball.h"

#include <iostream>
#include <sstream>

// Game-related State data
SpriteRenderer *Renderer;
TextRenderer *Text;

const GLfloat BALL_RADIUS = 25.0f;
const glm::vec2 BALL_POS = glm::vec2(125.0f, 425.0f);
const glm::vec2 BALL_SIZE = glm::vec2(50.0f, 50.0f);
const GLfloat ARROW_WIDTH = 30.0f, SPEED_FACTOR = 1.0f, RESTITUTION = 0.75f;

const GLfloat TIME_LIMIT = 15.0f;

Ball *ball;

Game::Game(GLuint width, GLuint height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Mouse()
{ 

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadTexture("textures/background.png", GL_TRUE, "background");
    ResourceManager::LoadTexture("textures/break.png", GL_TRUE, "break");
    ResourceManager::LoadTexture("textures/notbreak.png", GL_TRUE, "notbreak");
    ResourceManager::LoadTexture("textures/ball.png", GL_TRUE, "ball");
    ResourceManager::LoadTexture("textures/arrow.png", GL_TRUE, "arrow");
    ResourceManager::LoadTexture("textures/grav.png", GL_TRUE, "grav");
    
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height),
        0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use();
    ResourceManager::GetShader("sprite").SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    Level one; one.Load("levels/one.lvl", this->Width, this->Height);
    Level two; two.Load("levels/two.lvl", this->Width, this->Height);
    Level three; three.Load("levels/three.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->level = 0;

    ball = new Ball(BALL_POS, BALL_SIZE, BALL_RADIUS, ResourceManager::GetTexture("ball"));
    ball->setParams(0.0f, 0.0f);

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/Atlanta Demi.ttf", 24);
}

void DoGravity(glm::vec2 g)
{
    glm::vec2 c = ball->Position + glm::vec2(ball->Radius, ball->Radius);
    glm::vec2 d = g-c;

    GLfloat theta = GetAngle(d), magna = glm::sqrt(DistanceSq(g, c));

    ball->Velocity += 5.0f*UnitVector(theta);
}

void Game::Update(GLfloat dt)
{
    if(this->State == GAME_ACTIVE)
    {
        for(auto &tile : this->Levels[this->level].Enchanted)
            if(tile.Exists)
                DoGravity(tile.Position + (tile.Size/2.0f));
        ball->Update(dt, this->Width, this->Height);
        this->Collisions();
    }
}


void Game::ProcessInput(GLfloat dt, GLFWwindow* window)
{
    if(!(ball->free))
        if(this->Keys[GLFW_KEY_SPACE])
            ball->Shoot();
}

void Game::Render(GLFWwindow* window)
{
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height));
    
    if(this->State == GAME_ACTIVE)
    {
        this->Levels[this->level].Draw(*Renderer);
        ball->Draw(*Renderer);
        
        if(!(ball->free))
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            GLfloat x = xpos, y = ypos;
            GLfloat cx = BALL_POS.x + BALL_RADIUS;
            GLfloat cy = BALL_POS.y + BALL_RADIUS;

            GLfloat theta = GetAngle(y-cy,x-cx);

            GLfloat d = DistanceSq(glm::vec2(x,y), glm::vec2(cx,cy));
            d = glm::sqrt(d);
            
            Renderer->DrawSprite(ResourceManager::GetTexture("arrow"), glm::vec2(cx - d, cy - ARROW_WIDTH/2), 
                glm::vec2(d*2, ARROW_WIDTH), theta);
            ball->setParams(theta, d/SPEED_FACTOR);

            std::stringstream angle; angle << int(glm::degrees(theta));
            std::stringstream speed; speed << int(d/SPEED_FACTOR);
            Text->RenderText("Angle: ", 125.0f, 130.0f, 1.0f);
            Text->RenderText("Speed: ", 125.0f, 170.0f, 1.0f);
            Text->RenderText(angle.str(), 215.0f, 130.0f, 1.0f);
            Text->RenderText(speed.str(), 215.0f, 170.0f, 1.0f);
        }
        
        std::stringstream cleared, total;
        cleared << this->Levels[this->level].Cleared;
        total << this->Levels[this->level].Remaining + this->Levels[this->level].Cleared;

        Text->RenderText(cleared.str() + "/" + total.str() + " cleared", 1325.0f, 130.0f, 1.0f);

        std::stringstream tries;
        tries << this->Levels[this->level].Tries;

        Text->RenderText(tries.str() + "/3 tries left", 1325.0f, 170.f, 1.0f);
        
        std::stringstream time_rem;
        time_rem << (int)(TIME_LIMIT - ball->TimeOfFlight);
        
        Text->RenderText(time_rem.str(), 1525.0f, 25.0f, 2.0f);
    }
    else if(this->State == GAME_LOST)
    {
        Text->RenderText("YOU LOST", 725.0f, 425.0f, 2.0f);
    }
    else if(this->State == GAME_WIN)
    {
        Text->RenderText("YOU WIN", 725.0f, 425.0f, 2.0f);
    }
}

GLboolean CheckCollision(Entity block, GLboolean reflect = GL_FALSE);

void Game::Collisions()
{
    for(Entity &block : this->Levels[this->level].Destructible)
        if(block.Exists)
            if(CheckCollision(block))
                block.Exists = GL_FALSE;
    for(Entity &block : this->Levels[this->level].Enchanted)
        CheckCollision(block, GL_TRUE);
}

GLboolean CheckCollision(Entity block, GLboolean reflect)
{
    GLfloat cr = ball->Radius;
    GLfloat cx = ball->Position.x + cr, cy = ball->Position.y + cr;

    GLfloat r1 = block.Position.x, r2 = r1 + block.Size.x;
    GLfloat c1 = block.Position.y, c2 = c1 + block.Size.y;
    
    if(IsBetween(r1, r2, cx))
    {
        if(Absolute(c1-cy) <= cr)
        {
            if(reflect)
            {
                ball->Velocity.y = -ball->Velocity.y * RESTITUTION;
                ball->Position.y = c1 - ball->Size.y;
            }
            return GL_TRUE;
        }
        if(Absolute(c2-cy) <= cr)
        {
            if(reflect)
            {
                ball->Velocity.y = -ball->Velocity.y * RESTITUTION;
                ball->Position.y = c2;
            }
            return GL_TRUE;
        }
    }
    if(IsBetween(c1, c2, cy))
    {
        if(Absolute(r1-cx) <= cr)
        {
            if(reflect)
            {
                ball->Velocity.x = -ball->Velocity.x * RESTITUTION;
                ball->Position.x = r1 - ball->Size.x;
            }
            return GL_TRUE;
        }
        if(Absolute(r2-cx) <= cr)
        {
            if(reflect)
            {
                ball->Velocity.x = -ball->Velocity.x * RESTITUTION;
                ball->Position.x = r2;
            }
            return GL_TRUE;
        }
    }

    glm::vec2 Points[] = { glm::vec2(r1,c1), glm::vec2(r1, c2), glm::vec2(r2,c1), glm::vec2(r2,c2) };
    glm::vec2 c = glm::vec2(cx, cy);

    for(auto p : Points)
    {
        if(DistanceSq(c, p) <= cr*cr)
        {
            if(!reflect)
                return GL_TRUE;

            GLfloat theta = GetAngle(p-c);

            ball->Position = p - (cr * UnitVector(theta)) - glm::vec2(cr, cr);

            GLfloat magnitude = DotProduct(ball->Velocity, UnitVector(theta));
            glm::vec2 reflection = magnitude * UnitVector(theta);

            ball->Velocity -= 2.0f * reflection * RESTITUTION;

            return GL_TRUE;
        }
    }

    return GL_FALSE;
}

GLboolean Game::CheckLevelFail()
{
    if(ball->TimeOfFlight > TIME_LIMIT)
        return GL_TRUE;
    return GL_FALSE;
}
GLboolean Game::CheckLevelDone()
{
    if(this->Levels[this->level].Remaining == 0)
        return GL_TRUE;
    return GL_FALSE;
}

void Game::NextLevel()
{
    (this->level)++;

    if(this->level >= this->Levels.size())
        GameFinish();

    ResetBall();
}
void Game::Retry()
{
    this->Levels[this->level].Tries--;
    if(this->Levels[this->level].Tries == 0)
        GameOver();
    else
        ResetBall();
}
void Game::ResetBall()
{
    ball->Reset(BALL_POS);
}
void Game::GameOver()
{
    this->State = GAME_LOST;
}
void Game::GameFinish()
{
    this->State = GAME_WIN;
}
