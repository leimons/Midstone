#include "VMath.h"
#include "Player.h"
#include "GameManager.h"

Player::~Player() {
	std::cerr << "Calling Player destructor..." << std::endl;
}

//test collision before moving character
/*/steps:
	- Get player velocity to find out the direction of the player movement
	- Get player position and transform to tile to find out the players next tile movements
	- Calculate the tiles to be tested
	- Query the map for the tiles
		- Test each tile, and if colision, set the velocity on that direction to zero
			(player->SetVelocity(player->getVelocity() - Vec3(0.0f, -1.0f, 0.0f)); for example.
			this line sets the velocity to the Y vector to be zero in case the player is trying to move UP
			but there is an obstacle
*/

//if the object is moving, its velocity is different from 0, so it's velocity vector magnitude is different from zero
//in this case, test for colision
void Player::TestCollision()
{
	//Vec3 collisionBoxPosition = Vec3(position.x + collisionBox.topLeftCorner.x, )
	float xMultiplier = 1;
	float yMultiplier = 1;

	if (position.x < 0)
		xMultiplier = -1;

	if (position.y < 0)
		yMultiplier = -1;

	if (VMath::mag(velocity) != 0) {
		MATH::Vec3 vecAuxAdjancent; //test side
		MATH::Vec3 vecAuxDiagonal; //test bottom or up
		int tileValueAdjacent = -1;
		int tileValueDiagonal = -1;

		//object going righ -> +x
		if (velocity.x > 0.0f)
		{
			//we have to use the speed because the object will move speed pixels
			vecAuxAdjancent = position + Vec3((xMultiplier*collisionBox.bottomRightCorner.x) + speed, (yMultiplier*collisionBox.topLeftCorner.y), 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier*collisionBox.bottomRightCorner.x) + speed, (yMultiplier*collisionBox.bottomRightCorner.y), 0.0f);
			//vecAuxAdjancent = position + Vec3(TILE_RENDER_SIZE + speed, 0.0f, 0.0f);
			//vecAuxDiagonal = position + Vec3(TILE_RENDER_SIZE + speed, (TILE_RENDER_SIZE - 1), 0.0f);
		}
		//object going left -> -x
		else if (velocity.x < 0.0f)
		{
			vecAuxAdjancent = position + Vec3((xMultiplier*collisionBox.topLeftCorner.x) - speed, (yMultiplier*collisionBox.topLeftCorner.y), 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier*collisionBox.topLeftCorner.x) - speed, (yMultiplier*collisionBox.bottomRightCorner.y), 0.0f);
			//vecAuxAdjancent = position + Vec3(-speed, 0.0f, 0.0f);
			//vecAuxDiagonal = position + Vec3(-speed, (TILE_RENDER_SIZE - 1), 0.0f);
		}

		tileValueAdjacent = collisionLayer->at(vecAuxAdjancent.y / TILE_RENDER_SIZE).at(vecAuxAdjancent.x / TILE_RENDER_SIZE);
		tileValueDiagonal = collisionLayer->at(vecAuxDiagonal.y / TILE_RENDER_SIZE).at(vecAuxDiagonal.x / TILE_RENDER_SIZE);

		//if tile is bigger than 0, it means there is a object in the map and we should stop our player
		//so set the velocity to zero.
		if (tileValueAdjacent > 0 || tileValueDiagonal > 0) {
			//testObj->setVelocity(testObj->getVelocity() + Vec3(-testObj->getVelocity().x, 0.0f, 0.0f));
			velocity.x = 0.0f;
		}

		//now for the Y axis
		tileValueAdjacent = -1;
		tileValueDiagonal = -1;
		//object going righ -> +y (down the screen)
		if (velocity.y > 0.0f)
		{
			vecAuxAdjancent = position + Vec3((xMultiplier*collisionBox.topLeftCorner.x), (yMultiplier*collisionBox.bottomRightCorner.y) + speed, 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier*collisionBox.bottomRightCorner.x), (yMultiplier*collisionBox.bottomRightCorner.y) + speed, 0.0f);
			//vecAuxAdjancent = position + Vec3(0.0f, TILE_RENDER_SIZE + speed, 0.0f);
			//vecAuxDiagonal = position + Vec3((TILE_RENDER_SIZE - 1), TILE_RENDER_SIZE + speed, 0.0f);
		}
		//object going left -> -y (up on the screen)
		else if (velocity.y < 0.0f)
		{
			vecAuxAdjancent = position + Vec3((xMultiplier*collisionBox.topLeftCorner.x), (yMultiplier*collisionBox.topLeftCorner.y) -speed, 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier*collisionBox.bottomRightCorner.x), (yMultiplier*collisionBox.topLeftCorner.y) - speed, 0.0f);
			//vecAuxAdjancent = position + Vec3(0.0f, -speed, 0.0f);
			//vecAuxDiagonal = position + Vec3((TILE_RENDER_SIZE - 1), -speed, 0.0f);
		}

		tileValueAdjacent = collisionLayer->at(vecAuxAdjancent.y / TILE_RENDER_SIZE).at(vecAuxAdjancent.x / TILE_RENDER_SIZE);
		tileValueDiagonal = collisionLayer->at(vecAuxDiagonal.y / TILE_RENDER_SIZE).at(vecAuxDiagonal.x / TILE_RENDER_SIZE);

		//if tile is bigger than 0, it means there is a object in the map and we should stop our player
		//so set the velocity to zero.
		if (tileValueAdjacent > 0 || tileValueDiagonal > 0) {
			velocity.y = 0.0f;
		}
	}
}

