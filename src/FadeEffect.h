#pragma once
#include "includes.h"
#include <functional>

class Player;
class FadeEffect;

class FadeEffect {
private:
    float fadeAlpha;
    float fadeSpeed;
    bool fadeEffectActive;
    bool fadeIn;
    std::function<void(Player&)> onFadeIn;

public:
    FadeEffect(float speed, std::function<void(Player&)> onFadeIn) : fadeAlpha(0.0f), fadeSpeed(speed), fadeEffectActive(false), fadeIn(true), onFadeIn(onFadeIn){}

    void startFadeIn() {
        fadeIn = true;
        fadeEffectActive = true;
        fadeAlpha = 0.0f; 
    }

    void startFadeOut() {
        fadeIn = false;
        fadeEffectActive = true;
        fadeAlpha = 1.0f;
    }


    //fades in and fades out from 0-1 and fade out from 1 to 0, call lambda function after fade in completes to do its specific operation
    void update(Player& player) {
        if (!fadeEffectActive) return;

        if (fadeIn) {
            fadeAlpha += fadeSpeed; 
            if (fadeAlpha >= 1.0f) {
                fadeAlpha = 1.0f;
                fadeEffectActive = false;

                onFadeIn(player);
                this->startFadeOut();
            }
        }
        else {
            fadeAlpha -= fadeSpeed;
            if (fadeAlpha <= 0.0f) {
                fadeAlpha = 0.0f;
                fadeEffectActive = false;
            }
        }
    }


    /// <summary>
    /// draw the fade in
    /// </summary>
    void draw() const {
        if (fadeAlpha > 0) {
            Color fadeColor = Fade(BLACK, fadeAlpha);
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fadeColor);
        }
    }


    //check if effect is active
    bool isActive() const {
        return fadeEffectActive;
    }
};