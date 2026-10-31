#pragma once
#include "Includes.h"
#include "define.h"
#include "Bullet.h"
#include "Character.h"
#include <functional>

//forward declaration
class Turret;


class Turret {
private:
	std::string map;
	float fireCooldown = 0;
	float fireThreshold = 3.0f;
	Texture2D bullet;
	Texture2D turretHead;
	Texture2D turretBase;
	Vector2 position;
	float turretAngle;

	std::vector<std::shared_ptr<Bullet>> bullets;

private:
	void Fire() {
		bullets.push_back(std::make_shared<Bullet>(Bullet(position, { 20,20 }, 1, bullet)));
	}

public:
	Turret(std::string map, float fireSpeed, Texture2D turretHead, Texture2D turretBase, Vector2 Position, Texture2D bullet) :
		map(map), fireCooldown(fireSpeed), turretHead(turretHead), turretBase(turretBase), position(Position), bullet(bullet), bullets(std::vector<std::shared_ptr<Bullet>>()) {}

	std::vector<std::shared_ptr<Bullet>> GetTurretBullets() {
		return this->bullets;
	}

	std::string GetMap() const
	{
		return this->map;
	}


	//every 3 seconds generate a new bullet class and add it to a list of bullets (vector) and then upsate the bullet if the player isnt invisible, if player changes map clear the list
	void Update(Player& player) {
		fireCooldown += GetFrameTime();
		if (this->map == current_map) {

			if (fireCooldown >= fireThreshold) {
				Fire();
				fireCooldown = 0;
			}

			for (auto& bullet : bullets) {
				if (!player.GetInvisible()) {
					bullet->Update(player);
				}
			}
		}
		else {
			bullets.clear();
		}
	}


	//if bullet collides with player or terrain delete it
	void RemoveBullet(const std::shared_ptr<Bullet>& targetBullet) {
		auto it = std::find_if(bullets.begin(), bullets.end(),
			[&targetBullet](const std::shared_ptr<Bullet>& bullet) {
				return bullet == targetBullet;
			});
		if (it != bullets.end()) {
			bullets.erase(it);
		}
	}

	//draw the turret rotating towards player, if player is invisible dont draw it following the playuer
	void Draw(Player& player) {
		if (this->map == current_map) {
			if (!player.GetInvisible())
			{
				Vector2 direction = Vector2Subtract(player.GetPosition(), this->position);
				float angle = atan2(direction.y, direction.x) * (180 / PI);
				turretAngle = angle - 90;
			}

			for (auto& bullet : bullets) {
				bullet->Draw();
			}

			DrawTextureEx(this->turretHead, this->position, turretAngle, 1, WHITE);
		}
	}

};