bool Player::Interact()
{
	float xMultiplier = 1;
	float yMultiplier = 1;

	if (position.x < 0)
		xMultiplier = -1;

	if (position.y < 0)
		yMultiplier = -1;


	if (VMath::mag(orientation) == 0)
		return false;


	//std::cout << "Test interaction!\n";
	TILE playerTile;
	playerTile.x = position.y / TILE_RENDER_SIZE;
	playerTile.y = position.x / TILE_RENDER_SIZE;
	//std::cout << "Player tile coordinates: " << playerTile.x << "," << playerTile.y << "\n";
	MATH::Vec3 vecAuxAdjancent; //test side
	MATH::Vec3 vecAuxDiagonal; //test bottom or up
	int tileValueAdjacent = -1;
	int tileValueDiagonal = -1;
	TILE tileCoords;
	unsigned int tileID;
	//player is facing left or right
	if (orientation.x != 0) {
		//std::cout << "Interaction on X axis\n";
		//object going righ -> +x
		if (orientation.x > 0.0f)
		{
			//std::cout << "Interaction on +X\n";
			//we have to use the speed because the object will move speed pixels
			vecAuxAdjancent = position + Vec3((xMultiplier * collisionBox.bottomRightCorner.x) + speed, (yMultiplier * collisionBox.topLeftCorner.y), 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier * collisionBox.bottomRightCorner.x) + speed, (yMultiplier * collisionBox.bottomRightCorner.y), 0.0f);
		}
		//object going left -> -x
		else if (orientation.x < 0.0f)
		{
			//std::cout << "Interaction on -X\n";
			vecAuxAdjancent = position + Vec3((xMultiplier * collisionBox.topLeftCorner.x) - speed, (yMultiplier * collisionBox.topLeftCorner.y), 0.0f);;
			vecAuxDiagonal = position + Vec3((xMultiplier * collisionBox.topLeftCorner.x) - speed, (yMultiplier * collisionBox.bottomRightCorner.y), 0.0f);
		}

		tileCoords.x = vecAuxAdjancent.y / TILE_RENDER_SIZE;
		tileCoords.y = vecAuxAdjancent.x / TILE_RENDER_SIZE;
		tileID = unsigned(collisionLayer->at(tileCoords.x).at(tileCoords.y));
		///std::cout << "Tile ID: " << tileID << std::endl;
		//priority to the top collision - if no object in the top corner, test the one below it
		if (GameManager::getInstance()->OBJECT_MAP.find(tileID) == GameManager::getInstance()->OBJECT_MAP.end() || tileID == 0) {
			tileCoords.x = vecAuxDiagonal.y / TILE_RENDER_SIZE;
			tileCoords.y = vecAuxDiagonal.x / TILE_RENDER_SIZE;
			tileID = unsigned(collisionLayer->at(tileCoords.x).at(tileCoords.y));
			//std::cout << "Tile ID: " << tileID << std::endl;

			//no objects on the MAP or in the collision layer
			if (GameManager::getInstance()->OBJECT_MAP.find(tileID) == GameManager::getInstance()->OBJECT_MAP.end() || tileID == 0)
				Attack();
				return false;
		}
	}
	//player if facing up or down
	else {
		//std::cout << "Player facing up or down\n";
		//object going righ -> +x
		if (orientation.y > 0.0f)
		{
			//std::cout << "Interaction on +Y\n";
			//we have to use the speed because the object will move speed pixels
			vecAuxAdjancent = position + Vec3((xMultiplier * collisionBox.topLeftCorner.x), (yMultiplier * collisionBox.bottomRightCorner.y) + speed, 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier * collisionBox.bottomRightCorner.x), (yMultiplier * collisionBox.bottomRightCorner.y) + speed, 0.0f);
		}
		//object going left -> -x
		else if (orientation.y < 0.0f)
		{
			//std::cout << "Interaction on -Y\n";
			vecAuxAdjancent = position + Vec3((xMultiplier * collisionBox.topLeftCorner.x), (yMultiplier * collisionBox.topLeftCorner.y) - speed, 0.0f);
			vecAuxDiagonal = position + Vec3((xMultiplier * collisionBox.bottomRightCorner.x), (yMultiplier * collisionBox.topLeftCorner.y) - speed, 0.0f);
		}

		tileCoords.x = vecAuxAdjancent.y / TILE_RENDER_SIZE;
		tileCoords.y = vecAuxAdjancent.x / TILE_RENDER_SIZE;
		tileID = unsigned(collisionLayer->at(tileCoords.x).at(tileCoords.y));
		//std::cout << "Tile ID: " << tileID << std::endl;
		//priority to the top collision - if no object in the top corner, test the one below it
		if (GameManager::getInstance()->OBJECT_MAP.find(tileID) == GameManager::getInstance()->OBJECT_MAP.end() || tileID == 0) {
			tileCoords.x = static_cast<int>(vecAuxDiagonal.y / TILE_RENDER_SIZE);
			tileCoords.y = static_cast<int>(vecAuxDiagonal.x / TILE_RENDER_SIZE);
			tileID = unsigned(collisionLayer->at(tileCoords.x).at(tileCoords.y));
			//std::cout << "Tile ID: " << tileID << std::endl;

			//no objects on the MAP or in the collision layer
			if (GameManager::getInstance()->OBJECT_MAP.find(tileID) == GameManager::getInstance()->OBJECT_MAP.end() || tileID == 0)
				Attack();
				return false;
		}
	}

	//std::cout << "Tile coordinates: " << tileCoords.x << "," << tileCoords.y << "\n";
	CheckObjectInteractionList(tileCoords, tileID);

	return true;
}

