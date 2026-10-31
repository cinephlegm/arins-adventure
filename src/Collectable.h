#pragma once
#include "Character.h"

class Pickable;
class SpeedBoost;
class Shield;
class HealthPotion;
class Coin;
class InvisibilityPotion;
class GoldKey;
class WhiteKey;

class Pickable {
private:
	Texture2D texture;
	Vector2 spawnPosition;
	Rectangle rect;
	std::string map_to_spawn;

public:
	Pickable() {}
	Pickable(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : texture(texture), spawnPosition(spawnPosition), map_to_spawn(map_to_spawn) {
		this->rect = Rectangle{ this->spawnPosition.x, this->spawnPosition.y, (float)texture.width * 2, (float)texture.height  * 4};
	} 

	std::string GetSpawnMap() const { return this->map_to_spawn; }

	Rectangle GetRect() const { return this->rect; }

	virtual void Pickup(Player& character) {}

	virtual void Pickup() { }

	void Draw(Vector2 shakeOffset)
	{
		DrawTextureEx(this->texture, Vector2Add(this->spawnPosition,shakeOffset), 0, 1, WHITE);
	}

	virtual ~Pickable() = default;
};

/// <summary>
/// Each class down here inherits public Pickable and override method Pickup for their own implementation
/// </summary>

class SpeedBoost : public Pickable {
public:
	SpeedBoost(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup(Player& character)override {
		printf("Collected Speed Boost \n");
		character.SetMaxSpeedTime(5);
		character.SetSpedUp(true);
	}
};

class Shield : public Pickable {
public:
	Shield(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup(Player& character) override {
		printf("Collected Shield Bubble \n");
		character.SetMaxShieldTime(6);
		character.SetShielded(true);
	}
};

class HealthPotion : public Pickable {
private:
	int healing = 1;
public:
	HealthPotion(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup(Player& character) override {
		printf("Collected Health Potion \n");

		character.SetHealth(character.GetHealth() + healing);

		if (character.GetHealth() > character.GetMaxHealth())
		{
			character.SetHealth(character.GetMaxHealth());
		}
	}
};

class Coin : public Pickable {
public:
	Coin(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup() override {
		printf("Collected Coin \n");
		coinsCollected++;

		if (coinsCollected >= coinsToCollect)
		{
			collectedEveryGem = true;
			coinsCollected = 0;
		}
	}
};

class InvisibilityPotion : public Pickable {
public:
	InvisibilityPotion(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup(Player& character) override {
		printf("Collected Invisibility Potion \n");
		character.SetupMaxInvisibilityTime(8);
		character.SetInvisible(true);
	}
};

class GoldKey : public Pickable {
public:
	GoldKey(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup() override {
		printf("Collected Red Key \n");
		collectedGoldKey = true;
	}
};

class WhiteKey : public Pickable {
public:
	WhiteKey(Texture2D texture, Vector2 spawnPosition, std::string map_to_spawn) : Pickable(texture, spawnPosition, map_to_spawn) {}
	void Pickup() override {
		printf("Collected White Key \n");
		collectedWhiteKey = true;
	}
};
