#pragma once
#include "Includes.h"
#include "Turret.h"
#include "Character.h"

class Bullet;

class Bullet {
private:
	Rectangle hitbox;
	Vector2 position;
	Vector2 velocity;
	float damage;
	Texture2D texture;
	bool isActive;
public:

	Bullet(Vector2 pos, Vector2 vel, float dmg, Texture2D texture) :
		position(pos), velocity(vel), damage(dmg), isActive(true), texture(texture) {
		this->hitbox = { position.x,position.y,static_cast<float>(texture.width),static_cast<float>(texture.height) };
	}

	bool GetIsActive() const
	{
		return this->isActive;
	}

	void SetIsActive(bool value) {
		this->isActive = value;
	}

	int GetBulletDamage() { return this->damage; }

	Rectangle GetRect() const { return this->hitbox; }

	void Update(Player& player) {
		if (!this->isActive) {
			return;
		}

		//moves bulelt towards player
		this->position = Vector2MoveTowards(position, player.GetPosition(), velocity.x);

		//update rect hitbox position based on its new position
		this->hitbox.x = this->position.x;
		this->hitbox.y = this->position.y;


		//if its outside of map deactive it
		if (position.x < 0 || position.x > GetScreenWidth() ||
			position.y < 0 || position.y > GetScreenHeight()) {
			this->isActive = false;
		}
	}


	//draw bullet
	void Draw() {
		if (this->isActive) {
			DrawTextureEx(texture, position, 0, 1, WHITE);
		}
	}
};