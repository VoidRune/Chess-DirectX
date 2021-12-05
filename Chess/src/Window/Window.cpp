#include "Window.h"
#include "../../res/icon/resource.h"
#include "../Vendor/ImGui/imgui.h"
#include "../Vendor/ImGui/imgui_impl_dx11.h"
#include "../Vendor/ImGui/imgui_impl_win32.h"
#include "../Graphics/RenderTarget.h"
HWND Window::hWnd = {};
Keyboard Window::kbd = {};
Mouse Window::mouse = {};
bool Window::isFullscreen = 0;
unsigned int Window::screenWidth = 0;
unsigned int Window::screenHeight = 0;
unsigned int Window::m_Width = 0;
unsigned int Window::m_Height = 0;

Window::Window(int width, int height, const char* name, bool isFullscreen) :
    hInstance(GetModuleHandle(nullptr)),
    windowName(name)
{
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    Window::isFullscreen = isFullscreen;
    if (isFullscreen)
    {
        m_Width = screenWidth;
        m_Height = screenHeight;
    }
    else
    {
        m_Width = width;
        m_Height = height;
    }
    WNDCLASSEXA wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEXA));

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pclassName;
    wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

    RegisterClassExA(&wc);

    RECT wr = { 0, 0, m_Width, m_Height };
    AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, FALSE);
    hWnd = CreateWindowExA(
        0,
        pclassName,
        windowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        (screenWidth - m_Width) / 2, (screenHeight - m_Height) / 2,
        wr.right - wr.left, wr.bottom - wr.top,
        nullptr,
        nullptr,
        hInstance,
        this);

    ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
    UnregisterClassA(pclassName, hInstance);
    DestroyWindow(hWnd);
}

HWND Window::GetHandle()
{
    return hWnd;
}

int Window::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
            return 0;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 1;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        mouse.OnLMouseDown();
        mouse.OnLMousePressed();
        break;
    }
    case WM_LBUTTONUP:
    {
        mouse.OnLMouseReleased();
        break;
    }
    case WM_RBUTTONDOWN:
    {
        mouse.OnRMouseDown();
        mouse.OnRMousePressed();
        break;
    }
    case WM_RBUTTONUP:
    {
        mouse.OnRMouseReleased();
        break;
    }
    case WM_MOUSEWHEEL:
    {
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mouse.OnWheelUp();
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mouse.OnWheelDown();
        }
        break;
    }
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
            return 0;
        }
        kbd.OnKeyDown(static_cast<unsigned char>(wParam));
        kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_KEYUP:
    {
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_SIZE:
    {
        m_Width = LOWORD(lParam);
        m_Height = HIWORD(lParam);
        //Width           Height
        Graphics::Resize(m_Width, m_Height);
        RenderTarget::Resize(m_Width, m_Height);
        break;
    }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pWnd->WindowProc(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->WindowProc(hWnd, msg, wParam, lParam);
}