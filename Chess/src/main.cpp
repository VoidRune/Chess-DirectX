#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    Application* App = new Application;
    App->Run();
    delete App;
    return 0;
}