void Player::CheckObjectInteractionList(TILE key, unsigned int objectID)
{
	velocity = Vec3(0.0f, 0.0f, 0.0f);
	std::string animationName;
	Object interactionObj = GameManager::getInstance()->OBJECT_MAP.at(objectID);
	switch (interactionObj.type) {
	case OBJECT_TYPE::IRON:
		animationName = "pickaxe_";
		if (orientation.x > 0) {//player is going right - priorize side movement
			animationName += "right";
		}
		else if (orientation.x < 0) {
			animationName += "left";
		}
		else if (orientation.y > 0) {
			animationName += "down";
		}
		else if (orientation.y < 0) {
			animationName += "up";
		}
		characterAnimation->ChangeAnimation(animationName);
		//interacting = true;
		//interactedObjects.find(key);
		if (interactedObjects.find(key) != interactedObjects.end()) {
			//the object was found
			interactedObjects.at(key)->numberInteractions++;
			//std::cout << "Object interacted with already on the list! ID: " << interactedObjects.at(key)->objNumber << std::endl;
		}
		else {
			//first interaction. Creates a new object on the map
			ObjectScene* newObj = new ObjectScene{ 1, objectID };
			interactedObjects.insert(std::pair<TILE, ObjectScene*>(key, newObj));
			//std::cout << "Object interacted with NEW! ID: " << interactedObjects.at(key)->objNumber << std::endl;
		}

		//if the object being interacted with reached the number of interactions necessary to be deleted, remove from map
		if (interactedObjects.at(key)->numberInteractions >= interactionObj.interactionNumber)
		{
			(*collisionLayer)[key.x][key.y] = 0;
			AddItemBag(interactionObj.loot, interactionObj.lootQuantity);
			interactedObjects.erase(key);
		}

		break;
		// scene switching in game.
	case OBJECT_TYPE::DOOR: // enter the mines
		EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(MINES_SCENE1));
		mineLevel = 1;
		break;
	case OBJECT_TYPE::EXIT: // leave the mines
		EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(SHOP_SCENE));
		break;
	case OBJECT_TYPE::LADDER: // the way to the upper floor
		if (mineLevel == 2) { 
			EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(MINES_SCENE1));
			mineLevel = 1;
		}
		else {
			EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(MINES_SCENE2));
			mineLevel = 2;
		}
		break;
	case OBJECT_TYPE::HATCH: // the way to the lower floor
		if (mineLevel == 1) {
			EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(MINES_SCENE2));
			mineLevel = 2;
		}
		else {
			EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(MINES_SCENE3));
			mineLevel = 3;
		}
		break;
		//Bed interaction, heals the player to full
	case OBJECT_TYPE::BED:
		setHealth(maxHealth);
		animationName = "interact_";
		if (orientation.x > 0) {//player is going right - priorize side movement
			animationName += "right";
		}
		else if (orientation.x < 0) {
			animationName += "left";
		}
		else if (orientation.y > 0) {
			animationName += "down";
		}
		else if (orientation.y < 0) {
			animationName += "up";
		}
		characterAnimation->ChangeAnimation(animationName);
		break;
	default:
		Attack();
		//animationName = "interact_";
		//if (orientation.x > 0) {//player is going right - priorize side movement
		//	animationName += "right";
		//}
		//else if (orientation.x < 0) {
		//	animationName += "left";
		//}
		//else if (orientation.y > 0) {
		//	animationName += "down";
		//}
		//else if (orientation.y < 0) {
		//	animationName += "up";
		//}
		//characterAnimation->ChangeAnimation(animationName);

		break;
	}

}

