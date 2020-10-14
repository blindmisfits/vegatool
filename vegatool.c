#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "args.h"
#include "vbios-tables.h"

uint32_t GetPPTableOffset(uint8_t* VBIOS)
{
    ATOM_ROM_HEADER* hdr =
        (ATOM_ROM_HEADER*)(VBIOS + ((uint16_t*)(VBIOS + OFFSET_TO_POINTER_TO_ATOM_ROM_HEADER))[0]);
    ATOM_MASTER_LIST_OF_DATA_TABLES* DataTblList =
        &((ATOM_MASTER_DATA_TABLE*)(VBIOS + hdr->usMasterDataTableOffset))->ListOfDataTables;
    return DataTblList->PowerPlayInfo;
}

static char stringbuf[32];

static char* voltage(uint8_t* PPTblBuf, uint8_t idx)
{
    ATOM_Vega10_POWERPLAYTABLE* PPHdr = (ATOM_Vega10_POWERPLAYTABLE*)PPTblBuf;
    if (PPHdr->usVddcLookupTableOffset)
    {
        ATOM_Vega10_Voltage_Lookup_Table* t =
            (ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf + PPHdr->usVddcLookupTableOffset);
        if (idx < t->ucNumEntries)
        {
            ATOM_Vega10_Voltage_Lookup_Record* r = &t->entries[idx];
            sprintf(stringbuf, "%.3fV", r->usVdd / 1000.0);
        }
        else
            strcpy(stringbuf, "(Invalid index)");
    }
    else
        strcpy(stringbuf, "(VCC table missing)");
    return stringbuf;
}

