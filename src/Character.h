#pragma once
#include "FadeEffect.h"
#include "Includes.h"
#include "Animations.h"
#include "GameTextures.h"
#include <memory>


class Character;
class Player;
class NPC;
class BettyNPC;

class Character
{
protected:
	float size;
	Vector2 position;
	Rectangle rect;
	float speed;
	float originalSpeed;
	std::unique_ptr<Animations> animations;
	float collidedTime;

	std::string map;

	bool collided;

	bool isInvisible = false;
	float invisibilityTime;
	float invisibilityTimeMax;

	float invincibilityTime;
	float invincibilityFrames = 3.0f;
	bool isInvincible = false;

public:
	Character() {}
	Character(float size, Vector2 position, float speed, std::string map)
		: size(size), position(position), speed(speed), originalSpeed(speed), map(map)
	{
		this->originalSpeed = speed;
		rect = { position.x, position.y, size, size };
	}

	// Getters
	float GetSize() const { return this->size; }
	Vector2 GetPosition() const { return this->position; }
	Rectangle GetRect() const { return this->rect; }
	float GetSpeed() const { return this->speed; }
	std::string GetMap() const { return this->map; }
	bool IsInvincible() const { return this->isInvincible; }

	// Setters
	void SetPosition(Vector2 value) { this->position = value; }
	void SetRectWidthHeight(Vector2 value) { this->rect.width = value.x; this->rect.height = value.y; }
	void SetRect(Rectangle value) { this->rect = value; }
	void SetSpeed(float value) { this->speed = value; }

	void UpdateMap(std::string value) { this->map = value; }

	virtual void Update(Character& character)
	{
		//get current frame
		int currentFrame = this->animations->GetCurrentFrame();
		std::vector<Texture2D> animDict = this->animations->GetAnimsDictionary()[this->animations->GetAnimState()];
		//get current animationm

		//update current animation frame

		this->animations->UpdateAnimations();


		//if currentFrame isnt larger than vectorsize then divide the size of the width and height by 2 of rectable, gives a more accurate representation where player / npc is on map
		if (currentFrame < animDict.size())
		{
			this->rect.width = (float)(animDict[currentFrame].width / 2);
			this->rect.height = (float)(animDict[currentFrame].height / 2);
		}

		//update char rect for position
		this->rect.x = this->position.x;
		this->rect.y = this->position.y;

		//if player collided with npc if you check lower there is a tape fix solution which i did but yeah old code so kinda redundant but keeping it just in case as a back up if it ever is meant to happen
		if (character.GetMap() == current_map)
		{
			collided = (CheckCollisionRecs(character.GetRect(), this->rect) ? true : false);

			this->speed = (this->collided ? 0 : this->originalSpeed);
		}


	}

	//draw current animation, if player or npc is invisible or invincible lowe their alpha level
	virtual void Draw(Vector2 shakeOffset)
	{
		int currentFrame = this->animations->GetCurrentFrame();
		std::vector<Texture2D> animDict = this->animations->GetAnimsDictionary()[this->animations->GetAnimState()];

		float alpha = 1;

		if (this->isInvisible || this->isInvincible)
		{
			alpha = 0.5;
		}

		this->animations->UpdateAnimations();

		DrawTextureEx(animDict[currentFrame], Vector2Add(this->position, shakeOffset), 0, size, Fade(WHITE, alpha));

	}

	virtual void Draw(Textures textures, Vector2 shakeOffset) {}

	virtual ~Character() {}
};

class Player : public Character
{
private:
	int maxHp;
	int health;

	bool isShielded = false;
	bool isSpedUp = false;
	bool isSpeedUpSet = false;

	float shieldedTime;
	float SpeedUpTime;

	float shieldTimeMax;
	float speedTimeMax;

	float speedMultiplyer = 2;

	std::vector<Texture2D> powerUps;

	std::unique_ptr<FadeEffect> end_game_fade;

public:

	Player(float size, Vector2 position, float speed, int health, std::string map)
		: Character(size, position, speed, map), maxHp(health), health(maxHp)
	{
		this->powerUps.resize(0);

		this->rect = Rectangle{ this->position.x,this->position.y,this->size,this->size };

		std::vector<std::string> directories(5);

		//Directory files to load animations from check #aninations.h for a better explenation
		directories[0] = "sprite/Player/Player_Idle/";
		directories[1] = "sprite/Player/Player_Walk_Left/";
		directories[2] = "sprite/Player/Player_Walk_Right/";
		directories[3] = "sprite/Player/Player_Walk_Up/";
		directories[4] = "sprite/Player/Player_Walk_Down/";

		this->animations = std::make_unique<Animations>(directories);

		end_game_fade = std::make_unique<FadeEffect>(0.01f,[](Player& player) {
			STATE = END_GAME;
			});
	}

	void Update(Character& character) override
	{
		//update player animation state based on what button hes pressing
		if (IsKeyDown(KEY_RIGHT)) this->animations->SetState(WALKRIGHT);
		else if (IsKeyDown(KEY_UP)) this->animations->SetState(WALKUP);
		else if (IsKeyDown(KEY_DOWN)) this->animations->SetState(WALKDOWN);
		else if (IsKeyDown(KEY_LEFT)) this->animations->SetState(WALKLEFT);
		else this->animations->SetState(IDLE);

		//make player not be able to go out of bounds
		if (this->position.x < 0) this->position.x = 0;
		if (this->position.x + this->rect.width >= SWIDTH) this->position.x = SWIDTH - this->rect.width;
		if (this->position.y < 0) this->position.y = 0;
		if (this->position.y >= SHEIGHT) this->position.y = SHEIGHT;

		//call base class
		Character::Update(character);


		//if collided with npc and is stuck then teleprot him out but this im gonnabe honest i think when i impemented my "abduction event from events.h this solved the collision issue lol so this is kinda
		//redundant but ill keep it here just in case XD (IF IT WORKS DO NOT TOCUH IT :3)
		if (collided)
		{
			collidedTime += GetFrameTime();

			if (collidedTime >= 2.5)
			{
				//this code is so old it only worked with map one which isnt even the base map anymore goes to show how much of a tape fix this is XD
				this->position = (current_map == map_one and map == map_one ? enter_map_one_spawn : map_two_spawn);
			}
		}
		else {
			collidedTime = 0;
		}

		//if player dies play fadw and update it so it fades in and out
		this->end_game_fade->update(*this);
	}

	#pragma region Getters

		int GetHealth() const { return this->health; }

		int GetMaxHealth() const { return this->maxHp; }

		bool GetInvisible() const { return this->isInvisible; }
	#pragma endregion



	#pragma region Setters
		void SetShielded(bool value) { this->isShielded = value; }

		void SetSpedUp(bool value) { this->isSpedUp = value; }

		void SetInvisible(bool value) { this->isInvisible = value; }

		void SetHealth(int value) { this->health = value; }

		void SetMaxShieldTime(float value) {
			this->shieldTimeMax = value;
		}

		void DamagePlayer(int value)
		{
			if (!isInvincible && !isShielded)
			{
				this->health -= value;

				this->isInvincible = true;
			}


			if (this->health <= 0)
			{
				this->end_game_fade->startFadeIn();
			}
		}

		void SetMaxSpeedTime(float value) { this->speedTimeMax = value; }

		void SetupMaxInvisibilityTime(float value) { this->invisibilityTimeMax = value; }
	#pragma endregion

