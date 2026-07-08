#include "FpsManager.hpp"

#include <SDL3/SDL_timer.h>

bool FPSManager::setFramerate(Uint32 rate) {
    if (rate < FPS_LOWER_LIMIT || rate > FPS_UPPER_LIMIT) {
        return false;
    }
    this->frameCount = 0;
    this->rate = rate;
    this->rateTicks = (1000.0f / static_cast<float>(rate));
    return true;
}

int FPSManager::getFramerate() const {
    return this->rate;
}

int FPSManager::getFrameCount() const {
    return this->frameCount;
}

Uint64 FPSManager::framerateDelay() {
    frameCount++;

    Uint64 currentTime = SDL_GetTicks();
    Uint64 elapsed = currentTime - lastTicks;
    lastTicks = currentTime;

    Uint64 targetTicks = baseTicks + static_cast<Uint64>(frameCount * rateTicks);
    if (currentTime <= targetTicks) {
        SDL_Delay(static_cast<Uint32>(targetTicks - currentTime));
    } else {
        frameCount = 0;
        baseTicks = currentTime;
    }

    return elapsed;
}