int main(int argc, char** argv)
{
    struct stat FileStats;
    char FilePath[64];
    uint8_t* PPTblBuf;
    size_t BytesRead;
    ArgsObj Config;
    size_t PPTblSize;
    FILE* PPFile;

    if (!ParseCmdLine(&Config, argc, argv))
        return -1;

    if (!Config.GPUIdxProvided && !Config.VBIOSFileProvided)
    {
        printf("GPU index is required when editing a live PowerPlay table.\n");
        return -1;
    }

    if (Config.VBIOSFileProvided)
    {
        PPFile = fopen(Config.VBIOSFileName, "rb+");

        if (!PPFile)
        {
            printf("Error opening %s (are you root, and does it exist?)\n", Config.VBIOSFileName);
            free(Config.VBIOSFileName);
            fclose(PPFile);
            return -1;
        }

        stat(Config.VBIOSFileName, &FileStats);

        if (FileStats.st_size > 1048576)
        {
            printf("File too large to be a VBIOS.\n");
            free(Config.VBIOSFileName);
            fclose(PPFile);
            return -1;
        }

        free(Config.VBIOSFileName);
    }
    else
    {
        sprintf(FilePath, "/sys/class/drm/card%d/device/pp_table", Config.GPUIdx);

        PPFile = fopen(FilePath, "rb+");

        if (!PPFile)
        {
            printf("Error opening %s (are you root, and does it exist?)\n", FilePath);
            return -1;
        }
    }

    PPTblBuf = (uint8_t*)malloc(sizeof(ATOM_COMMON_TABLE_HEADER));

    BytesRead = fread(PPTblBuf, 1, sizeof(ATOM_COMMON_TABLE_HEADER), PPFile);

    if (BytesRead != sizeof(ATOM_COMMON_TABLE_HEADER))
    {
        printf("Unable to read header from file.\n");
        free(PPTblBuf);
        fclose(PPFile);
        return -1;
    }

    PPTblSize = ((ATOM_COMMON_TABLE_HEADER*)PPTblBuf)->usStructureSize;

    PPTblBuf = realloc(PPTblBuf, sizeof(ATOM_COMMON_TABLE_HEADER) + PPTblSize);

    if (errno == ENOMEM)
    {
        printf("Unable to allocate enough memory for the PowerPlay table.\n");
        free(PPTblBuf);
        fclose(PPFile);
        return -1;
    }

    BytesRead += fread(PPTblBuf + sizeof(ATOM_COMMON_TABLE_HEADER), 1, PPTblSize, PPFile);

    if (BytesRead != PPTblSize)
    {
        printf("Unable to read entire PowerPlay table. (read %u, size was %u)\n",
            (unsigned)BytesRead, (unsigned)PPTblSize);
        free(PPTblBuf);
        fclose(PPFile);
        return -1;
    }

    ATOM_Vega10_POWERPLAYTABLE* PPHdr = (ATOM_Vega10_POWERPLAYTABLE*)PPTblBuf;

    if (PPHdr->sHeader.ucTableFormatRevision < 8)
    {
        printf("Tool is for Vega only\n");
        return -1;
    }

    if (Config.ShowPPProvided || Config.ShowAllProvided)
    {
        printf("Powerplay table\n");
        printf("    Header.StructureSize :         %u\n", PPHdr->sHeader.usStructureSize);
        printf("    Header.TableFormatRevision :   %u\n", PPHdr->sHeader.ucTableFormatRevision);
        printf("    Header.TableContentRevision :  %u\n", PPHdr->sHeader.ucTableContentRevision);
        printf("    TableRevision :                %u\n", PPHdr->ucTableRevision);
        printf("    TableSize :                    %u\n", PPHdr->usTableSize);
        printf("    GoldenPPID :                   %u\n", PPHdr->ulGoldenPPID);
        printf("    GoldenRevision :               %u\n", PPHdr->ulGoldenRevision);
        printf("    FormatID :                     %u\n", PPHdr->usFormatID);
        printf("    PlatformCaps :                 %u\n", PPHdr->ulPlatformCaps);
        printf("    MaxODEngineClock :             %u\n", PPHdr->ulMaxODEngineClock);
        printf("    MaxODMemoryClock :             %u\n", PPHdr->ulMaxODMemoryClock);
        printf("    PowerControlLimit :            %u\n", PPHdr->usPowerControlLimit);
        printf("    UlvVoltageOffset :             %u\n", PPHdr->usUlvVoltageOffset);
        printf("    UlvSmnclkDid :                 %u\n", PPHdr->usUlvSmnclkDid);
        printf("    UlvMp1clkDid :                 %u\n", PPHdr->usUlvMp1clkDid);
        printf("    UlvGfxclkBypass :              %u\n", PPHdr->usUlvGfxclkBypass);
        printf("    GfxclkSlewRate :               %u\n", PPHdr->usGfxclkSlewRate);
        printf("    GfxVoltageMode :               %u\n", PPHdr->ucGfxVoltageMode);
        printf("    SocVoltageMode :               %u\n", PPHdr->ucSocVoltageMode);
        printf("    UclkVoltageMode :              %u\n", PPHdr->ucUclkVoltageMode);
        printf("    UvdVoltageMode :               %u\n", PPHdr->ucUvdVoltageMode);
        printf("    VceVoltageMode :               %u\n", PPHdr->ucVceVoltageMode);
        printf("    Mp0VoltageMode :               %u\n", PPHdr->ucMp0VoltageMode);
        printf("    DcefVoltageMode :              %u\n", PPHdr->ucDcefVoltageMode);
        printf("    StateArrayOffset :             %u\n", PPHdr->usStateArrayOffset);
        printf("    FanTableOffset :               %u\n", PPHdr->usFanTableOffset);
        printf("    ThermalControllerOffset :      %u\n", PPHdr->usThermalControllerOffset);
        printf("    SocclkDependencyTableOffset :  %u\n", PPHdr->usSocclkDependencyTableOffset);
        printf("    MclkDependencyTableOffset :    %u\n", PPHdr->usMclkDependencyTableOffset);
        printf("    GfxclkDependencyTableOffset :  %u\n", PPHdr->usGfxclkDependencyTableOffset);
        printf("    DcefclkDependencyTableOffset : %u\n", PPHdr->usDcefclkDependencyTableOffset);
        printf("    VddcLookupTableOffset :        %u\n", PPHdr->usVddcLookupTableOffset);
        printf("    VddmemLookupTableOffset :      %u\n", PPHdr->usVddmemLookupTableOffset);
        printf("    MMDependencyTableOffset :      %u\n", PPHdr->usMMDependencyTableOffset);
        printf("    VCEStateTableOffset :          %u\n", PPHdr->usVCEStateTableOffset);
        printf("    PowerTuneTableOffset :         %u\n", PPHdr->usPowerTuneTableOffset);
        printf("    HardLimitTableOffset :         %u\n", PPHdr->usHardLimitTableOffset);
        printf("    VddciLookupTableOffset :       %u\n", PPHdr->usVddciLookupTableOffset);
        printf("    PCIETableOffset :              %u\n", PPHdr->usPCIETableOffset);
        printf("    PixclkDependencyTableOffset :  %u\n", PPHdr->usPixclkDependencyTableOffset);
        printf("    DispClkDependencyTableOffset : %u\n", PPHdr->usDispClkDependencyTableOffset);
        printf("    PhyClkDependencyTableOffset :  %u\n", PPHdr->usPhyClkDependencyTableOffset);
    }
    if (Config.ShowHardLimitProvided || Config.ShowAllProvided)
    {
        printf("HardLimit table\n");
        if (PPHdr->usHardLimitTableOffset)
        {
            if (Config.ShowHardLimitIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowHardLimitIdx);
                ATOM_Vega10_Hard_Limit_Record* r =
                    &((ATOM_Vega10_Hard_Limit_Table*)(PPTblBuf + PPHdr->usHardLimitTableOffset))
                         ->entries[Config.ShowHardLimitIdx];
                printf("    SOCCLKLimit : %u\n", r->ulSOCCLKLimit);
                printf("    GFXCLKLimit : %u\n", r->ulGFXCLKLimit);
                printf("    MCLKLimit :   %u\n", r->ulMCLKLimit);
                printf("    VddcLimit :   %u\n", r->usVddcLimit);
                printf("    VddciLimit :  %u\n", r->usVddciLimit);
                printf("    VddMemLimit : %u\n", r->usVddMemLimit);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_Hard_Limit_Table*)(PPTblBuf + PPHdr->usHardLimitTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_Hard_Limit_Record* r =
                        &((ATOM_Vega10_Hard_Limit_Table*)(PPTblBuf + PPHdr->usHardLimitTableOffset))
                             ->entries[idx];
                    printf("    SOCCLKLimit : %u\n", r->ulSOCCLKLimit);
                    printf("    GFXCLKLimit : %u\n", r->ulGFXCLKLimit);
                    printf("    MCLKLimit :   %u\n", r->ulMCLKLimit);
                    printf("    VddcLimit :   %u\n", r->usVddcLimit);
                    printf("    VddciLimit :  %u\n", r->usVddciLimit);
                    printf("    VddMemLimit : %u\n", r->usVddMemLimit);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowStateProvided || Config.ShowAllProvided)
    {
        printf("State table\n");
        if (PPHdr->usStateArrayOffset)
        {
            if (Config.ShowStateIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowStateIdx);
                ATOM_Vega10_State* r =
                    &((ATOM_Vega10_State_Array*)(PPTblBuf + PPHdr->usStateArrayOffset))
                         ->states[Config.ShowStateIdx];
                printf("    SocClockIndexHigh : %u\n", r->ucSocClockIndexHigh);
                printf("    SocClockIndexLow :  %u\n", r->ucSocClockIndexLow);
                printf("    GfxClockIndexHigh : %u\n", r->ucGfxClockIndexHigh);
                printf("    GfxClockIndexLow :  %u\n", r->ucGfxClockIndexLow);
                printf("    MemClockIndexHigh : %u\n", r->ucMemClockIndexHigh);
                printf("    MemClockIndexLow :  %u\n", r->ucMemClockIndexLow);
                printf("    Classification :    %u\n", r->usClassification);
                printf("    CapsAndSettings :   %u\n", r->ulCapsAndSettings);
                printf("    Classification2 :   %u\n", r->usClassification2);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx < ((ATOM_Vega10_State_Array*)(PPTblBuf + PPHdr->usStateArrayOffset))
                               ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_State* r =
                        &((ATOM_Vega10_State_Array*)(PPTblBuf + PPHdr->usStateArrayOffset))
                             ->states[idx];
                    printf("    SocClockIndexHigh : %u\n", r->ucSocClockIndexHigh);
                    printf("    SocClockIndexLow  : %u\n", r->ucSocClockIndexLow);
                    printf("    GfxClockIndexHigh : %u\n", r->ucGfxClockIndexHigh);
                    printf("    GfxClockIndexLow  : %u\n", r->ucGfxClockIndexLow);
                    printf("    MemClockIndexHigh : %u\n", r->ucMemClockIndexHigh);
                    printf("    MemClockIndexLow  : %u\n", r->ucMemClockIndexLow);
                    printf("    Classification    : %u\n", r->usClassification);
                    printf("    CapsAndSettings   : %u\n", r->ulCapsAndSettings);
                    printf("    Classification2   : %u\n", r->usClassification2);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowSocclkProvided || Config.ShowAllProvided)
    {
        printf("Socclk table\n");
        if (PPHdr->usSocclkDependencyTableOffset)
        {
            if (Config.ShowSocclkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowSocclkIdx);
                ATOM_Vega10_CLK_Dependency_Record* r =
                    &((ATOM_Vega10_SOCCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usSocclkDependencyTableOffset))
                         ->entries[Config.ShowSocclkIdx];
                printf("    Clk :    %u\n", r->ulClk);
                printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_SOCCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usSocclkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_CLK_Dependency_Record* r = &(
                        (ATOM_Vega10_SOCCLK_Dependency_Table*)(PPTblBuf +
                                                               PPHdr
                                                                   ->usSocclkDependencyTableOffset))
                                                                ->entries[idx];
                    printf("    Clk :    %u\n", r->ulClk);
                    printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowMclkProvided || Config.ShowAllProvided)
    {
        printf("Mclk table\n");
        if (PPHdr->usMclkDependencyTableOffset)
        {
            if (Config.ShowMclkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowMclkIdx);
                ATOM_Vega10_MCLK_Dependency_Record* r =
                    &((ATOM_Vega10_MCLK_Dependency_Table*)(PPTblBuf +
                                                           PPHdr->usMclkDependencyTableOffset))
                         ->entries[Config.ShowMclkIdx];
                printf("    MemClk :    %u\n", r->ulMemClk);
                printf("    VddInd :    %u\n", r->ucVddInd);
                printf(
                    "    VddMemInd : %u (%s)\n", r->ucVddMemInd, voltage(PPTblBuf, r->ucVddMemInd));
                printf(
                    "    VddciInd :  %u (%s)\n", r->ucVddciInd, voltage(PPTblBuf, r->ucVddciInd));
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_MCLK_Dependency_Table*)(PPTblBuf +
                                                           PPHdr->usMclkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_MCLK_Dependency_Record* r =
                        &((ATOM_Vega10_MCLK_Dependency_Table*)(PPTblBuf +
                                                               PPHdr->usMclkDependencyTableOffset))
                             ->entries[idx];
                    printf("    MemClk :    %u\n", r->ulMemClk);
                    printf("    VddInd :    %u\n", r->ucVddInd);
                    printf("    VddMemInd : %u (%s)\n", r->ucVddMemInd,
                        voltage(PPTblBuf, r->ucVddMemInd));
                    printf("    VddciInd :  %u (%s)\n", r->ucVddciInd,
                        voltage(PPTblBuf, r->ucVddciInd));
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowGfxclkProvided || Config.ShowAllProvided)
    {
        printf("Gfxclk table\n");
        if (PPHdr->usGfxclkDependencyTableOffset)
        {
            if (Config.ShowGfxclkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowGfxclkIdx);
                ATOM_Vega10_GFXCLK_Dependency_Record_V2* r = &(
                    (ATOM_Vega10_GFXCLK_Dependency_Table_V2*)(PPTblBuf +
                                                              PPHdr->usGfxclkDependencyTableOffset))
                                                                  ->entries[Config.ShowGfxclkIdx];

                printf("    Clk :                  %u\n", r->ulClk);
                printf("    VddInd :               %u (%s)\n", r->ucVddInd,
                    voltage(PPTblBuf, r->ucVddInd));
                printf("    CKSVOffsetandDisable : %u\n", r->usCKSVOffsetandDisable);
                printf("    AVFSOffset :           %u\n", r->usAVFSOffset);
                printf("    ACGEnable :            %u\n", r->ucACGEnable);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_GFXCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usGfxclkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_GFXCLK_Dependency_Record_V2* r = &(
                        (ATOM_Vega10_GFXCLK_Dependency_Table_V2*)(PPTblBuf +
                                                                  PPHdr
                                                                      ->usGfxclkDependencyTableOffset))
                                                                      ->entries[idx];
                    printf("    Clk :                  %u\n", r->ulClk);
                    printf("    VddInd :               %u (%s)\n", r->ucVddInd,
                        voltage(PPTblBuf, r->ucVddInd));
                    printf("    CKSVOffsetandDisable : %u\n", r->usCKSVOffsetandDisable);
                    printf("    AVFSOffset :           %u\n", r->usAVFSOffset);
                    printf("    ACGEnable :            %u\n", r->ucACGEnable);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowDcefclkProvided || Config.ShowAllProvided)
    {
        printf("Dcefclk table\n");
        if (PPHdr->usDcefclkDependencyTableOffset)
        {
            if (Config.ShowDcefclkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowDcefclkIdx);
                ATOM_Vega10_CLK_Dependency_Record* r = &(
                    (ATOM_Vega10_DCEFCLK_Dependency_Table*)(PPTblBuf +
                                                            PPHdr->usDcefclkDependencyTableOffset))
                                                            ->entries[Config.ShowDcefclkIdx];
                printf("    Clk :    %u\n", r->ulClk);
                printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_DCEFCLK_Dependency_Table*)(PPTblBuf +
                                                              PPHdr
                                                                  ->usDcefclkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_CLK_Dependency_Record* r = &(
                        (ATOM_Vega10_DCEFCLK_Dependency_Table*)(PPTblBuf +
                                                                PPHdr
                                                                    ->usDcefclkDependencyTableOffset))
                                                                ->entries[idx];
                    printf("    Clk :    %u\n", r->ulClk);
                    printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowVddcLookupProvided || Config.ShowAllProvided)
    {
        printf("VddcLookup table\n");
        if (PPHdr->usVddcLookupTableOffset)
        {
            if (Config.ShowVddcLookupIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowVddcLookupIdx);
                ATOM_Vega10_Voltage_Lookup_Record* r = &(
                    (ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf + PPHdr->usVddcLookupTableOffset))
                                                            ->entries[Config.ShowVddcLookupIdx];
                printf("    Vdd : %u (%.3fV)\n", r->usVdd, r->usVdd / 1000.0);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx < ((ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf +
                                                                PPHdr->usVddcLookupTableOffset))
                               ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_Voltage_Lookup_Record* r =
                        &((ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf +
                                                              PPHdr->usVddcLookupTableOffset))
                             ->entries[idx];
                    printf("    Vdd : %u (%.3fV)\n", r->usVdd, r->usVdd / 1000.0);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowVddmemLookupProvided || Config.ShowAllProvided)
    {
        printf("VddmemLookup table\n");
        if (PPHdr->usVddmemLookupTableOffset)
        {
            if (Config.ShowVddmemLookupIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowVddmemLookupIdx);
                ATOM_Vega10_Voltage_Lookup_Record* r = &(
                    (ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf + PPHdr->usVddmemLookupTableOffset))
                                                            ->entries[Config.ShowVddmemLookupIdx];
                printf("    Vdd : %u\n", r->usVdd);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx < ((ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf +
                                                                PPHdr->usVddmemLookupTableOffset))
                               ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_Voltage_Lookup_Record* r =
                        &((ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf +
                                                              PPHdr->usVddmemLookupTableOffset))
                             ->entries[idx];
                    printf("    Vdd : %u\n", r->usVdd);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowMMProvided || Config.ShowAllProvided)
    {
        printf("MM table\n");
        if (PPHdr->usMMDependencyTableOffset)
        {
            if (Config.ShowMMIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowMMIdx);
                ATOM_Vega10_MM_Dependency_Record* r = &(
                    (ATOM_Vega10_MM_Dependency_Table*)(PPTblBuf + PPHdr->usMMDependencyTableOffset))
                                                           ->entries[Config.ShowMMIdx];
                printf("    VddcInd : %u (%s)\n", r->ucVddcInd, voltage(PPTblBuf, r->ucVddcInd));
                printf("    DClk :    %u\n", r->ulDClk);
                printf("    VClk :    %u\n", r->ulVClk);
                printf("    EClk :    %u\n", r->ulEClk);
                printf("    PSPClk :  %u\n", r->ulPSPClk);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx < ((ATOM_Vega10_MM_Dependency_Table*)(PPTblBuf +
                                                               PPHdr->usMMDependencyTableOffset))
                               ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_MM_Dependency_Record* r =
                        &((ATOM_Vega10_MM_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usMMDependencyTableOffset))
                             ->entries[idx];
                    printf(
                        "    VddcInd : %u (%s)\n", r->ucVddcInd, voltage(PPTblBuf, r->ucVddcInd));
                    printf("    DClk :    %u\n", r->ulDClk);
                    printf("    VClk :    %u\n", r->ulVClk);
                    printf("    EClk :    %u\n", r->ulEClk);
                    printf("    PSPClk :  %u\n", r->ulPSPClk);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowPCIEProvided || Config.ShowAllProvided)
    {
        printf("PCIE table\n");
        if (PPHdr->usPCIETableOffset)
        {
            if (Config.ShowPCIEIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowPCIEIdx);
                ATOM_Vega10_PCIE_Record* r =
                    &((ATOM_Vega10_PCIE_Table*)(PPTblBuf + PPHdr->usPCIETableOffset))
                         ->entries[Config.ShowPCIEIdx];
                printf("    LCLK :          %u\n", r->ulLCLK);
                printf("    PCIEGenSpeed :  %u\n", r->ucPCIEGenSpeed);
                printf("    PCIELaneWidth : %u\n", r->ucPCIELaneWidth);
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_PCIE_Table*)(PPTblBuf + PPHdr->usPCIETableOffset))->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_PCIE_Record* r =
                        &((ATOM_Vega10_PCIE_Table*)(PPTblBuf + PPHdr->usPCIETableOffset))
                             ->entries[idx];
                    printf("    LCLK :          %u\n", r->ulLCLK);
                    printf("    PCIEGenSpeed :  %u\n", r->ucPCIEGenSpeed);
                    printf("    PCIELaneWidth : %u\n", r->ucPCIELaneWidth);
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowPixclkProvided || Config.ShowAllProvided)
    {
        printf("Pixclk table\n");
        if (PPHdr->usPixclkDependencyTableOffset)
        {
            if (Config.ShowPixclkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowPixclkIdx);
                ATOM_Vega10_CLK_Dependency_Record* r =
                    &((ATOM_Vega10_PIXCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usPixclkDependencyTableOffset))
                         ->entries[Config.ShowPixclkIdx];
                printf("    Clk :    %u\n", r->ulClk);
                printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_PIXCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usPixclkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_CLK_Dependency_Record* r = &(
                        (ATOM_Vega10_PIXCLK_Dependency_Table*)(PPTblBuf +
                                                               PPHdr
                                                                   ->usPixclkDependencyTableOffset))
                                                                ->entries[idx];
                    printf("    Clk :    %u\n", r->ulClk);
                    printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowDispClkProvided || Config.ShowAllProvided)
    {
        printf("DispClk table\n");
        if (PPHdr->usDispClkDependencyTableOffset)
        {
            if (Config.ShowDispClkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowDispClkIdx);
                ATOM_Vega10_CLK_Dependency_Record* r = &(
                    (ATOM_Vega10_DISPCLK_Dependency_Table*)(PPTblBuf +
                                                            PPHdr->usDispClkDependencyTableOffset))
                                                            ->entries[Config.ShowDispClkIdx];
                printf("    Clk :    %u\n", r->ulClk);
                printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_DISPCLK_Dependency_Table*)(PPTblBuf +
                                                              PPHdr
                                                                  ->usDispClkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_CLK_Dependency_Record* r = &(
                        (ATOM_Vega10_DISPCLK_Dependency_Table*)(PPTblBuf +
                                                                PPHdr
                                                                    ->usDispClkDependencyTableOffset))
                                                                ->entries[idx];
                    printf("    Clk :    %u\n", r->ulClk);
                    printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.ShowPhyClkProvided || Config.ShowAllProvided)
    {
        printf("PhyClk table\n");
        if (PPHdr->usPhyClkDependencyTableOffset)
        {
            if (Config.ShowPhyClkIdxProvided)
            {
                printf("  Entry %u\n", Config.ShowPhyClkIdx);
                ATOM_Vega10_CLK_Dependency_Record* r =
                    &((ATOM_Vega10_PHYCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usPhyClkDependencyTableOffset))
                         ->entries[Config.ShowPhyClkIdx];
                printf("    Clk :    %u\n", r->ulClk);
                printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
            }
            else
            {
                for (uint32_t idx = 0;
                     idx <
                     ((ATOM_Vega10_PHYCLK_Dependency_Table*)(PPTblBuf +
                                                             PPHdr->usPhyClkDependencyTableOffset))
                         ->ucNumEntries;
                     idx++)
                {
                    printf("  Entry %u\n", idx);
                    ATOM_Vega10_CLK_Dependency_Record* r = &(
                        (ATOM_Vega10_PHYCLK_Dependency_Table*)(PPTblBuf +
                                                               PPHdr
                                                                   ->usPhyClkDependencyTableOffset))
                                                                ->entries[idx];
                    printf("    Clk :    %u\n", r->ulClk);
                    printf("    VddInd : %u (%s)\n", r->ucVddInd, voltage(PPTblBuf, r->ucVddInd));
                }
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.SetVddcLookupProvided)
    {
        if (Config.SetVddcLookupValue < 800 || Config.SetVddcLookupValue > 1200)
        {
            printf("Value out of range: 800 - 1200\n");
            return 1;
        }
        if (PPHdr->usVddcLookupTableOffset)
        {
            ATOM_Vega10_Voltage_Lookup_Table* t =
                (ATOM_Vega10_Voltage_Lookup_Table*)(PPTblBuf + PPHdr->usVddcLookupTableOffset);
            if (Config.SetVddcLookupIdx >= t->ucNumEntries)
            {
                printf("Index out of range\n");
            }
            else
            {
                ATOM_Vega10_Voltage_Lookup_Record* r = &t->entries[Config.SetVddcLookupIdx];
                printf("Index %u old = %u, new = %u\n", Config.SetVddcLookupIdx, r->usVdd,
                    Config.SetVddcLookupValue);
                r->usVdd = Config.SetVddcLookupValue;
            }
        }
        else
            printf("  Table missing.\n");
    }

    if (Config.SetClockProvided)
    {
        if (!strcasecmp(Config.SetClockName, "gfx"))
        {
            if (PPHdr->usGfxclkDependencyTableOffset)
            {
                ATOM_Vega10_GFXCLK_Dependency_Table_V2* t =
                    (ATOM_Vega10_GFXCLK_Dependency_Table_V2*)(PPTblBuf +
                                                              PPHdr->usGfxclkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_GFXCLK_Dependency_Record_V2* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
        else if (!strcasecmp(Config.SetClockName, "m"))
        {
            if (PPHdr->usMclkDependencyTableOffset)
            {
                ATOM_Vega10_MCLK_Dependency_Table* t =
                    (ATOM_Vega10_MCLK_Dependency_Table*)(PPTblBuf +
                                                         PPHdr->usMclkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_MCLK_Dependency_Record* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulMemClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulMemClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
        else if (!strcasecmp(Config.SetClockName, "soc"))
        {
            if (PPHdr->usSocclkDependencyTableOffset)
            {
                ATOM_Vega10_SOCCLK_Dependency_Table* t =
                    (ATOM_Vega10_SOCCLK_Dependency_Table*)(PPTblBuf +
                                                           PPHdr->usSocclkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_CLK_Dependency_Record* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
        else if (!strcasecmp(Config.SetClockName, "dcef"))
        {
            if (PPHdr->usDcefclkDependencyTableOffset)
            {
                ATOM_Vega10_DCEFCLK_Dependency_Table* t =
                    (ATOM_Vega10_DCEFCLK_Dependency_Table*)(PPTblBuf +
                                                            PPHdr->usDcefclkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_CLK_Dependency_Record* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
        else if (!strcasecmp(Config.SetClockName, "pix"))
        {
            if (PPHdr->usPixclkDependencyTableOffset)
            {
                ATOM_Vega10_PIXCLK_Dependency_Table* t =
                    (ATOM_Vega10_PIXCLK_Dependency_Table*)(PPTblBuf +
                                                           PPHdr->usPixclkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_CLK_Dependency_Record* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
        else if (!strcasecmp(Config.SetClockName, "disp"))
        {
            if (PPHdr->usDispClkDependencyTableOffset)
            {
                ATOM_Vega10_DISPCLK_Dependency_Table* t =
                    (ATOM_Vega10_DISPCLK_Dependency_Table*)(PPTblBuf +
                                                            PPHdr->usDispClkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_CLK_Dependency_Record* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
        else if (!strcasecmp(Config.SetClockName, "phy"))
        {
            if (PPHdr->usPhyClkDependencyTableOffset)
            {
                ATOM_Vega10_PHYCLK_Dependency_Table* t =
                    (ATOM_Vega10_PHYCLK_Dependency_Table*)(PPTblBuf +
                                                           PPHdr->usPhyClkDependencyTableOffset);
                if (Config.SetClockIdx >= t->ucNumEntries)
                {
                    printf("Index out of range\n");
                }
                else
                {
                    ATOM_Vega10_CLK_Dependency_Record* r = &t->entries[Config.SetClockIdx];
                    printf("Index %u old = %u %u, new = %u %u\n", Config.SetClockIdx, r->ulClk,
                        r->ucVddInd, Config.SetClockClk * 100, Config.SetClockVccIdx);
                    r->ulClk = Config.SetClockClk * 100;
                    r->ucVddInd = Config.SetClockVccIdx;
                }
            }
            else
                printf("  Table missing.\n");
        }
    }
    if (Config.SetMMClockProvided)
    {
        if (PPHdr->usMMDependencyTableOffset)
        {
            ATOM_Vega10_MM_Dependency_Table* t =
                (ATOM_Vega10_MM_Dependency_Table*)(PPTblBuf + PPHdr->usMMDependencyTableOffset);
            if (Config.SetMMClockIdx >= t->ucNumEntries)
            {
                printf("Index out of range\n");
            }
            else
            {
                ATOM_Vega10_MM_Dependency_Record* r = &t->entries[Config.SetMMClockIdx];
                printf("Index %u old = %u %u %u %u %u, new = %u %u %u %u %u\n",
                    Config.SetMMClockIdx, r->ulDClk, r->ulVClk, r->ulEClk, r->ulPSPClk,
                    r->ucVddcInd, Config.SetMMClockDClk * 100, Config.SetMMClockVClk * 100,
                    Config.SetMMClockEClk * 100, Config.SetMMClockPSPClk * 100, Config.SetMMClockVccIdx);
                r->ucVddcInd = Config.SetMMClockVccIdx;
                r->ulDClk = Config.SetMMClockDClk * 100;
                r->ulVClk = Config.SetMMClockVClk * 100;
                r->ulEClk = Config.SetMMClockEClk * 100;
                r->ulPSPClk = Config.SetMMClockPSPClk * 100;
            }
        }
        else
            printf("  Table missing.\n");
    }
    if (Config.SetClockProvided || Config.SetVddcLookupProvided || Config.SetMMClockProvided)
    {
        printf("Updating vega\n");
        if (Config.VBIOSFileProvided)
            fseek(PPFile, 0, SEEK_SET);

        fwrite(PPTblBuf, sizeof(uint8_t), BytesRead, PPFile);
    }
    fclose(PPFile);
    free(PPTblBuf);

    return 0;
}
