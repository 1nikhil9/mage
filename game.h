#ifndef GAME_H
#define GAME_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "level.h"
#include "utility.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_LOST,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;	
    GLboolean              Keys[1024];
    GLboolean              Mouse[1024];
    GLuint                 Width, Height;
    std::vector<Level>     Levels;
    GLuint                 level;
    
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt, GLFWwindow* window);
    void Update(GLfloat dt);
    void Render(GLFWwindow* window);
    void Collisions();
    GLboolean CheckLevelDone();
    GLboolean CheckLevelFail();
    void NextLevel();
    void Retry();
    void ResetBall();
    void GameOver();
    void GameFinish();
};

#endif
