#pragma once
#include "Includes.h"
#include "events.h"
#include "Character.h"
#include "Collidable.h"
#include "Turret.h"

#pragma region METHOD_DECLARATIONS
void HandleFullScreen();
int loadTileMap(std::unique_ptr<tson::Map>& map, const char* levelDirectory);
int loadTileMapAssets(std::unique_ptr<tson::Map>& map);
void checkMapLayer(std::unique_ptr<tson::Map>& map);
int loadLevel(std::unique_ptr<tson::Map>& map, const char* levelDirectory);
void HandleChangeLevel(Player& player, std::unique_ptr<tson::Map>& map, const char* level, Vector2 spawn);
void CheckCollidableCollision(Player& player, NPC& npc, std::unique_ptr<tson::Map>& map, std::vector<std::shared_ptr<Turret>>& turrets);
void CheckPickableCollision(std::vector<std::shared_ptr<Pickable>>& pickable, Player& player);
void handleQuestLine(bool& value);
void AdvanceQuestLine();
void CheckCollisionWithEvent(Player& player, std::vector<std::shared_ptr<Events>>& events, SoundManager& music);
void Update(Player& player, std::vector<std::shared_ptr<NPC>>& NPC,
	std::unique_ptr<tson::Map>& map, std::vector<std::shared_ptr<Pickable>>& pickable,
	std::vector<std::shared_ptr<Events>>& events, SoundManager& sound, std::unique_ptr<FadeEffect>& fade, std::vector<std::shared_ptr<Turret>>& turrets);
void DrawObjectives(Textures textures, Vector2 shakeOffset);
Vector2 GetCurrentShakeOffset(const std::vector<std::shared_ptr<Events>>& events);
void DrawTile(std::unique_ptr<tson::Map>& map, Vector2 shakeOffset);
void Draw(Player& player, std::vector<std::shared_ptr<NPC>>& NPC, std::unique_ptr<tson::Map>& map, std::vector<std::shared_ptr<Pickable>>& pickable, Textures texture, std::vector<std::shared_ptr<Events>>& events, std::unique_ptr<FadeEffect>& fade, std::vector<std::shared_ptr<Turret>>& turrets);
#pragma endregion

#pragma region Variables
std::vector<std::unique_ptr<Collidable>> collidables;
float scaleFactor = 2.0f;

Texture2D tilesetTexture;
#pragma endregion

/// <summary>
/// If user presses F12 it will get their current main monitor, checks if the window isfullscreen set it to windowed else set it to fullscreen and then toggle it
/// </summary>
void HandleFullScreen()
{
	if (IsKeyPressed(KEY_F12))
	{
		// see what display we are on right now
		int display = GetCurrentMonitor();

		if (IsWindowFullscreen())
		{
			// if we are full screen, then go back to the windowed size
			SetWindowSize(SWIDTH, SHEIGHT);
		}
		else
		{
			// if we are not full screen, set the window size to match the monitor we are on
			SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
		}

		// toggle the state
		ToggleFullscreen();
	}
}

#pragma region HandleMapLoading
// Changes i made was to add level directory which is just a char pointer (string) of the map that needs to load from #define.h
//clear out the collidables list so the user doesnt collide with objects that arent in the current tile map that is loaded, if loaded correctly set current_map (define.h) to the level directory
#pragma region TEMPLATE_CODE
int loadTileMap(std::unique_ptr<tson::Map>& map, const char* levelDirectory) {
	try {
		collidables.clear();
		tson::Tileson tileson;

		map = tileson.parse(levelDirectory);

		if (map->getStatus() != tson::ParseStatus::OK) {
			std::cerr << "Error: Failed to load map '" << levelDirectory << "'\n";
			return -1;
		}

		current_map = levelDirectory;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while loading map: " << e.what() << "\n";
		return -1;
	}
	return 0;
}


