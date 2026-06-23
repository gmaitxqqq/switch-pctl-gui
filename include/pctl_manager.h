#pragma once

#include <pctl.h>
#include <mutex>

// Matches libnx PctlSettings struct
typedef struct {
    bool restriction_enabled;
    u8   rating_age;
    u8   __reserved[7];
} PctlRestrictionSettings;

class PctlManager {
public:
    static PctlManager& instance();

    bool initialize();   // Call pctlInitialize()
    void finalize();    // Call pctlExit()

    bool isAvailable();

    // ── Status getters (using official libnx API) ──
    bool isRestrictionEnabled();
    u8   getRatingAge();

    // ── Stubs (real IPC to be implemented later) ──
    int  getPlayTimerMinutes();      // stub → returns 0 (unlimited)
    bool setPlayTimerMinutes(int minutes); // stub
    bool requestPinChange();         // stub
    bool requestDeleteRestriction(); // stub

private:
    PctlManager();
    ~PctlManager();

    bool                   available;
    PctlRestrictionSettings cachedSettings;
    std::mutex             mtx;
};
