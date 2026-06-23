/**
 * @file pctl_manager.cpp
 * @brief Implementation of PctlManager singleton class
 */

#include "pctl_manager.h"
#include <algorithm>

// ============================================================================
// Singleton Implementation
// ============================================================================

PctlManager& PctlManager::getInstance() {
    static PctlManager instance;
    return instance;
}

PctlManager::PctlManager() : m_initialized(false) {
    // Constructor is private - initialization must be explicit via initialize()
}

PctlManager::~PctlManager() {
    if (m_initialized) {
        shutdown();
    }
}

// ============================================================================
// Initialization and Shutdown
// ============================================================================

bool PctlManager::initialize() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        return true;  // Already initialized
    }
    
    Result rc = pctlInitialize();
    if (R_FAILED(rc)) {
        return false;
    }
    
    m_initialized = true;
    return true;
}

bool PctlManager::isInitialized() const {
    return m_initialized;
}

void PctlManager::shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        pctlExit();
        m_initialized = false;
    }
}

// ============================================================================
// Status Query Methods
// ============================================================================

int PctlManager::getPinLength() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return 0;
    }
    
    int length = 0;
    Result rc = pctlGetPinCodeLength(&length);
    return R_SUCCEEDED(rc) ? length : 0;
}

bool PctlManager::getRestrictionEnabled() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    PlayTimerSettings settings;
    Result rc = pctlGetPlayTimerSettings(&settings);
    if (R_FAILED(rc)) {
        return false;
    }
    
    // Correct: Read raw[1] to check restriction status
    // This is the time-limit level switch, not the system-level switch
    return settings.raw[1] != 0;
}

bool PctlManager::hasTimeLimit() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    PlayTimerSettings settings;
    Result rc = pctlGetPlayTimerSettings(&settings);
    if (R_FAILED(rc)) {
        return false;
    }
    
    // Check raw[2] through raw[8] for non-zero values (daily limits)
    for (int i = 2; i <= 8; i++) {
        if (settings.raw[i] != 0) {
            return true;
        }
    }
    
    return false;
}

int64_t PctlManager::getRemainingPlayTime() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return -1;
    }
    
    int64_t remaining = 0;
    Result rc = pctlGetPlayTimerRemaining(&remaining);
    return R_SUCCEEDED(rc) ? remaining : -1;
}

uint8_t PctlManager::getTimerState() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return 0xFF;  // Error value
    }
    
    uint8_t state = 0;
    Result rc = pctlGetPlayTimerState(&state);
    return R_SUCCEEDED(rc) ? state : 0xFF;
}

bool PctlManager::getPlayTimerSettings(PlayTimerSettings* settings) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || settings == nullptr) {
        return false;
    }
    
    Result rc = pctlGetPlayTimerSettings(settings);
    return R_SUCCEEDED(rc);
}

int64_t PctlManager::getTimeLimit() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return 0;
    }
    
    PlayTimerSettings settings;
    Result rc = pctlGetPlayTimerSettings(&settings);
    if (R_FAILED(rc)) {
        return 0;
    }
    
    // Read raw[2] (Monday's limit, assumed same for all days if sameEveryDay=true)
    // The raw value is assumed to be in minutes, convert to nanoseconds
    uint16_t rawValue = settings.raw[2];
    if (rawValue == 0) {
        return 0;
    }
    
    // Convert minutes to nanoseconds
    return static_cast<int64_t>(rawValue) * 60 * 1000000000LL;
}

// ============================================================================
// Time Limit Configuration Methods
// ============================================================================

bool PctlManager::setDailyTimeLimit(int hours, int minutes, bool sameEveryDay) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    // Validate input
    if (hours < 0 || hours > 24 || minutes < 0 || minutes > 59) {
        return false;
    }
    
    // Get current settings
    PlayTimerSettings settings;
    Result rc = pctlGetPlayTimerSettings(&settings);
    if (R_FAILED(rc)) {
        return false;
    }
    
    // Convert time to nanoseconds
    int64_t limit_ns = static_cast<int64_t>(hours) * 3600 * 1000000000LL +
                       static_cast<int64_t>(minutes) * 60 * 1000000000LL;
    
    if (sameEveryDay) {
        // Apply same limit to all 7 days (raw[2] to raw[8])
        // Note: The actual encoding format may vary, this is a simplified version
        for (int i = 2; i <= 8; i++) {
            settings.raw[i] = static_cast<uint16_t>(limit_ns / 1000000000LL / 60);  // Convert to minutes
        }
    }
    
    // Enable restriction
    settings.raw[1] = 1;
    
    // Apply settings
    rc = pctlSetSettings(&settings);
    return R_SUCCEEDED(rc);
}

bool PctlManager::clearTimeLimit() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    // Get current settings
    PlayTimerSettings settings;
    Result rc = pctlGetPlayTimerSettings(&settings);
    if (R_FAILED(rc)) {
        return false;
    }
    
    // Clear all daily limits (raw[2] to raw[8])
    for (int i = 2; i <= 8; i++) {
        settings.raw[i] = 0;
    }
    
    // Disable restriction
    settings.raw[1] = 0;
    
    // Apply settings
    rc = pctlSetSettings(&settings);
    return R_SUCCEEDED(rc);
}

// ============================================================================
// Parental Control Switch Methods
// ============================================================================

bool PctlManager::setRestrictionEnabled(bool enabled) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    // Get current settings
    PlayTimerSettings settings;
    Result rc = pctlGetPlayTimerSettings(&settings);
    if (R_FAILED(rc)) {
        return false;
    }
    
    // Write to raw[1] (correct field for restriction enable/disable)
    settings.raw[1] = enabled ? 1 : 0;
    
    // Apply settings (pctlSetSettings will handle internal reinit if needed)
    rc = pctlSetSettings(&settings);
    return R_SUCCEEDED(rc);
}

// ============================================================================
// PIN Management Methods
// ============================================================================

bool PctlManager::launchPinChangeApplet() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    Result rc = pctlLaunchParentalControlApplet();
    return R_SUCCEEDED(rc);
}

// ============================================================================
// Dangerous Operations
// ============================================================================

bool PctlManager::deleteAllConfig() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    // TODO: Implement actual delete config logic
    // This may require calling specific pctl commands or using pctlRequestPairingDevice
    // For now, return false to indicate not implemented
    return false;
}

bool PctlManager::unlinkParentalControl() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return false;
    }
    
    // TODO: Implement unlink logic
    // This may involve pctlRequestPairingDevice or similar
    // For now, return false to indicate not implemented
    return false;
}