	//handles power ups
	void DrawPowerUps(Textures texture)
	{
		HandlePowerUp(isShielded, shieldedTime, shieldTimeMax, [this](Textures tex) {
			Vector2 bubble = { this->position.x - 10, this->position.y - 10 };
			DrawTextureEx(tex.shield, bubble, 0, 1.5, WHITE);
			PushBackTexture(tex.shield);
			}, texture,
			[this](Textures tex) {
				DeletePowerFromList(tex.shield);
				});

		HandlePowerUp(isSpedUp, SpeedUpTime, speedTimeMax, [this](Textures tex) {
			if (!isSpeedUpSet)
			{
				this->isSpeedUpSet = true;
				this->SetSpeed(this->GetSpeed() * speedMultiplyer);
			}
			PushBackTexture(tex.speedPotion);
			}, texture,
			[this](Textures tex) {
				DeletePowerFromList(tex.speedPotion);
				this->isSpeedUpSet = false;
				});

		HandlePowerUp(isInvisible, invisibilityTime, invisibilityTimeMax, [this](Textures tex) {
			PushBackTexture(tex.invisibilityPotion);
			}, texture,
			[this](Textures tex) {
				DeletePowerFromList(tex.invisibilityPotion);
				});

		HandlePowerUp(isInvincible, invincibilityTime, invincibilityFrames, [this](Textures tex) {
			PushBackTexture(tex.invincibleHeart);
			}, texture,
			[this](Textures tex) {
				DeletePowerFromList(tex.invincibleHeart);
				});
	}

	//draw player and other stuff related to player
	void Draw(Textures texture, Vector2 shakeOffset)
	{

		Character::Draw(shakeOffset);

		DrawPowerUps(texture);

		DrawHearts(texture, shakeOffset);

		this->end_game_fade->draw();
	}


	//draw player heart
	void DrawHearts(const Textures texture, const Vector2 offset) const
	{
		float spacing = 30;
		float xDistanceBetweenHearts = 30;

		for (int i = 0; i < this->health; i++)
		{
			float heartX = 20 + i * (texture.heart.width + xDistanceBetweenHearts);
			DrawTextureEx(texture.heart, Vector2Add(Vector2{ heartX, 10 }, offset), 0, 2, WHITE);
		}
	}


	//if true, += timer for threshold time, call lambda fucntion to do stuff while ontimer else when finishes reset everything or do something else after with ontimerend lambda fucnction
	void HandlePowerUp(bool& powerup, float& timer, float threshold, std::function<void(Textures)> onTimer, Textures textures, std::function<void(Textures)> onTimerEnd)
	{
		if (powerup)
		{
			timer += GetFrameTime();

			if (timer > threshold)
			{
				timer = 0;
				powerup = false;
				onTimerEnd(textures);
				return;
			}

			onTimer(textures);
			DrawPowerUps(timer, threshold, textures);
		}
	}


	//if power up texture isnt in list add it else dont, so its only unique power ups being drawn and not the same stuff
	void PushBackTexture(Texture2D texture)
	{

		for (int i = 0; i < this->powerUps.size(); i++)
		{
			if (texture.id == powerUps[i].id)
			{
				return;
			}
		}

		printf("ADDED TEXTURE TO LIST \n");
		this->powerUps.push_back(texture);

	}

	void DrawPowerUps(float& timer, float& maxTime, Textures texture)
	{
		float spacing = 30;
		float textSpacing = spacing + 15;

		float progress = timer / maxTime;

		float scalingFactor = (float)(texture.progressBar.width);

		float progressRate = 2.0f;

		progress = fmin(progress + (GetFrameTime() * progressRate), 1.0f); // Limit progress to 1.0


		//  copying image and then resize it so its a progress bar type of thing, failed to fully implement it as when you resize,
		//  the image stretched soo didn replicate it that well :c but hey i tried :3 and then draw the textures in order
		for (int i = 0; i < this->powerUps.size(); i++)
		{
			Vector2 backgroundPos = { 0, 40 + i * (texture.powerUpBackground.height + spacing) };
			DrawTextureEx(texture.powerUpBackground, backgroundPos, 0, 1.5, WHITE);

			DrawTextureEx(texture.progressBar, Vector2{ backgroundPos.x, backgroundPos.y + textSpacing }, 0, 1, WHITE);

			float healthBarWidth = progress * scalingFactor;

			Image resizedHealthBar = ImageCopy(texture.healthBar);

			ImageResize(&resizedHealthBar, static_cast<int>(healthBarWidth), texture.healthBar.height);

			Texture2D healthBarTexture = LoadTextureFromImage(resizedHealthBar);

			// Calculate the adjusted position based on the resizing factor and the width difference
			float adjustedX = backgroundPos.x + (texture.healthBar.width - resizedHealthBar.width) / 3.5f;

			// Draw the resized health bar at the adjusted position
			DrawTextureEx(healthBarTexture, Vector2{ adjustedX, backgroundPos.y + textSpacing - 1.3f }, 0, 1, WHITE);


			bool isTextureShieldBubble = this->powerUps[i].height == texture.shield.height; // Check height for y-axis
			Vector2 powerUpPos = { backgroundPos.x + (texture.powerUpBackground.width - this->powerUps[i].width) / 2 + (isTextureShieldBubble ? 8 : 5), backgroundPos.y + (isTextureShieldBubble ? 8 : 10) };
			DrawTextureEx(this->powerUps[i], powerUpPos, 0, (isTextureShieldBubble ? 1 : 1.5f), WHITE);
		}
	}


