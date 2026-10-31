//Included needed header files
#include "Includes.h"
#include "define.h"
#include "SoundManager.h"
#include "GameTextures.h"
#include "Collectable.h"
#include "FadeEffect.h"
#include "events.h"
#include "main.h"
#include "Character.h"
#include "Turret.h"

int main(void)
{

	InitWindow(SWIDTH, SHEIGHT, "Arin's Adventure");
	SetTargetFPS(FPS);

	InitAudioDevice();
	SoundManager sound;

	#pragma region MAIN_MENURAYGUI
		//RAY GUI implemenation for buttons and labels
		Rectangle btnStartGame = { SWIDTH / 2 - 100, 180, 200, 40 };
		Rectangle btnExit = { SWIDTH / 2 - 100, 230, 200, 40 };
		Rectangle btnLabel = Rectangle{ SWIDTH / 2 - 200, 100, 600, 20 };

		//Set font size
		Font labelFont = GetFontDefault();
		labelFont.baseSize = 2;

		Font buttonFont = GetFontDefault();
		buttonFont.baseSize = 4;
	#pragma endregion

	//pointer for map and music
	std::unique_ptr<tson::Map> map;

	//Set the start music from static instance of sound class
	std::unique_ptr<Music> currentMusic = std::make_unique<Music>(sound.gameMusic);

	//play sound
	PlayMusicStream(*currentMusic);

	//load the level for background for main menu
	loadLevel(map, map_five);

	//static texture initalize
	Textures texture;

	//Game Loop
	while (!WindowShouldClose())
	{
		#pragma region MAINMENU
				while (!WindowShouldClose() && STATE == MAIN_MENU)
				{
					//Plays the music
					UpdateMusicStream(*currentMusic);

					BeginDrawing();

					ClearBackground(RAYWHITE);

					DrawTile(map, Vector2{ 0,0 });

					GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
					GuiSetFont(labelFont);
					GuiLabel(btnLabel, "ARIN'S ADVENTURE");


					GuiSetFont(buttonFont);
					if (GuiButton(btnStartGame, "Start Game")) {
						STATE = GAME;
					}

					if (GuiButton(btnExit, "Exit")) {
						CloseWindow();
					}

					EndDrawing();
				}

				//Make Smart pointer vectors of classes
				std::vector<std::shared_ptr<Pickable>> pickables = std::vector<std::shared_ptr<Pickable>>();
				std::vector<std::shared_ptr<Events>> events = std::vector<std::shared_ptr<Events>>();

				//default map to load on start of game
				//accessable at #define.h
				const char* beginner_map = spawn_area;
				const Vector2 beginner_spawn = village_spawn;

				//Set game to fullscreen
				SetConfigFlags(FLAG_FULLSCREEN_MODE);

		#pragma endregion
	
		#pragma region MAIN

				//make unique smart pointers of player and fade effect
				std::unique_ptr player = (std::make_unique<Player>(1.0f, beginner_spawn, 60.0f, 3, beginner_map));
				std::unique_ptr fade = (std::make_unique<FadeEffect>(0.01f, [&map](Player& player) {HandleChangeLevel(player, map, map_four, map_four_spawn); }));

		#pragma region AddNPC
				//shared pointer of NPC
				std::vector<std::shared_ptr<NPC>> NPCLIST = std::vector<std::shared_ptr<NPC>>();

				//Add npc to list
				NPCLIST.push_back(std::make_shared<BettyNPC>(1.0f, Vector2{ 200,300 }, 150.0f, Vector2{ 200,300 }, Vector2{ 500,300 }, spawn_area));
				NPCLIST.push_back(std::make_shared<BettyNPC>(1.0f, Vector2{ 450, 400 }, 100.0f, Vector2{ 450,400 }, Vector2{ 500,600 }, spawn_area));
				NPCLIST.push_back(std::make_shared<BettyNPC>(1.0f, Vector2{ 500, 600 }, 100.0f, Vector2{ 500,600 }, Vector2{ 700,600 }, spawn_area));

				//add pickables to list 
				pickables.push_back(std::make_shared<HealthPotion>(texture.healthPotion, Vector2{ 220,950 }, map_five));

				pickables.push_back(std::make_shared<HealthPotion>(texture.healthPotion, Vector2{ 320,920 }, map_six));
				pickables.push_back(std::make_shared<Shield>(texture.shield, Vector2{ 360,920 }, map_six));
				pickables.push_back(std::make_shared<SpeedBoost>(texture.speedPotion, Vector2{ 320,1000 }, map_six));

				pickables.push_back(std::make_shared<InvisibilityPotion>(texture.invisibilityPotion, Vector2{ 820,920 }, map_six));
				pickables.push_back(std::make_shared<Shield>(texture.shield, Vector2{ 780,920 }, map_six));
				pickables.push_back(std::make_shared<SpeedBoost>(texture.speedPotion, Vector2{ 760,980 }, map_six));

				pickables.push_back(std::make_shared<InvisibilityPotion>(texture.invisibilityPotion, Vector2{ 820,920 }, map_seven));
				pickables.push_back(std::make_shared<Shield>(texture.shield, Vector2{ 780,920 }, map_seven));
				pickables.push_back(std::make_shared<SpeedBoost>(texture.speedPotion, Vector2{ 760,980 }, map_seven));

				pickables.push_back(std::make_shared<GoldKey>(texture.GoldKey, Vector2{ 100,980 }, map_seven));
				pickables.push_back(std::make_shared<WhiteKey>(texture.WhiteKey, Vector2{ 700,500 }, map_seven));

				pickables.push_back(std::make_shared<InvisibilityPotion>(texture.invisibilityPotion, Vector2{ 567,285 }, map_seven));

				pickables.push_back(std::make_shared<Shield>(texture.shield, Vector2{ 371,438 }, map_seven));

				pickables.push_back(std::make_shared<Coin>(texture.coin_textures[GetRandomValue(0, static_cast<int>((texture.coin_textures.size()) - 1))], Vector2{ 100,580 }, map_two));
				pickables.push_back(std::make_shared<Coin>(texture.coin_textures[GetRandomValue(0, static_cast<int>((texture.coin_textures.size()) - 1))], Vector2{ 600,350 }, map_two));
				pickables.push_back(std::make_shared<Coin>(texture.coin_textures[GetRandomValue(0, static_cast<int>((texture.coin_textures.size()) - 1))], Vector2{ 90,950 }, map_two));
				pickables.push_back(std::make_shared<Coin>(texture.coin_textures[GetRandomValue(0, static_cast<int>((texture.coin_textures.size()) - 1))], Vector2{ 850,400 }, map_two));
				pickables.push_back(std::make_shared<Coin>(texture.coin_textures[GetRandomValue(0, static_cast<int>((texture.coin_textures.size()) - 1))], Vector2{ 1800,150 }, map_two));
				pickables.push_back(std::make_shared<Coin>(texture.coin_textures[GetRandomValue(0, static_cast<int>((texture.coin_textures.size()) - 1))], Vector2{ 120,200 }, map_two));
		#pragma endregion

		#pragma region AddEventsToList

				//----------------------------------------Add Events to List---------------------------------------------------------------------------------------------------------------------------------------------------
				events.push_back(std::make_shared<ShakeScreenEvent>(Rectangle{ SWIDTH / 2 - 150, SHEIGHT / 2 + 200, 200, 100 }, map_three, Vector2{ 50, 10 }, 20.0f, 6.0f, 4, [](Player& player) {}));

				events.push_back(std::make_shared<ShakeScreenEvent>(
					Rectangle{ SWIDTH / 2 - 320, SHEIGHT / 2, 550, 100 },
					map_three,
					Vector2{ 40, 30 },
					20.0f, 6.0f, 4,
					[](Player& player) {
						objectiveProgress++;
					}
				));

				events.push_back(std::make_shared<ShakeScreenEvent>(
					Rectangle{ SWIDTH / 2 - 150, SHEIGHT / 2 + 200, 550, 100 },
					map_three,
					Vector2{ 100, 100 },
					50.0f, 6.0f, 5,
					[&fade](Player& player) {
						objectiveProgress++;
						player.DamagePlayer(1);
						fade->startFadeIn();
					}
				));

				events.push_back(std::make_shared<ShakeScreenEvent>(
					Rectangle{ 100, 100, 550, 100 },
					map_six,
					Vector2{ 100, 100 },
					50.0f, 6.0f, 5,
					[](Player& player) {
						objectiveProgress++;
					}
				));

				events.push_back(std::make_shared<AbductionEvent>(Rectangle{ SWIDTH / 2 + 150, 50, 50,30 }, map_one, 1, [](Player& player) {
					wasAbducted = true;
					STATE = END_GAME;
					}, 20.0f));

				events.push_back(std::make_shared<TeleportEvent>(
					Rectangle{ SWIDTH / 2, SHEIGHT / 2, static_cast<float>(texture.teleporter.width), static_cast<float>(texture.teleporter.height) },
					map_eight, 8, [&map](Player& player) {
						HandleChangeLevel(player, map, map_one, enter_map_one_spawn);
						gameFinished = true;
						STATE = END_GAME;
					},
					texture.teleporter
				));
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		#pragma endregion

		#pragma region Turrets
				std::vector<std::shared_ptr<Turret>> turrets = std::vector<std::shared_ptr<Turret>>();
				turrets.push_back(std::make_shared<Turret>(map_six, 0.0f, texture.turretHead, texture.turretBase, Vector2{ SWIDTH - 140,SHEIGHT / 2 - 100 }, texture.bullet));

				turrets.push_back(std::make_shared<Turret>(map_seven, 0.0f, texture.turretHead, texture.turretBase, Vector2{ 654,89 }, texture.bullet));
				turrets.push_back(std::make_shared<Turret>(map_seven, 0.0f, texture.turretHead, texture.turretBase, Vector2{ SWIDTH - 140,SHEIGHT / 2 - 100 }, texture.bullet));
		#pragma endregion
				//Load main spawn map for main game
				loadLevel(map, beginner_map);

				while (!WindowShouldClose() && STATE == GAME)
				{
					//change music based on if player is outside or inside of cave
					if (current_map == map_one && currentMusic.get() != &sound.gameMusic) {
						currentMusic = std::make_unique<Music>(sound.gameMusic);
						PlayMusicStream(*currentMusic);
					}
					else if (current_map == map_two && currentMusic.get() != &sound.cave)
					{
						currentMusic = std::make_unique<Music>(sound.cave);
						PlayMusicStream(*currentMusic);
					}

					UpdateMusicStream(*currentMusic);


					//Check MAIN.H
					HandleFullScreen();
			
					Update(*player, NPCLIST, map, pickables, events, sound, fade, turrets);
					Draw(*player, NPCLIST, map, pickables, texture, events, fade, turrets);
				}
		#pragma endregion

		#pragma region ENDGAME
				currentMusic = std::make_unique<Music>(sound.gameMusic);
				objectiveProgress = 0;


				while (!WindowShouldClose() && STATE == END_GAME)
				{
					UpdateMusicStream(*currentMusic);

					BeginDrawing();

					ClearBackground(RAYWHITE);

					DrawTile(map, Vector2{ 0,0 });

					GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
					GuiSetFont(labelFont);

					if (wasAbducted)
					{
						GuiLabel(btnLabel, "YOU WERE ABDUCTED!");
					}
					else {
						GuiSetFont(labelFont);
						GuiLabel(btnLabel, (gameFinished ? "YOU ESCAPED SAFELY" : "YOU DIED!"));
					}

					GuiSetFont(buttonFont);
					if (GuiButton(btnStartGame, "Start Game")) {
						STATE = GAME;
					}

					if (GuiButton(btnExit, "Exit")) {
						CloseWindow();
					}

					EndDrawing();
				}

				gameFinished = false;
				wasAbducted = false;
				collectedWhiteKey = false;
				collectedGoldKey = false;
		#pragma endregion
	}

	//Unlolad Textures
	UnloadTexture(tilesetTexture);
	texture.UnloadTextures();

	CloseWindow();

	return 0;
}

