#include <iostream>
#include <format>

#include "memory.hpp"

#include <Windows.h>
#include <d3d11.h>
#include <dwmapi.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

namespace offsets {
    constexpr auto local_player = 0xDE7964;
    constexpr auto entity_list = 0x4DFCE74;
    constexpr auto view_matrix = 0x4DEDCA4;
    constexpr auto dwClientState = 0x59F194;
    constexpr auto m_iFov = 0x31F4;

    constexpr auto bone_matrix = 0x26A8;
    constexpr auto team_num = 0xF4;
    constexpr auto life_state = 0x25F;
    constexpr auto origin = 0x138;
    constexpr auto dormant = 0xED;
    constexpr auto Health = 0x100;
}

struct Vector {
    Vector() noexcept
        : x(), y(), z() {}

    Vector(float x, float y, float z) noexcept
        : x(x), y(y), z(z) {}

    Vector& operator+(const Vector& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& operator-(const Vector& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    float x, y, z;
};

struct ViewMatrix {
    ViewMatrix() noexcept
        : data() {}

    float* operator[](int index) noexcept {
        return data[index];
    }

    const float* operator[](int index) const noexcept {
        return data[index];
    }

    float data[4][4];
};

static bool world_to_screen(const Vector& world, Vector& screen, const ViewMatrix& vm) noexcept {
    float w = vm[3][0] * world.x + vm[3][1] * world.y + vm[3][2] * world.z + vm[3][3];

    if (w < 0.001f) {
        return false;
    }

    const float x = world.x * vm[0][0] + world.y * vm[0][1] + world.z * vm[0][2] + vm[0][3];
    const float y = world.x * vm[1][0] + world.y * vm[1][1] + world.z * vm[1][2] + vm[1][3];

    w = 1.f / w;
    float nx = x * w;
    float ny = y * w;

    const ImVec2 size = ImGui::GetIO().DisplaySize;

    screen.x = (size.x * 0.5f * nx) + (nx + size.x * 0.5f);
    screen.y = -(size.y * 0.5f * ny) + (ny + size.y * 0.5f);

    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
        return 1L;
    }

    switch (message) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0L;
    }
    }

    return DefWindowProc(window, message, w_param, l_param);
}

bool create_directx(HWND window) {

}


