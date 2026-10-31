#pragma once
#include "Includes.h"
#include <filesystem>
#include <map>

enum AnimationState { IDLE, WALKLEFT, WALKRIGHT, WALKUP, WALKDOWN };

class Animations;

class Animations {
private:
	std::vector<std::string> animationdirec;
	std::vector<int> sizeOfAnimations;

	//structure of animation frames to hold list of textures toether 
	struct AnimationFrames {
		std::vector<Texture2D> idleFrames;
		std::vector<Texture2D> walkLeftFrames;
		std::vector<Texture2D> walkRightFrames;
		std::vector<Texture2D> walkDownFrames;
		std::vector<Texture2D> walkUpFrames;
	};
	AnimationFrames frame;

	AnimationState state = IDLE;
	std::map<AnimationState, std::vector<Texture2D>> anims;

	float timeCounter = 0.0f;
	int currentFrame = 0;
	const float frameChangeRate = 0.5f;

	//this loops through the directory given from path, checks all the files insie of it, and if the file is a reglar file add its name to the list animateddirec to be looped through after this loop
	//ifgnore file count was used in old system also got this code from stack overflow, original creater is : John Doggett: https://stackoverflow.com/questions/2802188/file-count-in-a-directory-using-c
	// i just modified it a bit :3
	int countFilesInDirectory(std::string path) { 
		auto dirIter = std::filesystem::directory_iterator(path);
		int fileCount = 0;
		for (auto& entry : dirIter)
		{
			if (entry.is_regular_file())
			{
				animationdirec.push_back(entry.path().string());
				fileCount++;
			}
		}
		return fileCount;
	}

public:
	Animations(std::vector<std::string> directories)
	{
		printf("Setting up Animations \n");
		printf("Loading Animations \n");

		//loop through directories, ignore file size its redundant it was used before but havent bothered removing it when i changed my code system for animations
		for (const auto& directory : directories) {
			auto fileCount = countFilesInDirectory(directory);
			printf("Directory %s loaded with %d files\n", directory.c_str(), fileCount);
		}

		//loop through animationdirec which is a list of directory paths for images that are loaded from a single directory (look at player constructor or npc for better understandng)
		//find the main words for the animatin if found add to specific list like idleFrams for exampl then delete ti from the klist so if it finds another idle it adds the 2nd one instead of alwyays adding the first
		//if none were found iterate to thje next using ++it;
		printf("Adding Animations to list.. \n");
		for (auto it = animationdirec.begin(); it != animationdirec.end(); ) {
			std::string& dir = *it;

			if (dir.find("idle") != std::string::npos) {
				frame.idleFrames.push_back(LoadTexture(dir.c_str()));
				it = animationdirec.erase(it);
			}
			else if (dir.find("left") != std::string::npos) {
				frame.walkLeftFrames.push_back(LoadTexture(dir.c_str()));
				it = animationdirec.erase(it);
			}
			else if (dir.find("right") != std::string::npos) {
				frame.walkRightFrames.push_back(LoadTexture(dir.c_str()));
				it = animationdirec.erase(it);
			}
			else if (dir.find("down") != std::string::npos) {
				frame.walkDownFrames.push_back(LoadTexture(dir.c_str()));
				it = animationdirec.erase(it);
			}
			else if (dir.find("up") != std::string::npos) {
				frame.walkUpFrames.push_back(LoadTexture(dir.c_str()));
				it = animationdirec.erase(it);
			}
			else {
				++it;
			}
		}

		//dictionary based on state to access list of frames based on current state
		anims[IDLE] = frame.idleFrames;
		anims[WALKLEFT] = frame.walkLeftFrames;
		anims[WALKRIGHT] = frame.walkRightFrames;
		anims[WALKDOWN] = frame.walkDownFrames;
		anims[WALKUP] = frame.walkUpFrames;

		printf("Added Animations to list.. \n");
	}


	//getters
	AnimationState GetAnimState() const { return this->state; }

	int GetCurrentFrame() const { return this->currentFrame; }

	AnimationFrames GetAnimationFrame() const { return this->frame; }

	std::map<AnimationState, std::vector<Texture2D>> GetAnimsDictionary()
	{
		return this->anims;
	}

	//update state value based on npc / player current state
	void SetState(AnimationState value)
	{
		this->state = value;
	}

	//update the animatiom based on the currentFrame
	void UpdateAnimations()
	{
		timeCounter += GetFrameTime();

		if (timeCounter >= frameChangeRate) {
			timeCounter = 0.0f;
			currentFrame++;

		}

		if (currentFrame >= anims[state].size()) {
			currentFrame = 0;
		}
	}
};