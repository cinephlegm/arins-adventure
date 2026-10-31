#pragma once

//for initwindow
#define SWIDTH 1920
#define SHEIGHT 1088
#define FPS 60

#define COIN_TEXTURE_SIZE 5


#pragma region LoadedMaps
//current map is the main map loaded
std::string current_map;

//maps that can be loaded and played on
const char* spawn_area = "map/village.tmj";
const char* map_one = "map/world.tmj";
const char* map_two = "map/cave.tmj";
const char* map_three = "map/deepercave.tmj";
const char* map_four = "map/dungeon_entrance_cave.tmj";
const char* map_five = "map/dungeon_one.tmj";
const char* map_six = "map/dungeon_two.tmj";
const char* map_seven = "map/dungeon_three.tmj";
const char* map_eight = "map/dungeon_four.tmj";

#pragma endregion

//custom spawns forthe player based on thwe map hes on
#pragma region MapSpawns

const Vector2 village_spawn = { 800,450 };
const Vector2 enter_village_spawn = { SWIDTH - 150, SHEIGHT / 2 };

const Vector2 enter_map_one_spawn = { 80, 280 };
const Vector2 exit_cave_spawn = { 100,200 };

const Vector2 map_two_spawn = { 100,150 };

const Vector2 map_three_spawn = { SWIDTH / 2 - 100, 980 };
const Vector2 map_four_spawn = { SWIDTH / 2 , 680 };

const Vector2 map_five_spawn = { SWIDTH - 700, 160 };

const Vector2 map_six_spawn = { 100, 100 };
const Vector2 map_seven_spawn = { 100, 200 };

const Vector2 map_eight_spawn = { 100, 200 };
#pragma endregion

#pragma region Objective
bool collectedWhiteKey = false;
bool collectedGoldKey = false;

int coinsToCollect = 6;
int coinsCollected = 0;

bool inCutscene = true;
bool enteredCave = false;
bool collectedEveryGem = false;
bool wentDeeperInTheCave = false;
bool enteredTheDungeon = false;
bool gameFinished = false;
bool wasAbducted = false;
bool isLoadingMap = false;

int objectiveProgress = 0;

std::vector<std::string> objectives = {
	"Head Left",
	"Enter The Cave",
	"Collect All The Gems",
	"Dive Deeper ",
	"Explore Around",
	"LEAVE THE CAVE!",
	"Find a Way Out!",
	"Find Keys",
	"Leave the Catacombs",
	"Return to the village"
};

#pragma endregion


enum GAME_STATE { MAIN_MENU, GAME, END_GAME};

GAME_STATE STATE = MAIN_MENU;