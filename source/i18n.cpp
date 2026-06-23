/**
 * @file i18n.cpp
 * @brief 简体中文国际化字符串表
 */

#include "i18n.h"

static std::map<std::string, std::string> s_strings = {
    // 应用
    {"app_title", "Switch 家长控制管理工具"},
    {"app_version", "v1.0"},

    // 状态显示
    {"status_title", "📊 当前状态"},
    {"status_pin_length", "PIN 长度"},
    {"status_restriction_on", "已启用 ✓"},
    {"status_restriction_off", "已禁用 ✗"},
    {"status_time_limit_set", "已设置 ({0})"},
    {"status_time_limit_none", "未设置"},
    {"status_remaining", "剩余 {0}"},
    {"status_timer_stopped", "计时器已停止"},

    // 菜单
    {"menu_title", "⚙️ 功能菜单"},
    {"menu_time_limit", "设置游玩时长限制"},
    {"menu_time_limit_desc", "设置每日可游玩时长"},
    {"menu_toggle", "启用/禁用家长控制"},
    {"menu_toggle_desc", "打开或关闭家长控制功能"},
    {"menu_change_pin", "修改 PIN 码"},
    {"menu_change_pin_desc", "调用系统界面修改 PIN"},
    {"menu_delete_config", "删除所有配置"},
    {"menu_delete_config_desc", "忘记 PIN 时恢复出厂设置"},
    {"menu_unlink_app", "解除手机 App 绑定"},
    {"menu_unlink_app_desc", "解除与手机应用的配对"},

    // 按钮
    {"btn_refresh", "刷新状态"},
    {"btn_help", "使用帮助"},
    {"btn_confirm", "确认"},
    {"btn_cancel", "取消"},
    {"btn_apply", "应用"},
    {"btn_back", "返回"},

    // 开关状态
    {"enabled", "已启用"},
    {"disabled", "已禁用"},

    // 确认对话框
    {"confirm_title", "确认操作"},
    {"confirm_delete_title", "⚠️ 确认删除"},
    {"confirm_delete", "此操作将删除所有家长控制配置，包括 PIN 码。\n此操作不可撤销！\n\n确定要继续吗？"},
    {"confirm_unlink_title", "⚠️ 确认解除绑定"},
    {"confirm_unlink", "此操作将解除与手机 App 的绑定。\n解除后需要重新配对才能使用手机管理。\n\n确定要继续吗？"},

    // Toast 消息
    {"toast_success", "操作成功"},
    {"toast_failed", "操作失败"},
    {"toast_not_implemented", "功能待实现"},

    // 时间格式化
    {"hours", "小时"},
    {"minutes", "分钟"},
    {"seconds", "秒"},
    {"days", "天"},

    // 帮助
    {"help_title", "使用帮助"},
    {"help_content", "1. 使用方向键导航菜单\n2. 按 A 键选择\n3. 按 B 键返回\n4. 时长限制支持 1 分钟步进"},

    // 错误
    {"error_init_failed", "pctl 服务初始化失败，请在真实 Switch 上运行"},
    {"error_not_implemented", "功能待实现"},

    // 时长设置界面
    {"time_limit_title", "设置游玩时长限制"},
    {"time_limit_same_every_day", "每天相同"},
    {"time_limit_per_day", "每天不同"},
    {"time_limit_current", "当前设置"},
    {"time_limit_hours", "小时"},
    {"time_limit_minutes", "分钟"},
    {"time_limit_preset", "快捷设置"},
};

std::string i18n_get(const std::string& key) {
    auto it = s_strings.find(key);
    if (it != s_strings.end()) {
        return it->second;
    }
    return key;  // 找不到时返回 key 本身（便于调试）
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
