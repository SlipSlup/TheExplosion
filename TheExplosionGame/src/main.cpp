
#include <application.hpp>
#include <wtypes.h>

class TheExplosionGame : public TheExplosion::Application { virtual void on_update() override {} };

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

    auto app = std::make_unique<TheExplosionGame>();
	int returnCode = app->start(GetDesktopWidth(), GetDesktopHeight(), "TheExplosion");

}
