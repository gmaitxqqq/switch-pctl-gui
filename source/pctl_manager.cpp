#include "pctl_manager.h"
#include <pctl.h>
#include <mutex>

PctlManager::PctlManager() : available(false) {
    // Don't call pctlInitialize here - do it in main.cpp
}

PctlManager::~PctlManager() {
    // Don't call pctlExit here
}

PctlManager& PctlManager::instance() {
    static PctlManager inst;
    return inst;
}

bool PctlManager::initialize() {
    std::lock_guard<std::mutex> lock(mtx);
    Result rc = pctlInitialize();
    available = R_SUCCEEDED(rc);
    if (available) {
        // Cache initial settings
        pctlGetCurrentSettings(&cachedSettings, sizeof(cachedSettings));
    }
    return available;
}

void PctlManager::finalize() {
    std::lock_guard<std::mutex> lock(mtx);
    if (available) {
        pctlExit();
        available = false;
    }
}

bool PctlManager::isAvailable() {
    std::lock_guard<std::mutex> lock(mtx);
    return available;
}

// ── Status getters (using official libnx API) ──

bool PctlManager::isRestrictionEnabled() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!available) return false;
    bool enabled = false;
    pctlIsRestrictionEnabled(&enabled);
    cachedSettings.restriction_enabled = enabled;
    return enabled;
}

u8 PctlManager::getRatingAge() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!available) return 0;
    pctlGetCurrentSettings(&cachedSettings, sizeof(cachedSettings));
    return cachedSettings.rating_age;
}

// ── Stub implementations (real IPC not yet implemented) ──

int PctlManager::getPlayTimerMinutes() {
    // Stub: real implementation needs raw IPC (GetPlayTimerSettings cmd)
    return 0;  // 0 = unlimited
}

bool PctlManager::setPlayTimerMinutes(int minutes) {
    (void)minutes;
    // Stub
    return false;
}

bool PctlManager::requestPinChange() {
    // Stub: needs raw IPC (RequestPinChange cmd)
    return false;
}

bool PctlManager::requestDeleteRestriction() {
    // Stub: needs raw IPC (RequestDeleteRestriction cmd)
    return false;
}
