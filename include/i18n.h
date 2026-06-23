/**
 * @file i18n.h
 * @brief Internationalization support for Chinese strings
 *
 * This module provides a simple key-value based string lookup
 * for Chinese (Simplified) translations used in the application.
 */

#ifndef SWITCH_PCTL_GUI_I18N_H_
#define SWITCH_PCTL_GUI_I18N_H_

#include <string>

/**
 * @brief Get internationalized string by key
 * @param key String key (e.g., "app_title", "btn_confirm")
 * @return Translated string, or the key itself if not found
 */
std::string i18n_get(const std::string& key);

/**
 * @brief Check if a key exists in the string table
 * @param key String key to check
 * @return true if key exists, false otherwise
 */
bool i18n_has_key(const std::string& key);

/**
 * @brief Get all available keys (for debugging)
 * @return Comma-separated string of all keys
 */
std::string i18n_get_all_keys();

#endif  // SWITCH_PCTL_GUI_I18N_H_
