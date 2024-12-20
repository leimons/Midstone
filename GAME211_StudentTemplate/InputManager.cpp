#include "InputManager.h"

void InputManager::cleanKeyBoard()
{
	int i;
	for (i = 0; i < N_KEYS; i++) {
		//keyDown[i] = false;
		keyUp[i] = false;
	}
}

void InputManager::Update()
{
	//no inputs available!
	if (lockInput)
		return;

    SDL_Event event;
	cleanKeyBoard();
	//SDL_PumpEvents();
	//keyboard = (SDL_GetKeyboardState(NULL));

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym > 320)
				break;

			keyDown[event.key.keysym.sym] = true;
			keyUp[event.key.keysym.sym] = false;
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym > 320)
				break;

			keyUp[event.key.keysym.sym] = true;
			keyDown[event.key.keysym.sym] = false;
			break;
		case SDL_QUIT:
			EventHandler::GetInstance()->Broadcast(QuitEvent());
			quitGame = true;
			break;
		}
	}
}

//bool InputManager::IsKeyPressed(const int key) const
//{
//	//std::cout << "Key: " << keyboard[key] << "\n";
//	if (keyboard[key])
//		return true;
//
//	return false;
//}
