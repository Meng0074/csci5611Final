/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "resource_manager.h"
#include "gamesetup.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include <iostream>

SpriteRenderer  *Renderer;
GameObject      *Player;
GameObject      *en;

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
	delete Renderer;
    delete Player;
    delete en;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
    std::cout << "aa" << std::endl;
    ResourceManager::LoadTexture("textures/background.png", GL_FALSE, "background");
    ResourceManager::LoadTexture("textures/aa.jpg", GL_TRUE, "face");
    ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("textures/aa.jpg", true, "paddle");
    // Set render-specific controls
    Shader myshader;
    myshader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myshader);
    // Load levels
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Level = 0;
    // Configure game objects
    glm::vec2 playerPos = glm::vec2(600, 400);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    glm::vec2 enPos = glm::vec2(100, 400);
    en = new GameObject(enPos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
}

void Game::Update(GLfloat dt)
{
    if (Player->Position.y > 400) {
        Player->Position.y = 400;
        Player->Velocity.y = 0;
        Player->Velocity.x = 0;
    }
    else {
        Player->Velocity.y += 1000 * dt;
        Player->Position.y += Player->Velocity.y * dt;
    }
    if (Player->Position.x < 0) {
        Player->Position.x = 0;
        Player->Velocity.x = 0;
    }
    if (Player->Position.x > 800) {
        Player->Position.x = 800;
        Player->Velocity.x = 0;
    }
}


void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // Move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
                Player->Velocity.x = 0 - PLAYER_VELOCITY;
                Player->Position.x += Player->Velocity.x * dt;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Velocity.x = PLAYER_VELOCITY;
                Player->Position.x += Player->Velocity.x * dt;
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 400){
                Player->Velocity.y = 0 - 2 * PLAYER_VELOCITY;
                Player->Position.y += Player->Velocity.y * dt;
            }
            
            
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // Draw background
        Texture2D mytexture;
        mytexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(mytexture, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        //en->Draw(*Renderer);
    }
}