int loadTileMapAssets(std::unique_ptr<tson::Map>& map) {
	try {
		if (map->getTilesets().empty()) {
			std::cerr << "Error: Tileset not found in the map\n";
			return -1;
		}

		// Load the first tileset texture
		tilesetTexture = LoadTexture(map->getTilesets()[0].getImage().u8string().c_str());

		if (tilesetTexture.id == 0) {
			std::cerr << "Error: Tileset texture not loaded!\n";
			return -1;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while loading tileset assets: " << e.what() << "\n";
		return -1;
	}
	return 0;
}

void checkMapLayer(std::unique_ptr<tson::Map>& map)
{
	for (auto& layer : map->getLayers())
	{
		if (layer.getType() == tson::LayerType::TileLayer)
		{
			// Iterate through the tile objects in the layer
			for (auto& [pos, tileObject] : layer.getTileObjects())
			{
				// Check if the tile has an object group with objects
				if (tileObject.getTile()->getObjectgroup().getObjects().size() > 0)
				{
					// Check if the tile has a "collidable" property set to true
					if (tileObject.getTile()->getProperties().hasProperty("collidable")) {
						if (tileObject.getTile()->getProperties().getProperty("collidable")->getValue<bool>()) {
							

							std::string tag;
							if (tileObject.getTile()->getProperties().hasProperty("tag")) {
								tag = tileObject.getTile()->getProperties().getProperty("tag")->getValue<std::string>();
							}

							// Iterate through the objects in the tile's object group
							for (auto& obj : tileObject.getTile()->getObjectgroup().getObjects())
							{

								if (obj.getPolygons().empty()) { //we assume a rectangle boudning box if no polygon is detected
									// Calculate the scaled position and size of the tile
									Rectangle colliderRect = {
										(float)(tileObject.getPosition().x + obj.getPosition().x) * scaleFactor,
										(float)(tileObject.getPosition().y + obj.getPosition().y) * scaleFactor,
										(float)obj.getSize().x * scaleFactor,
										(float)obj.getSize().y * scaleFactor
									};

									// Create a CollidableRectangle and add it to the collidables vector
									collidables.push_back(std::make_unique<CollidableRectangle>(colliderRect, tag));
								}

								if (!obj.getPolygons().empty()) { //if there are polygons, we assume the object is a polygon
									const std::vector<tson::Vector2i>& polygonPoints = obj.getPolygons();

									std::vector<Vector2> scaledPolygonPoints;

									for (const auto& point : polygonPoints) {
										// Scale the polygon points based on the scale factor
										Vector2 scaledPoint = {
											(float)(tileObject.getPosition().x + obj.getPosition().x + point.x) * scaleFactor,
											(float)(tileObject.getPosition().y + obj.getPosition().y + point.y) * scaleFactor
										};
										scaledPolygonPoints.push_back(scaledPoint);
									}

									// Create a CollidablePolygon and add it to the collidables vector
									collidables.push_back(std::make_unique<CollidablePolygon>(scaledPolygonPoints, tag));

								}
							}
						}

					}
				}
			}

		}
	}
}

#pragma endregion

// Loads a Tiled map, its tileset texture, and collision data.
int loadLevel(std::unique_ptr<tson::Map>& map, const char* levelDirectory)
{
		if (loadTileMap(map, levelDirectory) == -1) return -1;
	
		if (loadTileMapAssets(map) == -1) return -1;
	
		checkMapLayer(map);
	
	return 0;
}

/// <summary>
/// so if map is loading, dont check for collisions (later down the code base)
/// call loadLevel the method above passing map and the leve lyou want to load
/// update the player map so the player is drawn in the new tile map
/// update the position to the spawn position of that map (#define.h)
/// then turn off loading map
/// </summary>
/// <param name="player"></param>
/// <param name="map"></param>
/// <param name="level"></param>
/// <param name="spawn"></param>
void HandleChangeLevel(Player& player, std::unique_ptr<tson::Map>& map, const char* level, Vector2 spawn)
{
	isLoadingMap = true;
	loadLevel(map, level);
	player.UpdateMap(level);
	player.SetPosition(spawn);
	isLoadingMap = false;
}
#pragma endregion

#pragma region CollisionDetection
void CheckCollidableCollision(Player& player, NPC& npc, std::unique_ptr<tson::Map>& map, std::vector<std::shared_ptr<Turret>>& turrets)
{
	//Get player position
	Vector2 potentialPosition = player.GetPosition();

	float deltaTime = GetFrameTime();

	//if player is in trigger cutscene dont let him move
	if (!inCutscene)
	{
		potentialPosition.x += (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * player.GetSpeed() * deltaTime;
		potentialPosition.y += (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP)) * player.GetSpeed() * deltaTime;
	}

	// Create a rectangle for the potential new position
	Rectangle potentialPlayerRect = { potentialPosition.x, potentialPosition.y, player.GetRect().width, player.GetRect().height };

	// Check for collisions
	bool collisionDetected = false;

	//if map is loading dont check collisions
	if (!isLoadingMap)
	{
		//loop through colldable list
		for (const auto& collidable : collidables)
		{
			//loop through turret list
			#pragma region HandleTurretCollision

						for (auto& turret : turrets)
						{//if turret map == the current map then
							if (turret->GetMap() == current_map)
							{
								//get the list of bullets it has
								for (auto& bullet : turret->GetTurretBullets())
								{//check ifthat bullet collided with any object in the world and if the bullet is active
									if (collidable->CheckCollision(bullet->GetRect()) && bullet->GetIsActive())
									{
										//if the tag is dungeon 2nd level for map_six ignore collision so it can shoot player theough it
										if (collidable->tag != "dungeon_second_level")
										{
											//delete bullet if collides with wall
											turret->RemoveBullet(bullet);
										}
									}

									//if bullet collides with player
									if (CheckCollisionRecs(bullet->GetRect(), player.GetRect()) && bullet->GetIsActive())
									{
										//delete the bullet and damage the player based onm the damage of the bullet
										turret->RemoveBullet(bullet);
										player.DamagePlayer(bullet->GetBulletDamage());
									}
								}
							}
						}
			#pragma endregion


			//if player rect collides with npc rect and the npc is in the current map (so we dont get false collisions from unloaded tile maps)
			if (collidable->CheckCollision(npc.GetRect()) && collidable->CheckCollision(potentialPlayerRect) && npc.GetMap() == current_map)
			{
				//set collision to true
				collisionDetected = true;
			}
			//if player collides with tile map
			else if (collidable->CheckCollision(potentialPlayerRect))
			{
				collisionDetected = true;

				//If player collides with certain tags it will load the new level 
				#pragma region ChangeLevelCollision
								if (collidable->tag == "CaveEntrance")
								{
									
									if (current_map == map_one)
									{

										if (objectiveProgress == 1)
										{
											enteredCave = true;
										}

										HandleChangeLevel(player, map, map_two, map_two_spawn);

									}
									else if (current_map == map_two)
									{
										HandleChangeLevel(player, map, map_one, exit_cave_spawn);
									}

									break;
								}
								else if (collidable->tag == "DeeperEntrance")
								{
									
									if (current_map == map_two && objectiveProgress >= 3)
									{
										HandleChangeLevel(player, map, map_three, map_three_spawn);
										objectiveProgress++;
									}
									break;
								}
								else if (collidable->tag == "EastEntrance")
								{
									if (objectiveProgress == 0)
									{
										objectiveProgress++;
									}

									HandleChangeLevel(player, map, map_one, enter_map_one_spawn);

									break;
								}
								else if (collidable->tag == "WestEntrance")
								{
									HandleChangeLevel(player, map, spawn_area, enter_village_spawn);
									break;
								}

								else if (collidable->tag == "DUNGEONENTRANCE")
								{
									if (objectiveProgress == 5)
									{
										objectiveProgress++;
									}

									HandleChangeLevel(player, map, map_five, map_five_spawn);
									break;
								}

								else if (collidable->tag == "dungeon_second_level")
								{
									if (current_map == map_five)
									{
										HandleChangeLevel(player, map, map_six, map_six_spawn);
									}
									else if (current_map == map_six) {
										HandleChangeLevel(player, map, map_seven, map_seven_spawn);
									}
									else if (current_map == map_seven && objectiveProgress == 8)
									{
										HandleChangeLevel(player, map, map_eight, map_eight_spawn);
									}

									break;
								}
				#pragma endregion

				//if player collides with pit, damage him by 1
				else if (collidable->tag == "pit")
				{
					player.DamagePlayer(1);
				}
			}

			//if npc collided with terrain somehow then enable collision detection
			else if (collidable->CheckCollision(npc.GetRect()) && npc.GetMap() == current_map)
			{
				collisionDetected = true;
			}
		}
	}


	//if player didnt collide update the player 
	if (!collisionDetected)
	{
		player.SetPosition(potentialPosition);
		player.SetRect(potentialPlayerRect);
	}
}

/// <summary>
/// Get Player movement and rect, loop through pickable vector with referece to pointer it value
/// if th pickable map is the current one then check if playuer cllided with the pickable rect,
/// if ptr is coin,whitekey or gold key call the empty version of pickup override else call pickup with oplayer paramater ovverride 
/// then after pickup erase it from the list of pickables and continue to the next item and if the ptr object item does not get erased (meaning not picked up) we do ++it to move to the next element
/// </summary>
/// <param name="pickable"></param>
/// <param name="player"></param>
void CheckPickableCollision(std::vector<std::shared_ptr<Pickable>>& pickable, Player& player)
{
	float deltaTime = GetFrameTime();
	Vector2 potentialPosition = player.GetPosition();
	potentialPosition.x += (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * player.GetSpeed() * deltaTime;
	potentialPosition.y += (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP)) * player.GetSpeed() * deltaTime;

	// Create a rectangle for the potential new position
	Rectangle potentialPlayerRect = { potentialPosition.x, potentialPosition.y, player.GetRect().width, player.GetRect().height };

	for (auto it = pickable.begin(); it != pickable.end(); )
	{
		auto& ptr = *it;
		if (ptr->GetSpawnMap() == current_map) {
			if (CheckCollisionRecs(potentialPlayerRect, ptr->GetRect()))
			{
				if (typeid(*ptr) == typeid(Coin) || typeid(*ptr) == typeid(WhiteKey) || typeid(*ptr) == typeid(GoldKey))
				{
					ptr->Pickup();
				}
				else {
					ptr->Pickup(player);
				}

				it = pickable.erase(it);
				continue;
			}
		}
		++it;
	}
}

/// <summary>
/// loop through list of events, if player collides with event and map is the current active map asnd the the evernt can be triggered then trigger it by setting even to true
/// 
/// then cvheck if event is true, if event is true trigger both eventtrigger with overrides (1 is empty for either method that is calling it so 1 method will do implementation)
/// then remove the event from the list so it doesnt happen again if the event is finished
/// 
/// loop through event, if event is active set in cutscene is true and return so it doesnt turn it off, if there are no active events set inCutscene to false
/// </summary>
/// <param name="player"></param>
/// <param name="events"></param>
/// <param name="music"></param>
void CheckCollisionWithEvent(Player& player, std::vector<std::shared_ptr<Events>>& events, SoundManager& music)
{
	for (auto& event : events)
	{
		if (CheckCollisionRecs(player.GetRect(), event->GetTriggerBox()) && event->GetMap() == current_map && event->CanTriggerEvent())
		{
			event->SetEventActive(true);
		}

		if (event->GetEventActive())
		{
			event->EventTrigger(music, player);
			event->EventTrigger(player);
		}
	}

	events.erase(
		std::remove_if(events.begin(), events.end(),
			[](const std::shared_ptr<Events>& event) {
				return event->GetFinished();
			}),
		events.end());

	for (auto& event : events)
	{
		if (event->GetEventActive())
		{
			inCutscene = true;
			return;
		}
	}

	inCutscene = false;
}
#pragma endregion

#pragma region QuestLine
//if value is true set it to false and progress the player
void handleQuestLine(bool& value)
{
	if (value)
	{
		value = false;
		objectiveProgress++;
	}
}

//switch case based on objectiveProgress (define.h)
void AdvanceQuestLine()
{
	//switch cases based on progress of the player in the quest, kind of redundant ended upo doing another way but cant be bothered rewriting these
	switch (objectiveProgress)
	{
	case 1:
		handleQuestLine(enteredCave);
		break;
	case 2:
		handleQuestLine(collectedEveryGem);
		break;
	}
}
#pragma endregion


#pragma region DRAWING_METHODS

/// <summary>
/// Draw the objective tab, so get textures background from GameTextures.h set its position
/// draw the background for it, shakeoffset is for ShakeEvent from events.h
/// 
/// get text width by m,easuring the text and the font size then get the x pos based on the center of obhwective background + the offset 
/// </summary>
/// <param name="textures"></param>
/// <param name="shakeOffset"></param>
void DrawObjectives(Textures textures, Vector2 shakeOffset)
{
	Texture objbg = textures.objectiveBackgroundtexture;
	Vector2 objv = { 1400, 10 };

	// Draw background
	DrawTextureEx(objbg, Vector2Add(objv, shakeOffset), 0, 1, WHITE);

	// Draw current objective text centered horizontally
	const char* objectiveText = objectives[objectiveProgress].c_str();
	int textWidth = MeasureText(objectiveText, 20);

	float xPos = (float)(objv.x + (float)(objbg.width / 2) - (float)(textWidth / 2));

	DrawText(objectiveText, static_cast<int>(xPos + shakeOffset.x), static_cast<int>((objv.y + 20) + shakeOffset.y), 20, BLACK);


	//if the objective is to collect gems / coins it will measure the width make an offset to center and place it slihtly under the main goal text and draw
	if (objectiveProgress == 2) {
		std::string goalText = TextFormat("%i / %i", coinsCollected, coinsToCollect);
		int gtextWidth = MeasureText(goalText.c_str(), 20);
		float gxPos = (float)(objv.x + (float)(objbg.width / 2) - (float)(gtextWidth / 2));
		DrawText(goalText.c_str(),
			static_cast<int>(gxPos + shakeOffset.x),
			static_cast<int>((objv.y + 40) + shakeOffset.y),
			20, BLACK);
	}
}


// Draws the active tile layer with the current screen-shake offset. 
void DrawTile(std::unique_ptr<tson::Map>& map, Vector2 shakeOffset)
{
	for (auto& layer : map->getLayers()) {
		if (layer.getType() == tson::LayerType::TileLayer) {
			for (auto& [pos, tileObject] : layer.getTileObjects()) {
				tson::Rect drawingRect = tileObject.getDrawingRect();
				Rectangle destRect = {
					(tileObject.getPosition().x * scaleFactor + shakeOffset.x), 
					(tileObject.getPosition().y * scaleFactor + shakeOffset.y), 
					(float)map->getTileSize().x * scaleFactor,
					(float)map->getTileSize().y * scaleFactor
				};
				Rectangle sourceRect = {
					(float)drawingRect.x,
					(float)drawingRect.y,
					(float)drawingRect.width,
					(float)drawingRect.height
				};
				DrawTexturePro(tilesetTexture, sourceRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
			}
		}
	}
}
#pragma endregion


#pragma region HANDLE_GAME
/// <summary>
/// Reference event pointer loop throug it (const cause we arent changing the value) if the event is shakescreen event dynamically case it to be one and get the object with event.get()
/// if shakeevent is not null and the event is active, get shakeoffset else return 0;
/// </summary>
/// <param name="events"></param>
/// <returns></returns>
Vector2 GetCurrentShakeOffset(std::vector<std::shared_ptr<Events>>& events) {
	for (const auto& event : events) {
		ShakeScreenEvent* shakeEvent = dynamic_cast<ShakeScreenEvent*>(event.get());
		if (shakeEvent && shakeEvent->GetEventActive()) {
			return shakeEvent->GetShakeOffset();
		}
	}
	return { 0.0f, 0.0f };
}

//this method is just used to call methods everry loop, to update functionality, hitboxes, collisions, rotations, trigger events etc or if player collected white key and gold key
void Update(Player& player, std::vector<std::shared_ptr<NPC>>& NPC, std::unique_ptr<tson::Map>& map,
	std::vector<std::shared_ptr<Pickable>>& pickable, std::vector<std::shared_ptr<Events>>& events, SoundManager& sound, std::unique_ptr<FadeEffect>& fade, std::vector<std::shared_ptr<Turret>>& turrets)
{
	CheckPickableCollision(pickable, player);
	CheckCollisionWithEvent(player, events, sound);

	for (auto& npc : NPC)
	{
		CheckCollidableCollision(player, *npc, map, turrets);

		player.Update(*npc);

		if (npc->GetMap() == current_map)
		{
			npc->Update(player);
		}
	}

	for (auto& turret : turrets)
	{
		turret->Update(player);
	}


	if (collectedWhiteKey && collectedGoldKey)
	{
		objectiveProgress++;
		collectedWhiteKey = false;
		collectedGoldKey = false;
	}


	AdvanceQuestLine();

	fade->update(player);
}


void Draw(Player& player, std::vector<std::shared_ptr<NPC>>& NPC, std::unique_ptr<tson::Map>& map,
	std::vector<std::shared_ptr<Pickable>>& pickable, Textures textures, std::vector<std::shared_ptr<Events>>& events, std::unique_ptr<FadeEffect>& fade, std::vector<std::shared_ptr<Turret>>& turrets)
{
	BeginDrawing();
	ClearBackground(BLACK);


	//Get shakeoffset
	Vector2 shakeOffset = GetCurrentShakeOffset(events);

	//draw tile map
	DrawTile(map, shakeOffset);

	//draw npc
	for (auto& npc : NPC)
	{
		if (npc->GetMap() == current_map) {
			npc->Draw(shakeOffset);
		}
	}

	//draw player
	player.Draw(textures, shakeOffset);


	//draw pickable items
	for (auto& ptr : pickable) {
		if (ptr->GetSpawnMap() == current_map) {
			ptr->Draw(shakeOffset);
		}
	}

	if (STATE == GAME)
	{
		//Draw Teleportation circles

		for (auto& event : events) {
			if (event->GetMap() == current_map)
			{	//if event.get() is teleport event then dynamically cast it to be teleport event pointer then check if its not null and draw
				TeleportEvent* teleportEvent = dynamic_cast<TeleportEvent*>(event.get());
				if (teleportEvent) {
					DrawTextureEx(teleportEvent->GetTexture(), Vector2{ teleportEvent->GetTriggerBox().x,teleportEvent->GetTriggerBox().y }, 0, 1, WHITE);
				}
			}
		}

		//Draw Abuduction Event

		for (auto& event : events)
		{
			if (event->GetEventActive() && event->GetMap() == current_map)
			{
				if (typeid(*event).name() == typeid(AbductionEvent).name())
				{
					AbductionEvent* abductionEvent = dynamic_cast<AbductionEvent*>(event.get());

					abductionEvent->draw();
				}

			}
		}
	}


	//draw turret
	for (auto& turret : turrets)
	{
		turret->Draw(player);
	}

	//draw the objective (quests)
	DrawObjectives(textures, shakeOffset);


	//draw fade overlayed above everything else if its active
	fade->draw();


	EndDrawing();
}

#pragma endregion