	//remove textue of power from list after the timer ran out
	void DeletePowerFromList(Texture2D texture)
	{
		int it = 0;

		for (int i = 0; i < this->powerUps.size(); i++)
		{
			if (texture.id == this->powerUps[i].id)
			{
				break;
			}

			it++;
		}
		this->powerUps.erase(this->powerUps.begin() + it);
	}
};

class NPC : public Character
{
private:
	Vector2 pointA;
	Vector2 pointB;
	std::string currentPoint;

public:
	NPC() {}
	NPC(float size, Vector2 position, float speed, Vector2 pointA, Vector2 pointB, std::string map)
		: Character(size, position, speed, map), currentPoint("A"), pointA(pointA), pointB(pointB)
	{}

	//overrides update of base class, goes from point A to be and vice versa and set the animation state based on what direction the npc is going

	virtual void Update(Character& character) override
	{
		if (currentPoint == "A") {
			this->position.x -= speed * GetFrameTime();
			this->animations->SetState(WALKLEFT);
		}
		else if (currentPoint == "B") {
			this->position.x += speed * GetFrameTime();
			this->animations->SetState(WALKRIGHT);

		}
		else {
			this->animations->SetState(IDLE);
		}

		if (this->position.x <= pointA.x && !collided) {
			currentPoint = "B";
		}
		else if (this->position.x >= pointB.x and !collided) {
			currentPoint = "A";
		}
		else if (collided)
		{
			currentPoint = (currentPoint == "A" ? "B" : "A");


			if (currentPoint == "A")
			{
				this->position.x -= 10;
			}
			else if (currentPoint == "B") {
				this->position.x += 10;
			}

			if (currentPoint == "A" && this->position.x < pointA.x)
			{
				this->position.x = pointA.x;
			}
			else if (currentPoint == "B" && this->position.x > pointB.x) {
				this->position.x = pointB.x;
			}
		}


		Character::Update(character);
	}

	virtual void Draw(Vector2 shakeOffset) override
	{
		Character::Draw(shakeOffset);
	}

	virtual void Draw(Textures texture, Vector2 shakeOffset)
	{
		Character::Draw(shakeOffset);
	}

};

class BettyNPC : public NPC {
public:
	BettyNPC() {}
	BettyNPC(float size, Vector2 position, float speed, Vector2 pointA, Vector2 pointB, std::string map) : NPC(size, position, speed, pointA, pointB, map)
	{
		std::vector<std::string> directories(5);

		//Directory files to load animations from check #aninations.h for a better explenation
		directories[0] = "sprite/NPC/BettyNPC/Betty_Idle";
		directories[1] = "sprite/NPC/BettyNPC/Betty_Walk_Left";
		directories[2] = "sprite/NPC/BettyNPC/Betty_Walk_Right/";
		directories[3] = "sprite/NPC/BettyNPC/Betty_Walk_Up/";
		directories[4] = "sprite/NPC/BettyNPC/Betty_Walk_Down/";

		printf("NPC Animation Dictionaries Added \n");

		animations = std::make_unique<Animations>(directories);
	}

	void Update(Character& character) override
	{
		NPC::Update(character);
	}

	void Draw(Vector2 shakeOffset) override
	{
		Character::Draw(shakeOffset);
	}

	void Draw(Textures texture, Vector2 shakeOffset)
	{
		Character::Draw(shakeOffset);
	}
};