void Player::AddItemBag(OBJECT_TYPE objType, unsigned int quantity) {

	//player already has type of item in the bag
	if (playerBag.find(objType) != playerBag.end()) {
		playerBag.at(objType)->quatity += quantity;
	}
	//player has none of the item in the bag
	else {
		ObjectLoot* newObj = new ObjectLoot{ quantity };
		playerBag.insert(std::pair<OBJECT_TYPE, ObjectLoot*>(objType, newObj));
	}
}

void Player::Attack() {

	velocity = Vec3(0.0f, 0.0f, 0.0f);
	std::string animationName = "attack_";

	if (orientation.x > 0) {//player is going right - priorize side movement
		animationName += "right";
	}
	else if (orientation.x < 0) {
		animationName += "left";
	}
	else if (orientation.y > 0) {
		animationName += "down";
	}
	else if (orientation.y < 0) {
		animationName += "up";
	}
	characterAnimation->ChangeAnimation(animationName);
}

void Player::Update(float deltaTime) {
	if (tag == "dead") {
		buffer += deltaTime;
		if (buffer >= 3.0f) {
			EventHandler::GetInstance()->Broadcast(ChangeSceneEvent(SHOP_SCENE));
			tag = "Player";
			buffer = 0.0f;
		}
	}
	else if (tag == "anvil") {
		characterAnimation->ChangeAnimation("anviled");
	}
	else {
		position += velocity * speed;

		//set animation
		std::string animationName;
		if (VMath::mag(velocity) == 0) {//player is not moving - select idle animation
			animationName = "idle_";
		}
		else { //player is moving - select walk animation
			animationName = "walk_";
		}

		if (orientation.x > 0) {//player is going right - priorize side movement
			animationName += "right";
		}
		else if (orientation.x < 0) {
			animationName += "left";
		}
		else if (orientation.y > 0) {
			animationName += "down";
		}
		else if (orientation.y < 0) {
			animationName += "up";
		}

		//if(!interacting)
		characterAnimation->ChangeAnimation(animationName);
	}
	characterAnimation->Update(deltaTime);
}

