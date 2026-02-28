#include <Windows.h>

// XInput 常量
#define XINPUT_DEVTYPE_GAMEPAD          0x01
#define XINPUT_DEVSUBTYPE_GAMEPAD       0x01
#define XINPUT_CAPS_VOICE_SUPPORTED     0x0004

// XInput 结构
struct XINPUT_GAMEPAD
{
    WORD  wButtons;
    BYTE  bLeftTrigger;
    BYTE  bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
};

struct XINPUT_STATE
{
    DWORD dwPacketNumber;
    XINPUT_GAMEPAD Gamepad;
};

struct XINPUT_VIBRATION
{
    WORD wLeftMotorSpeed;
    WORD wRightMotorSpeed;
};

struct XINPUT_CAPABILITIES
{
    BYTE Type;
    BYTE SubType;
    WORD Flags;
    XINPUT_GAMEPAD Gamepad;
    XINPUT_VIBRATION Vibration;
};

struct XINPUT_BATTERY_INFORMATION
{
    BYTE BatteryType;
    BYTE BatteryLevel;
};

struct XINPUT_KEYSTROKE
{
    WORD  VirtualKey;
    WCHAR Unicode;
    WORD  Flags;
    BYTE  UserIndex;
    BYTE  HidCode;
};

// 导出函数
extern "C"
{
    // 序号 2: XInputGetState
    // 作用: 告诉 Skyrim "有一个 Xbox 手柄连接"
    // 实际数据由 SKSE 插件的 IAT Hook 提供
    __declspec(dllexport) DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
    {
        if (!pState || dwUserIndex != 0) {
            return 1167; // ERROR_DEVICE_NOT_CONNECTED
        }

        // 返回空状态（IAT Hook 会拦截并填充真实数据）
        ZeroMemory(pState, sizeof(XINPUT_STATE));
        pState->dwPacketNumber = 1;

        return 0; // ERROR_SUCCESS
    }

    // 序号 3: XInputSetState
    // 作用: 捕获震动请求
    __declspec(dllexport) DWORD WINAPI XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
    {
        (void)dwUserIndex;
        (void)pVibration;

        // TODO: 未来可以在这里转换为 DualSense 震动命令
        // 通过共享内存传递给 SKSE 插件

        return 0;
    }

    // 序号 4: XInputGetCapabilities
    // 作用: 告诉 Skyrim 手柄的能力
    __declspec(dllexport) DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
    {
        (void)dwFlags;

        if (!pCapabilities || dwUserIndex != 0) {
            return 1167;
        }

        // 伪装成 Xbox 360 手柄
        ZeroMemory(pCapabilities, sizeof(XINPUT_CAPABILITIES));
        pCapabilities->Type = XINPUT_DEVTYPE_GAMEPAD;
        pCapabilities->SubType = XINPUT_DEVSUBTYPE_GAMEPAD;
        pCapabilities->Flags = XINPUT_CAPS_VOICE_SUPPORTED;

        // 所有按键和轴都支持
        pCapabilities->Gamepad.wButtons = 0xFFFF;
        pCapabilities->Gamepad.bLeftTrigger = 0xFF;
        pCapabilities->Gamepad.bRightTrigger = 0xFF;
        pCapabilities->Gamepad.sThumbLX = 32767;
        pCapabilities->Gamepad.sThumbLY = 32767;
        pCapabilities->Gamepad.sThumbRX = 32767;
        pCapabilities->Gamepad.sThumbRY = 32767;
        pCapabilities->Vibration.wLeftMotorSpeed = 0xFFFF;
        pCapabilities->Vibration.wRightMotorSpeed = 0xFFFF;

        return 0;
    }

    // 序号 5: XInputEnable
    __declspec(dllexport) void WINAPI XInputEnable(BOOL enable)
    {
        (void)enable;
    }

    // 序号 6: XInputGetDSoundAudioDeviceGuids
    __declspec(dllexport) DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid)
    {
        (void)dwUserIndex;
        (void)pDSoundRenderGuid;
        (void)pDSoundCaptureGuid;
        return 1167;
    }

    // 序号 7: XInputGetBatteryInformation
    __declspec(dllexport) DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation)
    {
        (void)dwUserIndex;
        (void)devType;
        (void)pBatteryInformation;
        return 1167;
    }

    // 序号 8: XInputGetKeystroke
    __declspec(dllexport) DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* pKeystroke)
    {
        (void)dwUserIndex;
        (void)dwReserved;
        (void)pKeystroke;
        return 1167;
    }
}

// DLL 入口
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)hinstDLL;
    (void)lpvReserved;

    if (fdwReason == DLL_PROCESS_ATTACH) {
        // 可选: 输出调试信息
        OutputDebugStringA("[XInputWrapper] Loaded - providing fake Xbox 360 gamepad detection\n");
    }

    return TRUE;
}