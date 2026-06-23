/**
 * @file pctl_manager.h
 * @brief Singleton manager for Nintendo Switch parental control (pctl) services
 *
 * This class provides a thread-safe interface to the Switch pctl service,
 * handling PIN management, time limits, and restriction settings.
 */

#ifndef SWITCH_PCTL_GUI_PCTL_MANAGER_H_
#define SWITCH_PCTL_GUI_PCTL_MANAGER_H_

#include <switch.h>
#include <mutex>
#include <cstdint>

/**
 * @class PctlManager
 * @brief Singleton class for managing Switch parental control features
 *
 * This class wraps the libnx pctl service functions with proper thread safety
 * and error handling. All pctl calls are serialized via an internal mutex.
 */
class PctlManager {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the PctlManager instance
     */
    static PctlManager& getInstance();

    /**
     * @brief Initialize the pctl service
     * @return true if initialization succeeded, false otherwise
     * @note This calls pctlInitialize() internally
     */
    bool initialize();

    /**
     * @brief Check if pctl service is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

    /**
     * @brief Shutdown the pctl service
     * @note This calls pctlExit() internally
     */
    void shutdown();

    // =========================================================================
    // Status Query Methods
    // =========================================================================

    /**
     * @brief Get the PIN code length
     * @return PIN length in bits (e.g., 4, 6, 8), or 0 on failure
     */
    int getPinLength();

    /**
     * @brief Check if restriction (parental control) is enabled
     * @return true if enabled, false if disabled or error
     * @note This reads raw[1] from PlayTimerSettings, NOT pctlIsRestrictionEnabled()
     */
    bool getRestrictionEnabled();

    /**
     * @brief Check if a time limit is configured
     * @return true if any day has a non-zero time limit
     * @note Checks raw[2] through raw[8] for non-zero values
     */
    bool hasTimeLimit();

    /**
     * @brief Get remaining play time for today
     * @return Remaining time in nanoseconds, or -1 on failure
     */
    int64_t getRemainingPlayTime();

    /**
     * @brief Get the play timer state
     * @return Timer state value (see pctl docs), or 0xFF on failure
     */
    uint8_t getTimerState();

    /**
     * @brief Get full play timer settings
     * @param[out] settings Output parameter for settings
     * @return true if successful, false otherwise
     */
    bool getPlayTimerSettings(PlayTimerSettings* settings);

    /**
     * @brief Get the configured time limit for today
     * @return Time limit in nanoseconds, or 0 if not set / error
     * @note Reads raw[2] from PlayTimerSettings (Monday's limit)
     */
    int64_t getTimeLimit();

    // =========================================================================
    // Time Limit Configuration Methods
    // =========================================================================

    /**
     * @brief Set daily time limit
     * @param hours Hours (0-24)
     * @param minutes Minutes (0-59)
     * @param sameEveryDay If true, apply same limit to all 7 days
     * @return true if successful, false otherwise
     */
    bool setDailyTimeLimit(int hours, int minutes, bool sameEveryDay = true);

    /**
     * @brief Clear all time limits
     * @return true if successful, false otherwise
     */
    bool clearTimeLimit();

    // =========================================================================
    // Parental Control Switch Methods
    // =========================================================================

    /**
     * @brief Enable or disable parental control restriction
     * @param enabled true to enable, false to disable
     * @return true if successful, false otherwise
     * @note This writes to raw[1] of PlayTimerSettings
     */
    bool setRestrictionEnabled(bool enabled);

    // =========================================================================
    // PIN Management Methods
    // =========================================================================

    /**
     * @brief Launch the system PIN change applet
     * @return true if applet launched successfully, false otherwise
     */
    bool launchPinChangeApplet();

    // =========================================================================
    // Dangerous Operations
    // =========================================================================

    /**
     * @brief Delete all parental control configuration
     * @return true if successful, false otherwise
     * @warning This operation is irreversible!
     */
    bool deleteAllConfig();

    /**
     * @brief Unlink the parental control from the mobile app
     * @return true if successful, false otherwise
     */
    bool unlinkParentalControl();

private:
    /**
     * @brief Private constructor for singleton pattern
     */
    PctlManager();

    /**
     * @brief Private destructor
     */
    ~PctlManager();

    /**
     * @brief Delete copy constructor
     */
    PctlManager(const PctlManager&) = delete;

    /**
     * @brief Delete copy assignment operator
     */
    PctlManager& operator=(const PctlManager&) = delete;

    /**
     * @brief Delete move constructor
     */
    PctlManager(PctlManager&&) = delete;

    /**
     * @brief Delete move assignment operator
     */
    PctlManager& operator=(PctlManager&&) = delete;

    // Member variables
    bool m_initialized;           ///< Whether pctl service is initialized
    std::mutex m_mutex;            ///< Mutex for thread-safe pctl calls
};

#endif  // SWITCH_PCTL_GUI_PCTL_MANAGER_H_
