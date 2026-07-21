#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Basic2D
{
    namespace Tools
    {
        inline void InitCPUUsageTracking()
        {
            static bool g_CpuInited = false;
            static ULARGE_INTEGER g_LastCPU = {};
            static ULARGE_INTEGER g_LastSysCPU = {};
            static ULARGE_INTEGER g_LastUserCPU = {};
            static int g_NumProcessors = 1;
            static HANDLE g_ProcessHandle = nullptr;

            if (g_CpuInited)
                return;

            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            g_NumProcessors = static_cast<int>(sysInfo.dwNumberOfProcessors);

            g_ProcessHandle = GetCurrentProcess();

            FILETIME ftime, fsys, fuser;
            GetSystemTimeAsFileTime(&ftime);
            memcpy(&g_LastCPU, &ftime, sizeof(FILETIME));

            GetProcessTimes(g_ProcessHandle, &ftime, &ftime, &fsys, &fuser);
            memcpy(&g_LastSysCPU, &fsys, sizeof(FILETIME));
            memcpy(&g_LastUserCPU, &fuser, sizeof(FILETIME));

            g_CpuInited = true;
        }

        inline float GetProcessCPUUsagePercent()
        {
            static bool g_CpuInited = false;
            static ULARGE_INTEGER g_LastCPU = {};
            static ULARGE_INTEGER g_LastSysCPU = {};
            static ULARGE_INTEGER g_LastUserCPU = {};
            static int g_NumProcessors = 1;
            static HANDLE g_ProcessHandle = nullptr;

            if (!g_CpuInited)
            {
                InitCPUUsageTracking();
                g_CpuInited = true;
            }

            FILETIME ftime, fsys, fuser;
            ULARGE_INTEGER now, sys, user;

            GetSystemTimeAsFileTime(&ftime);
            memcpy(&now, &ftime, sizeof(FILETIME));

            GetProcessTimes(g_ProcessHandle, &ftime, &ftime, &fsys, &fuser);
            memcpy(&sys, &fsys, sizeof(FILETIME));
            memcpy(&user, &fuser, sizeof(FILETIME));

            ULONGLONG sysDelta = (sys.QuadPart - g_LastSysCPU.QuadPart) +
                                 (user.QuadPart - g_LastUserCPU.QuadPart);
            ULONGLONG timeDelta = now.QuadPart - g_LastCPU.QuadPart;

            float percent = 0.0f;
            if (timeDelta > 0)
            {
                percent = static_cast<float>((static_cast<double>(sysDelta) / static_cast<double>(timeDelta) / g_NumProcessors) * 100.0);
            }

            g_LastCPU = now;
            g_LastSysCPU = sys;
            g_LastUserCPU = user;

            return percent;
        }
    }
}
#endif
