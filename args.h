#pragma once

#include <stdbool.h>
#include <stdint.h>

#define ATOOL_VERSION "v1.0.0"

typedef struct
{
    bool GPUIdxProvided;
    uint32_t GPUIdx;
    bool VBIOSFileProvided;
    char* VBIOSFileName;

    bool ShowHardLimitProvided, ShowHardLimitIdxProvided;
    uint32_t ShowHardLimitIdx;
    bool ShowStateProvided, ShowStateIdxProvided;
    uint32_t ShowStateIdx;
    bool ShowSocclkProvided, ShowSocclkIdxProvided;
    uint32_t ShowSocclkIdx;
    bool ShowMclkProvided, ShowMclkIdxProvided;
    uint32_t ShowMclkIdx;
    bool ShowGfxclkProvided, ShowGfxclkIdxProvided;
    uint32_t ShowGfxclkIdx;
    bool ShowDcefclkProvided, ShowDcefclkIdxProvided;
    uint32_t ShowDcefclkIdx;
    bool ShowVddcLookupProvided, ShowVddcLookupIdxProvided;
    uint32_t ShowVddcLookupIdx;
    bool ShowVddmemLookupProvided, ShowVddmemLookupIdxProvided;
    uint32_t ShowVddmemLookupIdx;
    bool ShowMMProvided, ShowMMIdxProvided;
    uint32_t ShowMMIdx;
    bool ShowPCIEProvided, ShowPCIEIdxProvided;
    uint32_t ShowPCIEIdx;
    bool ShowPixclkProvided, ShowPixclkIdxProvided;
    uint32_t ShowPixclkIdx;
    bool ShowDispClkProvided, ShowDispClkIdxProvided;
    uint32_t ShowDispClkIdx;
    bool ShowPhyClkProvided, ShowPhyClkIdxProvided;
    uint32_t ShowPhyClkIdx;
    bool ShowAllProvided;
    bool ShowPPProvided;

    bool SetVddcLookupProvided;
    uint8_t SetVddcLookupIdx;
    uint32_t SetVddcLookupValue;

    bool SetClockProvided;
    char* SetClockName;
    uint8_t SetClockIdx;
    uint32_t SetClockClk;
    uint32_t SetClockVccIdx;

    bool SetMMClockProvided;
    uint8_t SetMMClockIdx;
    uint32_t SetMMClockDClk;
    uint32_t SetMMClockVClk;
    uint32_t SetMMClockEClk;
    uint32_t SetMMClockPSPClk;
    uint32_t SetMMClockVccIdx;

} ArgsObj;

bool ParseCmdLine(ArgsObj* Args, int argc, char** argv);
