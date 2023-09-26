#include "main.h"

Root* root;

int main() {
    std::wcout.imbue(std::locale("korean"));
    if (fileExists(getCurrentPath() + "\\screen.txt") == false) alert(L"there is no screen file");
    interpret_screen(readFileW(s2ws(getCurrentPath()) + L"\\screen.txt"));
    if (fileExists(getCurrentPath() + "\\script.txt") == false) alert(L"there is no script file");
    std::thread script(interpret_script, readFileW(s2ws(getCurrentPath()) + L"\\script.txt"), TRUE, false, false, false);
    script.detach();
    root->init();
    while (not glfwWindowShouldClose(root->window)) {
        root->process_frame();
    }
    root->quit();
    if (script.joinable()) script.join();
	return 0;
}