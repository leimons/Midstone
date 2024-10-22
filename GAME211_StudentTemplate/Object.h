#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

//project includes
#include "Camera.h"
#include "GameManager.h"
#include "DataCollection.h"

class Object
{
private:
	SDL_Texture* TEX_object; //to start simple, only a single sprite, no animation right now
	SDL_Surface* SPR_object;
	SDL_Renderer* sceneRenderer;

	//initiate the variables of the sprite to be 16x16 pixels and scale of 1
	int SPR_width = 16;
	int SPR_height = 16;
	int SPR_scale = 1;

	Vec3 position = Vec3(0,0,0); //initiate position to (0,0,0) -> x = 0, y = 0, layer, z = 0
	Vec3 velocity = Vec3(0.0f, 0.0f, 0.0f);
	float speed = 3.0f; //3 pixel per movement
	std::string filePath;


public:
	Object(std::string filePath_, SDL_Renderer* sceneRenderer_);
	~Object();

	bool OnCreate();
	bool OnDestroy();
	void HandleEvents(const SDL_Event& sdlEvent);
	void Update(float deltaTime);
	void Render(SDL_Renderer* windowRender);
	Vec3 getPosition() { return position; }
	void setPosition(Vec3 newPosition) { position.x = newPosition.x; position.y = newPosition.y; position.z = newPosition.z; }

};

