#include "pctl_manager.h"
#include <switch.h>
#include <cstdio>

bool PctlManager::init() {
    std::lock_guard<std::mutex> lock(mtx);

    Result rc = pctlInitialize();
    if (R_FAILED(rc)) {
        available = false;
        cachedSettings.restriction_enabled = false;
        cachedSettings.rating_age = 0;
        cachedSettings.sns_restriction = false;
        cachedSettings.free_comm_restriction = false;
        return false;
    }

    available = true;

    // Read initial settings
    bool enabled = false;
    rc = pctlIsRestrictionEnabled(&enabled);
    cachedSettings.restriction_enabled = R_SUCCEEDED(rc) && enabled;

    PctlRestrictionSettings libnxSettings = {};
    rc = pctlGetCurrentSettings(&libnxSettings);
    if (R_SUCCEEDED(rc)) {
        cachedSettings.rating_age            = libnxSettings.rating_age;
        cachedSettings.sns_restriction       = libnxSettings.sns_post_restriction;
        cachedSettings.free_comm_restriction = libnxSettings.free_communication_restriction;
    }

    return true;
}

bool PctlManager::isRestrictionEnabled() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!available) return false;

    bool enabled = false;
    Result rc = pctlIsRestrictionEnabled(&enabled);
    cachedSettings.restriction_enabled = R_SUCCEEDED(rc) && enabled;
    return cachedSettings.restriction_enabled;
}

bool PctlManager::getCurrentSettings(PlayTimerSettings* settings) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!available || !settings) return false;

    // Refresh restriction enabled status
    bool enabled = false;
    Result rc = pctlIsRestrictionEnabled(&enabled);
    if (R_SUCCEEDED(rc)) {
        cachedSettings.restriction_enabled = enabled;
    }

    // Get restriction settings
    PctlRestrictionSettings libnxSettings = {};
    rc = pctlGetCurrentSettings(&libnxSettings);
    if (R_SUCCEEDED(rc)) {
        cachedSettings.rating_age            = libnxSettings.rating_age;
        cachedSettings.sns_restriction       = libnxSettings.sns_post_restriction;
        cachedSettings.free_comm_restriction = libnxSettings.free_communication_restriction;
    }

    *settings = cachedSettings;
    return true;
}

u8 PctlManager::getRatingAge() {
    std::lock_guard<std::mutex> lock(mtx);
    return cachedSettings.rating_age;
}

int PctlManager::getPlayTimerMinutes() {
    // Play timer data is not available via standard libnx pctl API
    // Returns 0 (unlimited) until raw IPC implementation is added
    std::lock_guard<std::mutex> lock(mtx);
    return 0;
}

// ── Stubbed setters (need raw IPC implementation) ──

bool PctlManager::setRestrictionEnabled(bool enabled) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!available) return false;

    // pctlEnableRestriction / pctlDisableRestriction are NOT in standard libnx pctl.h
    // Use raw IPC via serviceDispatch on IParentalControlService
    // Cmd 2 = EnableRestriction, Cmd 3 = DisableRestriction
    Service* srv = pctlGetServiceSession_Service();
    u32 cmdId = enabled ? 2 : 3;
    Result rc = serviceDispatch(srv, cmdId);

    if (R_SUCCEEDED(rc)) {
        cachedSettings.restriction_enabled = enabled;
    }
    return R_SUCCEEDED(rc);
}

bool PctlManager::setPlayTimerMinutes(int minutes) {
    // Play timer settings require raw IPC (cmd 1000+ range)
    // Stub: not yet implemented
    return false;
}

bool PctlManager::showPin() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!available) return false;

    // PIN display uses system applet launch via appletRequestLaunch
    // This requires knowing the PIN viewer applet ID
    // Stub: not yet implemented properly
    // For now, try libnx's pctlShowPin if it exists (it might in newer libnx)
    // If it doesn't exist, the linker will fail and we'll handle it
    return false;
}

bool PctlManager::deleteConfig() {
    // Requires raw IPC
    // Stub: not yet implemented
    return false;
}
