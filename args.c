#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

bool ParseCmdLine(ArgsObj* Args, int argc, char** argv);
void PrintUsage(char* BinName)
{
    printf("\nVegatool %s\n", ATOOL_VERSION);
    printf("Author: Jean M. Cyr https://github.com/jean-m-cyr");
    printf("Usage: %s [-i GPUIdx | -f VBIOSFile] [Options]\n", BinName);
    printf("Options:\n");

    printf("\t--ShowAll\n");
    printf("\t--ShowPP\n");
    printf("\t--ShowHardLimit [idx]\n");
    printf("\t--ShowState [idx]\n");
    printf("\t--ShowSocclk [idx]\n");
    printf("\t--ShowMclk [idx]\n");
    printf("\t--ShowGfxclk [idx]\n");
    printf("\t--ShowDcefclk [idx]\n");
    printf("\t--ShowVddcLookup [idx]\n");
    printf("\t--ShowVddmemLookup [idx]\n");
    printf("\t--ShowMM [idx]\n");
    printf("\t--ShowPCIE [idx]\n");
    printf("\t--ShowPixclk [idx]\n");
    printf("\t--ShowDispClk [idx]\n");
    printf("\t--ShowPhyClk [idx]\n\n");

    printf("\t--SetVddcLookup idx value\n");
    printf("\t--SetClock gfx|m|soc|dcef|pix|disp|phy idx clk vccidx\n");
    printf("\t--SetMMClock idx dclk vclk eclk pspclk vccidx\n");
}

bool ParseCmdLine(ArgsObj* Args, int argc, char** argv)
{
    memset(Args, 0, sizeof(ArgsObj));

    if (argc < 3)
    {
        PrintUsage(argv[0]);
        return false;
    }

    for (int i = 1; i < argc; ++i)
    {
        if (!strcasecmp("-i", argv[i]))
        {
            if (i == (argc - 1))
            {
                printf("Argument \"%s\" requires a parameter.\n", argv[i]);
                return false;
            }
            Args->GPUIdx = strtoul(argv[++i], NULL, 10);

            if (errno == EINVAL || errno == ERANGE || Args->GPUIdx > 15)
            {
                printf("Invalid GPU index specified.\n");
                return false;
            }

            Args->GPUIdxProvided = true;
        }
        else if (!strcasecmp("-f", argv[i]))
        {
            if (i == (argc - 1))
            {
                printf("Argument \"%s\" requires a parameter.\n", argv[i]);
                return false;
            }
            Args->VBIOSFileName = strdup(argv[++i]);
            Args->VBIOSFileProvided = true;
        }
        else if (!strcasecmp("--ShowAll", argv[i]))
        {
            Args->ShowAllProvided = true;
        }
        else if (!strcasecmp("--ShowPP", argv[i]))
        {
            Args->ShowPPProvided = true;
        }
        else if (!strcasecmp("--ShowHardLimit", argv[i]))
        {
            Args->ShowHardLimitProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowHardLimitIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowHardLimitIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowState", argv[i]))
        {
            Args->ShowStateProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowStateIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowStateIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowSocclk", argv[i]))
        {
            Args->ShowSocclkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowSocclkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowSocclkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowMclk", argv[i]))
        {
            Args->ShowMclkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowMclkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowMclkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowGfxclk", argv[i]))
        {
            Args->ShowGfxclkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowGfxclkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowGfxclkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowDcefclk", argv[i]))
        {
            Args->ShowDcefclkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowDcefclkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowDcefclkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowVddcLookup", argv[i]))
        {
            Args->ShowVddcLookupProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowVddcLookupIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowVddcLookupIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowVddmemLookup", argv[i]))
        {
            Args->ShowVddmemLookupProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowVddmemLookupIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowVddmemLookupIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowMM", argv[i]))
        {
            Args->ShowMMProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowMMIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowMMIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowPCIE", argv[i]))
        {
            Args->ShowPCIEProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowPCIEIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowPCIEIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowPixclk", argv[i]))
        {
            Args->ShowPixclkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowPixclkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowPixclkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowDispClk", argv[i]))
        {
            Args->ShowDispClkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowDispClkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowDispClkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--ShowPhyClk", argv[i]))
        {
            Args->ShowPhyClkProvided = true;
            if ((i < (argc - 1)) && (*argv[i + 1] != '-'))
            {
                Args->ShowPhyClkIdx = strtoul(argv[++i], NULL, 10);
                if (errno == EINVAL || errno == ERANGE)
                {
                    printf("Invalid index specified.\n");
                    return false;
                }
                Args->ShowPhyClkIdxProvided = true;
            }
        }
        else if (!strcasecmp("--SetVddcLookup", argv[i]))
        {
            if ((i >= (argc - 2)) || (*argv[i + 1] == '-') || (*argv[i + 2] == '-'))
            {
                printf("Parameter requires 2 arguments\n");
                return false;
            }
            Args->SetVddcLookupIdx = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid index specified.\n");
                return false;
            }
            Args->SetVddcLookupValue = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid value specified.\n");
                return false;
            }
            Args->SetVddcLookupProvided = true;
        }
        else if (!strcasecmp("--SetClock", argv[i]))
        {
            if ((i >= (argc - 4)) || (*argv[i + 1] == '-') || (*argv[i + 2] == '-') ||
                (*argv[i + 3] == '-') || (*argv[i + 4] == '-'))
            {
                printf("Parameter requires 4 arguments\n");
                return false;
            }
            Args->SetClockName = strdup(argv[++i]);
            Args->SetClockIdx = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid index specified.\n");
                return false;
            }
            Args->SetClockClk = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid value specified.\n");
                return false;
            }
            Args->SetClockVccIdx = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid vdd index value specified.\n");
                return false;
            }
            Args->SetClockProvided = true;
        }
        else if (!strcasecmp("--SetMMClock", argv[i]))
        {
            if ((i >= (argc - 6)) || (*argv[i + 1] == '-') || (*argv[i + 2] == '-') ||
                (*argv[i + 3] == '-') || (*argv[i + 4] == '-') || (*argv[i + 5] == '-') ||
                (*argv[i + 6] == '-'))
            {
                printf("Parameter requires 6 arguments\n");
                return false;
            }
            Args->SetMMClockIdx = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid index specified.\n");
                return false;
            }
            Args->SetMMClockDClk = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid Dclk specified.\n");
                return false;
            }
            Args->SetMMClockVClk = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid VClk specified.\n");
                return false;
            }
            Args->SetMMClockEClk = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid EClk specified.\n");
                return false;
            }
            Args->SetMMClockPSPClk = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid PSPClk specified.\n");
                return false;
            }
            Args->SetMMClockVccIdx = strtoul(argv[++i], NULL, 10);
            if (errno == EINVAL || errno == ERANGE)
            {
                printf("Invalid vcc index specified.\n");
                return false;
            }
            Args->SetMMClockProvided = true;
        }
        else
        {
            PrintUsage(argv[0]);
            printf("\nUnknown option: \"%s\"\n", argv[i]);
            return false;
        }
    }

    return true;
}
