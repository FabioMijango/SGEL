#pragma once
#include <SDL3/SDL_stdinc.h>

/// Minimum supported frame rate
#define FPS_LOWER_LIMIT		1
/// Maximum supported frame rate
#define FPS_UPPER_LIMIT		200
/// Default frame rate
#define FPS_DEFAULT		30

/**
 * @class FPSManager
 * @brief Manages frame rate timing and synchronization.
 *
 * Tracks frame count and calculates frame timing delays to maintain
 * a constant target frame rate. Uses SDL timer functions internally.
 *
 * @see Engine
 */
class FPSManager {
    /// Frame counter for current second
    Uint32  frameCount = 0;
    /// Base ticks at start of timing measurement
    Uint64  baseTicks = 0;
    /// Ticks from last frame
    Uint64  lastTicks = 0;
    /// Calculated milliseconds per frame
    float   rateTicks = (1000.0f / static_cast<float>(FPS_DEFAULT));
    /// Target frame rate in FPS
    Uint32  rate = FPS_DEFAULT;

public:
    FPSManager() = default;

    /**
     * @brief Sets the target frame rate.
     * @param rate Target FPS (clamped to FPS_LOWER_LIMIT to FPS_UPPER_LIMIT).
     * @return true if set successfully, false if out of range.
     */
    bool setFramerate(Uint32 rate);

    /**
     * @brief Gets the current target frame rate.
     * @return Frame rate in FPS.
     */
    int getFramerate() const;

    /**
     * @brief Gets the current frame count since program start.
     * @return Number of frames rendered.
     */
    int getFrameCount() const;

    /**
     * @brief Calculates the delay needed to maintain target frame rate and call SDL_Delay to wait that long.
     * @return Milliseconds since last frame.
     * @note Call once per frame in the main loop.
     */
    Uint64 framerateDelay();
};
