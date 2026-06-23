#pragma once

#include <switch.h>
#include <mutex>
#include <string>

// PctlRestrictionSettings is defined in libnx pctl.h:
//   u8 rating_age; bool sns_post_restriction; bool free_communication_restriction;

// Our extended settings struct (parsed from service data)
struct PlayTimerSettings {
    bool restriction_enabled = false;
    u8   rating_age          = 0;
    bool sns_restriction     = false;
    bool free_comm_restriction = false;
};

class PctlManager {
public:
    static PctlManager& getInstance() {
        static PctlManager instance;
        return instance;
    }

    bool init();
    bool isAvailable() const { return available; }

    // ── Status getters (using official libnx API) ──
    bool isRestrictionEnabled();
    bool getCurrentSettings(PlayTimerSettings* settings);
    u8   getRatingAge();

    // ── Setters (require raw IPC — stubbed for now) ──
    bool setRestrictionEnabled(bool enabled);   // needs raw IPC cmd 2/3
    bool setPlayTimerMinutes(int minutes);      // needs raw IPC cmd 1000+
    bool showPin();                             // needs raw IPC cmd 50+
    bool deleteConfig();                        // needs raw IPC

private:
    PctlManager() = default;
    ~PctlManager() = default;
    PctlManager(const PctlManager&) = delete;
    PctlManager& operator=(const PctlManager&) = delete;

    bool available = false;
    std::mutex mtx;

    // Cached settings
    PlayTimerSettings cachedSettings;
};
