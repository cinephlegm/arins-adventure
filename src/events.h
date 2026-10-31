#pragma once
#include "Includes.h"
#include <memory>

class BettyNPC;
class Events;
class ShakeScreenEvent;
class AbductionEvent;

class Events {
protected:
	bool isFinished = false;
	bool isEventActive = false;
	int story_progression;
	Rectangle triggerBox;
	std::string map_occurance;
	std::function<void(Player&)> onCompletion;

public:
	Events(Rectangle triggerBox, std::string map_occurance, int story_progression, std::function<void(Player&)> onCompletion)
		: triggerBox(triggerBox), map_occurance(map_occurance), story_progression(story_progression), onCompletion(onCompletion) {}
	//getters
	Rectangle GetTriggerBox() const
	{
		return this->triggerBox;
	}

	std::string GetMap() const {
		return this->map_occurance;
	}

	bool GetEventActive() const
	{
		return this->isEventActive;
	}

	bool GetFinished() const
	{
		return this->isFinished;
	}

	bool CanTriggerEvent() const
	{
		return objectiveProgress >= story_progression;
	}

	//setter
	void SetEventActive(bool value)
	{
		this->isEventActive = value;
	}

	virtual void EventTrigger(SoundManager& sound, Player& player) = 0;
	virtual void EventTrigger(SoundManager& sound) = 0;
	virtual void EventTrigger(Player& player) = 0;
	virtual void EventTrigger() = 0;

	virtual ~Events() = default;
};

class ShakeScreenEvent : public Events
{
private:
	float shakeDuration;
	float shakeIntensity;
	Vector2 shakeOffset;

public:

	ShakeScreenEvent
	(Rectangle triggerBox, std::string map_occurance, Vector2 shakeOffset, float shakeIntensity, float shakeDuration, int story_progression,std::function<void(Player&)> onCompletion) :
		Events(triggerBox, map_occurance, story_progression,onCompletion),
		shakeOffset(shakeOffset), shakeIntensity(shakeIntensity), shakeDuration(shakeDuration) {
	}

	Vector2 GetShakeOffset() const
	{
		return this->shakeOffset;
	}

	//override method for its own implementation
	void EventTrigger(SoundManager& sound,Player& player) override {
		//play sound of earthquake
		Sound earthquake = sound.earthquake;
		//shake duration from x to 0 and generate a random number from negative x to positive x 
		if (this->shakeDuration > 0) {
			if (!IsSoundPlaying(earthquake)) PlaySound(earthquake);
			this->shakeDuration -= GetFrameTime();
			this->shakeOffset.x = static_cast<float>(GetRandomValue(static_cast<int>(-this->shakeIntensity), static_cast<int>(this->shakeIntensity)));
			this->shakeOffset.y = static_cast<float>(GetRandomValue(static_cast<int>(-this->shakeIntensity), static_cast<int>(this->shakeIntensity)));

		}
		else {
			//stop event
			this->shakeOffset = { 0.0f, 0.0f };
			this->isFinished = true;
			this->isEventActive = false;
			if (IsSoundPlaying(earthquake)) StopSound(earthquake);

			onCompletion(player);
		}
	}

	void EventTrigger(SoundManager& sound) override {}
	void EventTrigger(Player& player) override {}
	void EventTrigger() override {}

	virtual ~ShakeScreenEvent() = default;
};

class AbductionEvent : public Events
{
private:
	float abductionSpeed;
	std::unique_ptr<BettyNPC> abductor;

public:
	AbductionEvent(Rectangle triggerBox, std::string map_occurance, int story_progression, std::function<void(Player&)> onCompletion,float abductionSpeed) : 
		Events(triggerBox,map_occurance,story_progression,onCompletion), abductionSpeed(abductionSpeed),abductor(std::make_unique<BettyNPC>(1.0f, Vector2{ 9999,9999 }, 150.0f, Vector2{ 9999,9999 }, Vector2{ 9999,9999 }, map_one)) {}

	void EventTrigger(Player& player) override
	{
		
		this->isFinished = false;
		this->isEventActive = true;

		//set npc location to player oposition and just add to x position of player based on abdution speed
		player.SetPosition(Vector2Add(player.GetPosition(), Vector2{ abductionSpeed,0 }));
		abductor->SetPosition(player.GetPosition());

		//if player is off screen stop event
		if (player.GetPosition().x >= SWIDTH)
		{
			onCompletion(player);
		}
		
	}

	//draw event
	void draw()
	{
		abductor->Draw(Vector2{0,0});
	}

	void EventTrigger(SoundManager& sound, Player& player) override {}
	void EventTrigger(SoundManager& sound) override {}
	void EventTrigger() override {}

	virtual ~AbductionEvent() = default;
};


class TeleportEvent : public Events {
	Texture2D teleporterSprite;

public:
	TeleportEvent(Rectangle triggerBox, std::string map_occurance, int story_progression, std::function<void(Player&)> onCompletion, Texture2D teleporterSprite)
		: Events(triggerBox, map_occurance, story_progression, onCompletion), teleporterSprite(teleporterSprite) {
	}

	void EventTrigger(Player& player) override {
		this->isFinished = false;
		this->isEventActive = true;

		//make player go up pretending he got teleported in a 2d environment
		player.SetPosition(Vector2Add(player.GetPosition(), Vector2{ 0, -3 }));

		//if player rwacher position 0 stop event
		if (player.GetPosition().y < 0) {
			this->isFinished = true;
			this->isEventActive = false;
			onCompletion(player);
		}
	}

	//getter for sprite of teleporter 
	Texture2D GetTexture() const { return this->teleporterSprite; }

	void EventTrigger(SoundManager& sound, Player& player) override {}
	void EventTrigger(SoundManager& sound) override {}
	void EventTrigger() override {}
};