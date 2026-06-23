#include "i18n.h"
#include <map>
#include <vector>

static std::map<std::string, std::string> s_strings = {
    {"app_title",       "家长控制管理"},
    {"status_pin",      "PIN 信息"},
    {"status_restriction","限制状态"},
    {"status_time",     "时长限制"},
    {"status_remaining","剩余时间"},
    {"menu_toggle",     "启用限制"},
    {"menu_time_limit", "时长设置"},
    {"menu_pin_change", "修改 PIN"},
    {"menu_delete",     "删除配置"},
    {"menu_unlink",     "解除绑定"},
    {"enabled",         "已启用"},
    {"disabled",        "已禁用"},
    {"unlimited",       "无限制"},
    {"minutes",         "分钟"},
    {"btn_confirm",     "确认"},
    {"btn_cancel",      "取消"},
    {"hint_refresh",    "刷新"},
    {"hint_back",       "返回"},
    {"time_title",      "时长设置"},
    {"time_set_5",      "5 分钟"},
    {"time_set_15",     "15 分钟"},
    {"time_set_20",     "20 分钟"},
    {"time_set_30",     "30 分钟"},
    {"time_set_custom", "自定义"},
    {"pin_display",     "显示 PIN"},
    {"pin_hidden",      "PIN 已隐藏"},
    {"pin_length_fmt",  "PIN 长度: {}"},
    {"error_pctl",      "家长控制服务不可用"},
    {"error_generic",   "操作失败"},
    {"success",         "操作成功"},
    {"confirm_delete",  "确定要删除家长控制配置吗？此操作不可撤销。"},
    {"confirm_unlink",  "确定要解除绑定吗？"},
    {"time_current_fmt","当前: {} 分钟"},
    {"time_remaining_fmt","剩余: {} 分钟"},
};

std::string i18n_get(const std::string& key) {
    auto it = s_strings.find(key);
    if (it != s_strings.end()) {
        return it->second;
    }
    return key;
}

bool i18n_has_key(const std::string& key) {
    return s_strings.find(key) != s_strings.end();
}

std::vector<std::string> i18n_get_all_keys() {
    std::vector<std::string> keys;
    for (const auto& pair : s_strings) {
        keys.push_back(pair.first);
    }
    return keys;
}