void Player::Render(SDL_Renderer* sceneRender) {
	SDL_Rect rect;
	SDL_Rect clip = characterAnimation->getCurrentFrameSprite();
	MATH::Vec3 screenCoordinates = Camera::ToScreenCoordinates(position);

	rect.x = static_cast<int>(screenCoordinates.x + (characterAnimation->getCurrentAnimationInfo().anchor_x * TILE_SCALE));
	rect.y = static_cast<int>(screenCoordinates.y + (characterAnimation->getCurrentAnimationInfo().anchor_y * TILE_SCALE));
	rect.w = clip.w * TILE_SCALE;
	rect.h = clip.h * TILE_SCALE;

	SDL_RenderCopyEx(sceneRenderer, texture, &clip, &rect, 0.0, nullptr, SDL_FLIP_NONE);
}

void Player::HandleEvents()
{
	if (characterAnimation->getLockState())
		return;

    //key Down event -> movent character and play walking animation
    if (InputManager::getInstance()->IsKeyDown(SDLK_w))
        velocity.y = -1.0f;

    if (InputManager::getInstance()->IsKeyDown(SDLK_s))
        velocity.y = 1.0f;

    if (InputManager::getInstance()->IsKeyDown(SDLK_a))
        velocity.x = -1.0f;

    if (InputManager::getInstance()->IsKeyDown(SDLK_d))
        velocity.x = 1.0f;

	if(VMath::mag(velocity) != 0)
		orientation = velocity; //saves the orientation of the character

    //key UP event -> set animation to stoped at direction
    if (InputManager::getInstance()->IsKeyUp(SDLK_w))
        velocity.y = 0.0f;

    if (InputManager::getInstance()->IsKeyUp(SDLK_s))
        velocity.y = 0.0f;

    if (InputManager::getInstance()->IsKeyUp(SDLK_a))
        velocity.x = 0.0f;

	if (InputManager::getInstance()->IsKeyUp(SDLK_d))
		velocity.x = 0.0f;

	TestCollision();

	// changed from ->IsKeyDown to ->IsKeyUp
	if (InputManager::getInstance()->IsKeyUp(SDLK_e)) {
		//std::cout << "\n-------------------------------------------------------\n";
		//std::cout << "E pressed\n";
		Interact();
		//std::cout << "\n-------------------------------------------------------\n";
	}

	if (InputManager::getInstance()->IsKeyDown(SDLK_SPACE)) {  
		Interact();
		//Attack();
	}

	if (InputManager::getInstance()->IsKeyDown(SDLK_h)) {
		TakeDamage(50);
	}

	if (InputManager::getInstance()->IsKeyDown(SDLK_BACKSPACE) && InputManager::getInstance()->IsKeyDown(SDLK_a)) {
		velocity = Vec3(0.0f, 0.0f, 0.0f);
		characterAnimation->ChangeAnimation("anvil");
		tag = "anvil";
	}

	//Test for event
	//if (InputManager::getInstance()->IsKeyUp(SDLK_l))
	//	EventHandler::GetInstance()->Unsubscribe(ScreamEvent::eventType, "Player5");


}
