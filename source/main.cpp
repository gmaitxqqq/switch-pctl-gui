#include <borealis.hpp>
#include "main_activity.h"
#include "pctl_manager.h"
#include "i18n.h"

int main(int argc, char* argv[]) {
    // Init borealis framework
    if (!brls::Application::init()) {
        return EXIT_FAILURE;
    }

    // Create window with Chinese title
    brls::Application::createWindow(i18n_get("app_title"));

    // Init pctl service
    PctlManager& pctl = PctlManager::getInstance();
    if (!pctl.init()) {
        // pctl service init failed - app will still show but with error status
    }

    // Push main activity
    brls::Application::pushActivity(new MainActivity());

    // Run the main loop
    while (brls::Application::mainLoop()) {
    }

    return EXIT_SUCCESS;
}
