/**
 * @file main.cpp
 * @brief Switch 家长控制图形化管理工具 - 入口
 */

#include <borealis.hpp>
#include "main_frame.h"
#include "pctl_manager.h"
#include "i18n.h"
#include "toast.h"

#ifdef __SWITCH__
#include <switch.h>
#endif

int main(int argc, char* argv[]) {
    // 初始化 borealis
    if (!brls::Application::init("Switch 家长控制管理工具")) {
        std::cerr << "Failed to initialize borealis" << std::endl;
        return 1;
    }

#ifdef __SWITCH__
    romfsInit();
#endif

    // 加载中文字体
    // 将思源黑体放入 romfs:/fonts/SourceHanSansCN-Regular.otf
    // borealis 会通过 NanoVG 加载字体
    // 注意：borealis 的字体加载方式取决于版本
    // 新版本使用 Application::loadFont()
#ifdef __SWITCH__
    // 检查字体文件是否存在
    FILE* fontFile = fopen("romfs:/fonts/SourceHanSansCN-Regular.otf", "rb");
    if (fontFile != nullptr) {
        fclose(fontFile);
        // borealis 会通过 addFont() 或类似方法加载
        // 这里用 NanoVG 的 addFont 方式
        // 实际加载在 Application::init() 之后、run() 之前完成
        std::cout << "Chinese font found in romfs" << std::endl;
    } else {
        std::cout << "Chinese font not found, using default font" << std::endl;
    }
#endif

    // 初始化 pctl 服务
    PctlManager& pctlMgr = PctlManager::getInstance();
    if (!pctlMgr.initialize()) {
        std::cerr << "Warning: pctl service init failed (may be running on PC)" << std::endl;
    }

    // 启动主界面
    brls::Application::pushFrame(new MainFrame());

    // 主循环
    while (brls::Application::mainLoop()) {
    }

    // 清理
    if (pctlMgr.isInitialized()) {
        pctlMgr.shutdown();
    }

#ifdef __SWITCH__
    romfsExit();
#endif

    brls::Application::exit();
    return 0;
}