// application entry point
INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    // allocate this program a console
    if (!AllocConsole()) {
        return FALSE;
    }












    // redirect stream IO
    FILE* file{ nullptr };
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);

    // try get the process ID
    DWORD pid = memory::get_process_id(L"csgo.exe");

    // wait for the game
    if (!pid) {
        std::cout << "Waiting for CS:GO...\n";

        do {
            pid = memory::get_process_id(L"csgo.exe");
            Sleep(200UL);
        } while (!pid);
    }

    std::cout << std::format("Game found. Process ID = {}\n", pid);

    const DWORD client = memory::get_module_address(pid, L"client.dll");

    if (!client) {
        std::cout << "Failed to get game module.\n";
        FreeConsole();
        return FALSE;
    }

    std::cout << std::format("Client -> {:#x}\n", client);
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;

    std::cout <<
        "   ______      ______                    _____                                                            " << endl <<
        " .~      ~.  .~      ~.  |`````````,  .-~     ~.        .'.       |..          |                          " << endl <<
        "|           |          | |'''|'''''  :                .''```.     |  ``..      |                          " << endl <<
        "|           |          | |    `.     :     _____    .'       `.   |      ``..  |                          " << endl <<
        " `.______.'  `.______.'  |      `.    `-._____.'| .'           `. |          ``|                          " << endl <<
        "                                                                                                          " << endl <<
        " ____                                     ____                                                            " << endl <<
        "|             ``..      ..'' `````|````` |            |`````````, |..          |       .'.       |        " << endl <<
        "|______           ``..''          |      |______      |'''|'''''  |  ``..      |     .''```.     |        " << endl <<
        "|                 ..'`..          |      |            |    `.     |      ``..  |   .'       `.   |        " << endl <<
        "|___________  ..''      ``..      |      |___________ |      `.   |          ``| .'           `. |_______ ";
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;

    int crosshair = 1;
    int BoxESP = 1;
    int Headdot = 1;
    int FOV = 1;
    int FOV2 = 110;
    //* // just add a flash at the start to get the code back

    std::cout << "Would you like a crosshair? 0 No, 1 Yes" << endl;

    std::cin >> crosshair;

    std::cout << "Thanks! Your crosshair is " << crosshair << endl;
    std::cout << "" << endl;
    Sleep(500);
    

    std::cout << "Would you like Box ESP?" << endl;

    std::cin >> BoxESP;

    std::cout << "Thanks! Your Box ESP is " << BoxESP << endl;
    std::cout << "" << endl;

    Sleep(500);


    std::cout << "Would you like a Head Dot?" << endl;

    std::cin >> Headdot;

    std::cout << "Thanks! Your Head Dot is " << Headdot << endl;
    std::cout << "" << endl;
    Sleep(500);


    std::cout << "Would you like to change your fov?" << endl;
    std::cout << "" << endl;
    std::cin >> FOV;

    if (FOV == 1) {
        std::cout << "Enter your Fov" << endl;
        std::cin >> FOV2;
        std::cout << "Thanks! Your FOV is " << FOV2 << endl;
        std::cout << "" << endl;
    }
    Sleep(500);
    //*/
    std::cout << "Restart to change your settings..." << endl;


    if (!FreeConsole()) {
        return FALSE;
    }

    const HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!handle) {
        return FALSE;
    }

    // create our window class to specify options for our window
    const WNDCLASSEXW wc{
        .cbSize = sizeof(WNDCLASSEXW),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_procedure,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = instance,
        .hIcon = nullptr,
        .hCursor = nullptr,
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = L"External Overlay Class",
        .hIconSm = nullptr
    };

    // register it and make sure it succeeded
    if (!RegisterClassExW(&wc)) {
        return FALSE;
    }

    // create our window
    const HWND window = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        wc.lpszClassName,
        L"External Overlay",
        WS_POPUP,
        0,
        0,
        1920,
        1080,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    // make sure the window was created successfully
    if (!window) {
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    // set the window's opacity
    if (!SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA)) {
        DestroyWindow(window);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    {
        RECT client_area{};
        if (!GetClientRect(window, &client_area)) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }

        RECT window_area{};
        if (!GetWindowRect(window, &window_area)) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }

        POINT diff{};
        if (!ClientToScreen(window, &diff)) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }

        const MARGINS margins{
            window_area.left + (diff.x - window_area.left),
            window_area.top + (diff.y - window_area.top),
            client_area.right,
            client_area.bottom
        };

        if (FAILED(DwmExtendFrameIntoClientArea(window, &margins))) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }
    }

    // create our directx swap chain description
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferDesc.Width = 0U;
    sd.BufferDesc.Height = 0U;
    sd.BufferDesc.RefreshRate.Numerator = 30U;
    sd.BufferDesc.RefreshRate.Denominator = 1U;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    sd.SampleDesc.Count = 1U;
    sd.SampleDesc.Quality = 0U;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2U;
    sd.OutputWindow = window;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL feature_levels[2]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL feature_level{};

    // directx variables
    ID3D11Device* device{ nullptr };
    ID3D11DeviceContext* device_context{ nullptr };
    IDXGISwapChain* swap_chain{ nullptr };
    ID3D11RenderTargetView* render_target_view{ nullptr };

    // create the swap chain & device
    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0U,
        feature_levels,
        2U,
        D3D11_SDK_VERSION,
        &sd,
        &swap_chain,
        &device,
        &feature_level,
        &device_context))) {
        DestroyWindow(window);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    ID3D11Texture2D* back_buffer{ nullptr };

    if (FAILED(swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer)))) {
        return FALSE;
    }

    // create our render target
    if (FAILED(device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view))) {
        return FALSE;
    }

    if (back_buffer) {
        back_buffer->Release();
    }

    // tell windows to show this window
    ShowWindow(window, cmd_show);
    UpdateWindow(window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, device_context);

    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        const auto local_player = memory::read<DWORD>(handle, client + offsets::local_player);
        if (local_player) {
            const auto local_team = memory::read<int>(handle, local_player + offsets::team_num);
            const auto view_matrix = memory::read<ViewMatrix>(handle, client + offsets::view_matrix);

            for (int i = 1; i < 32; ++i) {
                const auto player = memory::read<DWORD>(handle, client + offsets::entity_list + i * 0x10);


                if (!player) {
                    continue;
                }

                DWORD LocalPlayer;
                ReadProcessMemory(handle, (LPVOID)(client + offsets::local_player), &LocalPlayer, sizeof(DWORD), 0);
                    int LocalPlayerHealth;
                    ReadProcessMemory(handle, (LPVOID)(LocalPlayer + offsets::Health), &LocalPlayerHealth, sizeof(int), 0);

                    if (LocalPlayerHealth <= 0) {
                        continue;
                    }

                const auto bones = memory::read<DWORD>(handle, player + offsets::bone_matrix);

                if (!bones) {
                    continue;
                }


                Vector head_pos{
                    memory::read<float>(handle, bones + 0x30 * 8 + 0x0C),
                    memory::read<float>(handle, bones + 0x30 * 8 + 0x1C),
                    memory::read<float>(handle, bones + 0x30 * 8 + 0x2C)
                };
                
                Vector top;
                Vector bottom;

    
                const auto LocalP = memory::read<DWORD>(handle, client + offsets::local_player);
                const auto EntityP = player;

                Vector local_pos{
                    memory::read<float>(handle, LocalP + offsets::origin),
                    memory::read<float>(handle, LocalP + offsets::origin + 0x4),
                    memory::read<float>(handle, LocalP + offsets::origin + 0x8),
                };

                Vector entity_pos{
                    memory::read<float>(handle, EntityP + offsets::origin),
                    memory::read<float>(handle, EntityP + offsets::origin + 0x4),
                    memory::read<float>(handle, EntityP + offsets::origin + 0x8),
                };

                
                float _dist = (float)trunc(sqrt(pow((entity_pos.x - local_pos.x), 2) + pow((entity_pos.y - local_pos.y), 2)) / 100);


                if (crosshair == 1) {
                    {
                            ImGui::GetBackgroundDrawList()->AddRectFilled({ 962, 539 }, { 968 , 541 }, ImColor(.8f, 0.f, 0.f));
                            ImGui::GetBackgroundDrawList()->AddRectFilled({ 958, 539 }, { 952 , 541 }, ImColor(.8f, 0.f, 0.f)); 
                            ImGui::GetBackgroundDrawList()->AddRectFilled({ 959, 537 }, { 961, 532 }, ImColor(.8f, 0.f, 0.f));
                            ImGui::GetBackgroundDrawList()->AddRectFilled({ 959, 543 }, { 961, 548 }, ImColor(.8f, 0.f, 0.f));

                        }

                    if (FOV == 1) {
                        
                            DWORD LocalBase = memory::read<DWORD>(handle, client + offsets::local_player);
                            memory::write(handle, LocalBase + offsets::m_iFov, FOV2);
                        }


                if (memory::read<bool>(handle, player + offsets::dormant)) {
                    continue;
                }

                if (memory::read<int>(handle, player + offsets::team_num) == local_team) {
                    continue;
                }

                if (memory::read<int>(handle, player + offsets::life_state) != 0) {
                    continue;
                }

             

                



                




                auto feet_pos = memory::read<Vector>(handle, player + offsets::origin);

               


                if (Headdot == 1) {
                    if (world_to_screen(head_pos + Vector{ 0, 0, 0.f }, top, view_matrix) && world_to_screen(head_pos - Vector{ 0, 0, 0.f }, bottom, view_matrix)) {
                        const float h = bottom.y - top.y;
                        const float w = h * 0.35f;
                        const float distance = _dist * 0.0254f;


                        ImGui::GetBackgroundDrawList()->AddCircle({ top.x - w, top.y }, static_cast<int>(1.5 / sqrt(distance)) , ImColor(1.f, 1.f, 1.f));

                    }
                }
                if (BoxESP == 1) {
                    if (world_to_screen(head_pos + Vector{ 0, 0, 11.f }, top, view_matrix) && world_to_screen(feet_pos - Vector{ 0, 0, 7.f }, bottom, view_matrix)) {
                        const float h = bottom.y - top.y;
                        const float w = h * 0.35f;


                        ImGui::GetBackgroundDrawList()->AddRect({ top.x - w, top.y }, { top.x + w, bottom.y }, ImColor(1.f, 1.f, 1.f));
                    }
                }
            }
        }
    }

        ImGui::Render();



        constexpr float clear_color[4] = { 0.f, 0.f, 0.f, 0.f };
        device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
        device_context->ClearRenderTargetView(render_target_view, clear_color);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        swap_chain->Present(0U, 0U);
    }





        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (swap_chain) {
            swap_chain->Release();
            swap_chain = nullptr;
        }

        if (device_context) {
            device_context->Release();
            device_context = nullptr;
        }

        if (device) {
            device->Release();
            device = nullptr;
        }

        if (render_target_view) {
            render_target_view->Release();
            render_target_view = nullptr;
        }

        DestroyWindow(window);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);

        CloseHandle(handle);

        return TRUE;
    }
