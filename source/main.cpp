#include <borealis.hpp>

// Minimal test: just show a label, no pctl, no custom activity
// If this works → crash is in our pctl/UI code
// If this crashes → crash is in borealis init/build config

int main(int argc, char* argv[]) {
    // Step 1: Init borealis
    if (!brls::Application::init()) {
        return EXIT_FAILURE;
    }

    // Step 2: Create window
    brls::Application::createWindow("Switch PCTL");

    // Step 3: Push a minimal activity — just a label
    brls::Label* label = new brls::Label();
    label->setText("Hello from borealis!");
    label->setFontSize(24);
    label->setHorizontalAlign(brls::HorizontalAlign::CENTER);
    label->setVerticalAlign(brls::VerticalAlign::CENTER);
    brls::Application::pushActivity(new brls::Activity(label));

    // Step 4: Run main loop
    while (brls::Application::mainLoop()) {
    }

    return EXIT_SUCCESS;
}
