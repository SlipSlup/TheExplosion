
#include <memory>
#include <application.hpp>
#include <wtypes.h>

class App : public TheExplosion::Application { virtual void on_update() override {} };

int GetDesktopWidth() {

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    return desktop.right;

}

int GetDesktopHeight() {

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    return desktop.bottom;

}

int main() {

    auto app = std::make_unique<App>();
	int returnCode = app->start(GetDesktopWidth() - 2, GetDesktopHeight() - 2, "TheExplosion");

}
