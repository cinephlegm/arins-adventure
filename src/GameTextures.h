#pragma once
#include "Includes.h"

class Textures {
public:
    std::vector<Texture2D> coin_textures;
    Texture2D speedPotion;
    Texture2D shield;
    Texture2D healthPotion;
    Texture2D invisibilityPotion;
    Image healthBar;
    Texture2D powerUpBackground;
    Texture2D progressBar;
    Texture2D heart;
    Texture2D invincibleHeart;
    Image objectivesBackground;
    Texture2D objectiveBackgroundtexture;

    Texture2D turretHead;
    Texture2D turretBase;
    Texture2D bullet;

    Texture2D GoldKey;
    Texture2D WhiteKey;

    Texture2D teleporter;

    Textures()
    {
        //load them at the start of texture static declaration
        speedPotion = LoadTexture("sprite/Objects/speedPotion.png");
        shield = LoadTexture("sprite/Objects/shield.png");
        healthPotion = LoadTexture("sprite/Objects/healthPotion.png");
        invisibilityPotion = LoadTexture("sprite/Objects/invisibilityPotion.png");
        healthBar = LoadImage("sprite/UI/healthBar.png");
        powerUpBackground = LoadTexture("sprite/UI/powerBoostBar.png");
        progressBar = LoadTexture("sprite/UI/progressBar.png");
        heart = LoadTexture("sprite/UI/heart.png");
        invincibleHeart = LoadTexture("sprite/UI/invincible.png");
        objectivesBackground = LoadImage("sprite/UI/label.png");

        ImageResize(&objectivesBackground, objectivesBackground.width + 250, objectivesBackground.height + 50);
        objectiveBackgroundtexture = LoadTextureFromImage(objectivesBackground);

        turretHead = LoadTexture("sprite/Turret/turret_FACE_one.png");
        turretBase = LoadTexture("sprite/Turret/turret_BASE.png");
        bullet = LoadTexture("sprite/Turret/turret_BULLET_one.png");

        GoldKey = LoadTexture("sprite/Objects/GoldKey.png");
        WhiteKey = LoadTexture("sprite/Objects/WhiteKey.png");

        teleporter = LoadTexture("sprite/UI/teleporter.png");


        this->coin_textures.resize(0);

        for (int i = 1; i <= COIN_TEXTURE_SIZE; i++)
        {
            std::string filename = "sprite/Objects/coin_" + std::to_string(i) + ".png";
            this->coin_textures.push_back(LoadTexture(filename.c_str()));
        }
    }


    //unload all textures from the game at the end
    void UnloadTextures() {

        UnloadTexture(speedPotion);
        UnloadTexture(shield);
        UnloadTexture(healthPotion);
        UnloadTexture(invisibilityPotion);
        UnloadTexture(powerUpBackground);
        UnloadTexture(progressBar);
        UnloadTexture(heart);
        UnloadTexture(invincibleHeart);
        UnloadTexture(objectiveBackgroundtexture);
        UnloadTexture(GoldKey);
        UnloadTexture(WhiteKey);
        UnloadTexture(turretHead);
        UnloadTexture(turretBase);
        UnloadTexture(bullet);
        UnloadTexture(teleporter);

        for (auto& texture : coin_textures) {
            UnloadTexture(texture);
        }

        UnloadImage(healthBar);
        UnloadImage(objectivesBackground);
    }
};