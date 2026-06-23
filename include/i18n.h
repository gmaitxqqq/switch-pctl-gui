#pragma once

#include <string>
#include <vector>

std::string i18n_get(const std::string& key);
bool i18n_has_key(const std::string& key);
std::vector<std::string> i18n_get_all_keys();
