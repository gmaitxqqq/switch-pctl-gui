#include <borealis.hpp>
#include "pctl_manager.h"
#include "main_activity.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // 1. Initialize borealis first
    if (!brls::Application::init()) {
        brls::Logger::error("Failed to initialize borealis");
        return EXIT_FAILURE;
    }

    // 2. Initialize pctl service (after borealis — ns, nifm, pl may be needed)
    PctlManager::instance().initialize();

    // 3. Create window and push activity
    brls::Application::createWindow("Switch 家长控制");
    brls::Application::pushActivity(new MainActivity());

    // 4. Main loop
    while (brls::Application::mainLoop()) {}

    // 5. Cleanup (order reversed)
    PctlManager::instance().finalize();
    brls::Application::quit();

    return EXIT_SUCCESS;
}
