#pragma once

#include <stdint.h>

#pragma pack(push, 1)

#define OFFSET_TO_POINTER_TO_ATOM_ROM_HEADER		0x00000048L

typedef struct _ATOM_COMMON_TABLE_HEADER
{
	uint16_t usStructureSize;
	uint8_t  ucTableFormatRevision;   /*Change it when the Parser is not backward compatible */
	uint8_t  ucTableContentRevision;  /*Change it only when the table needs to change but the firmware */
							  /*Image can't be updated, while Driver needs to carry the new table! */
}ATOM_COMMON_TABLE_HEADER;

/****************************************************************************/	
// Structure stores the ROM header.
/****************************************************************************/	
typedef struct _ATOM_ROM_HEADER
{
	ATOM_COMMON_TABLE_HEADER		sHeader;
	uint8_t	 uaFirmWareSignature[4];    /*Signature to distinguish between Atombios and non-atombios, 
								  atombios should init it as "ATOM", don't change the position */
	uint16_t usBiosRuntimeSegmentAddress;
	uint16_t usProtectedModeInfoOffset;
	uint16_t usConfigFilenameOffset;
	uint16_t usCRC_BlockOffset;
	uint16_t usBIOS_BootupMessageOffset;
	uint16_t usInt10Offset;
	uint16_t usPciBusDevInitCode;
	uint16_t usIoBaseAddress;
	uint16_t usSubsystemVendorID;
	uint16_t usSubsystemID;
	uint16_t usPCI_InfoOffset; 
	uint16_t usMasterCommandTableOffset; /*Offset for SW to get all command table offsets, Don't change the position */
	uint16_t usMasterDataTableOffset;   /*Offset for SW to get all data table offsets, Don't change the position */
	uint8_t  ucExtendedFunctionCode;
	uint8_t  ucReserved;
}ATOM_ROM_HEADER;

typedef struct _ATOM_MASTER_LIST_OF_COMMAND_TABLES
{
	uint16_t ASIC_Init;                              //Function Table, used by various SW components,latest version 1.1
	uint16_t GetDisplaySurfaceSize;                  //Atomic Table,  Used by Bios when enabling HW ICON
	uint16_t ASIC_RegistersInit;                     //Atomic Table,  indirectly used by various SW components,called from ASIC_Init
	uint16_t VRAM_BlockVenderDetection;              //Atomic Table,  used only by Bios
	uint16_t DIGxEncoderControl;										 //Only used by Bios
	uint16_t MemoryControllerInit;                   //Atomic Table,  indirectly used by various SW components,called from ASIC_Init
	uint16_t EnableCRTCMemReq;                       //Function Table,directly used by various SW components,latest version 2.1
	uint16_t MemoryParamAdjust; 										 //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock if needed
	uint16_t DVOEncoderControl;                      //Function Table,directly used by various SW components,latest version 1.2
	uint16_t GPIOPinControl;												 //Atomic Table,  only used by Bios
	uint16_t SetEngineClock;                         //Function Table,directly used by various SW components,latest version 1.1
	uint16_t SetMemoryClock;                         //Function Table,directly used by various SW components,latest version 1.1
	uint16_t SetPixelClock;                          //Function Table,directly used by various SW components,latest version 1.2  
	uint16_t DynamicClockGating;                     //Atomic Table,  indirectly used by various SW components,called from ASIC_Init
	uint16_t ResetMemoryDLL;                         //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock
	uint16_t ResetMemoryDevice;                      //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock
	uint16_t MemoryPLLInit;
	uint16_t AdjustDisplayPll;												//only used by Bios
	uint16_t AdjustMemoryController;                 //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock                
	uint16_t EnableASIC_StaticPwrMgt;                //Atomic Table,  only used by Bios
	uint16_t ASIC_StaticPwrMgtStatusChange;          //Obsolete ,     only used by Bios   
	uint16_t DAC_LoadDetection;                      //Atomic Table,  directly used by various SW components,latest version 1.2  
	uint16_t LVTMAEncoderControl;                    //Atomic Table,directly used by various SW components,latest version 1.3
	uint16_t LCD1OutputControl;                      //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t DAC1EncoderControl;                     //Atomic Table,  directly used by various SW components,latest version 1.1  
	uint16_t DAC2EncoderControl;                     //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t DVOOutputControl;                       //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t CV1OutputControl;                       //Atomic Table,  Atomic Table,  Obsolete from Ry6xx, use DAC2 Output instead 
	uint16_t GetConditionalGoldenSetting;            //only used by Bios
	uint16_t TVEncoderControl;                       //Function Table,directly used by various SW components,latest version 1.1
	uint16_t TMDSAEncoderControl;                    //Atomic Table,  directly used by various SW components,latest version 1.3
	uint16_t LVDSEncoderControl;                     //Atomic Table,  directly used by various SW components,latest version 1.3
	uint16_t TV1OutputControl;                       //Atomic Table,  Obsolete from Ry6xx, use DAC2 Output instead
	uint16_t EnableScaler;                           //Atomic Table,  used only by Bios
	uint16_t BlankCRTC;                              //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t EnableCRTC;                             //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t GetPixelClock;                          //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t EnableVGA_Render;                       //Function Table,directly used by various SW components,latest version 1.1
	uint16_t GetSCLKOverMCLKRatio;                   //Atomic Table,  only used by Bios
	uint16_t SetCRTC_Timing;                         //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t SetCRTC_OverScan;                       //Atomic Table,  used by various SW components,latest version 1.1 
	uint16_t SetCRTC_Replication;                    //Atomic Table,  used only by Bios
	uint16_t SelectCRTC_Source;                      //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t EnableGraphSurfaces;                    //Atomic Table,  used only by Bios
	uint16_t UpdateCRTC_DoubleBufferRegisters;
	uint16_t LUT_AutoFill;                           //Atomic Table,  only used by Bios
	uint16_t EnableHW_IconCursor;                    //Atomic Table,  only used by Bios
	uint16_t GetMemoryClock;                         //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t GetEngineClock;                         //Atomic Table,  directly used by various SW components,latest version 1.1 
	uint16_t SetCRTC_UsingDTDTiming;                 //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t ExternalEncoderControl;                 //Atomic Table,  directly used by various SW components,latest version 2.1
	uint16_t LVTMAOutputControl;                     //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t VRAM_BlockDetectionByStrap;             //Atomic Table,  used only by Bios
	uint16_t MemoryCleanUp;                          //Atomic Table,  only used by Bios    
	uint16_t ProcessI2cChannelTransaction;           //Function Table,only used by Bios
	uint16_t WriteOneByteToHWAssistedI2C;            //Function Table,indirectly used by various SW components 
	uint16_t ReadHWAssistedI2CStatus;                //Atomic Table,  indirectly used by various SW components
	uint16_t SpeedFanControl;                        //Function Table,indirectly used by various SW components,called from ASIC_Init
	uint16_t PowerConnectorDetection;                //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t MC_Synchronization;                     //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock
	uint16_t ComputeMemoryEnginePLL;                 //Atomic Table,  indirectly used by various SW components,called from SetMemory/EngineClock
	uint16_t MemoryRefreshConversion;                //Atomic Table,  indirectly used by various SW components,called from SetMemory or SetEngineClock
	uint16_t VRAM_GetCurrentInfoBlock;               //Atomic Table,  used only by Bios
	uint16_t DynamicMemorySettings;                  //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock
	uint16_t MemoryTraining;                         //Atomic Table,  used only by Bios
	uint16_t EnableSpreadSpectrumOnPPLL;             //Atomic Table,  directly used by various SW components,latest version 1.2
	uint16_t TMDSAOutputControl;                     //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t SetVoltage;                             //Function Table,directly and/or indirectly used by various SW components,latest version 1.1
	uint16_t DAC1OutputControl;                      //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t DAC2OutputControl;                      //Atomic Table,  directly used by various SW components,latest version 1.1
	uint16_t SetupHWAssistedI2CStatus;               //Function Table,only used by Bios, obsolete soon.Switch to use "ReadEDIDFromHWAssistedI2C"
	uint16_t ClockSource;                            //Atomic Table,  indirectly used by various SW components,called from ASIC_Init
	uint16_t MemoryDeviceInit;                       //Atomic Table,  indirectly used by various SW components,called from SetMemoryClock
	uint16_t EnableYUV;                              //Atomic Table,  indirectly used by various SW components,called from EnableVGARender
	uint16_t DIG1EncoderControl;                     //Atomic Table,directly used by various SW components,latest version 1.1
	uint16_t DIG2EncoderControl;                     //Atomic Table,directly used by various SW components,latest version 1.1
	uint16_t DIG1TransmitterControl;                 //Atomic Table,directly used by various SW components,latest version 1.1
	uint16_t DIG2TransmitterControl;	               //Atomic Table,directly used by various SW components,latest version 1.1 
	uint16_t ProcessAuxChannelTransaction;					 //Function Table,only used by Bios
	uint16_t DPEncoderService;											 //Function Table,only used by Bios
	uint16_t GetVoltageInfo;
}ATOM_MASTER_LIST_OF_COMMAND_TABLES;   

typedef struct _ATOM_MASTER_LIST_OF_DATA_TABLES
{
	uint16_t        UtilityPipeLine;	        // Offest for the utility to get parser info,Don't change this position!
	uint16_t        MultimediaCapabilityInfo; // Only used by MM Lib,latest version 1.1, not configuable from Bios, need to include the table to build Bios 
	uint16_t        MultimediaConfigInfo;     // Only used by MM Lib,latest version 2.1, not configuable from Bios, need to include the table to build Bios
	uint16_t        StandardVESA_Timing;      // Only used by Bios
	uint16_t        FirmwareInfo;             // Shared by various SW components,latest version 1.4
	uint16_t        DAC_Info;                 // Will be obsolete from R600
	uint16_t        LCD_Info;                 // Shared by various SW components,latest version 1.3, was called LVDS_Info 
	uint16_t        TMDS_Info;                // Will be obsolete from R600
	uint16_t        AnalogTV_Info;            // Shared by various SW components,latest version 1.1 
	uint16_t        SupportedDevicesInfo;     // Will be obsolete from R600
	uint16_t        GPIO_I2C_Info;            // Shared by various SW components,latest version 1.2 will be used from R600           
	uint16_t        VRAM_UsageByFirmware;     // Shared by various SW components,latest version 1.3 will be used from R600
	uint16_t        GPIO_Pin_LUT;             // Shared by various SW components,latest version 1.1
	uint16_t        VESA_ToInternalModeLUT;   // Only used by Bios
	uint16_t        ComponentVideoInfo;       // Shared by various SW components,latest version 2.1 will be used from R600
	uint16_t        PowerPlayInfo;            // Shared by various SW components,latest version 2.1,new design from R600
	uint16_t        CompassionateData;        // Will be obsolete from R600
	uint16_t        SaveRestoreInfo;          // Only used by Bios
	uint16_t        PPLL_SS_Info;             // Shared by various SW components,latest version 1.2, used to call SS_Info, change to new name because of int ASIC SS info
	uint16_t        OemInfo;                  // Defined and used by external SW, should be obsolete soon
	uint16_t        XTMDS_Info;               // Will be obsolete from R600
	uint16_t        MclkSS_Info;              // Shared by various SW components,latest version 1.1, only enabled when ext SS chip is used
	uint16_t        Object_Header;            // Shared by various SW components,latest version 1.1
	uint16_t        IndirectIOAccess;         // Only used by Bios,this table position can't change at all!!
	uint16_t        MC_InitParameter;         // Only used by command table
	uint16_t        ASIC_VDDC_Info;						// Will be obsolete from R600
	uint16_t        ASIC_InternalSS_Info;			// New tabel name from R600, used to be called "ASIC_MVDDC_Info"
	uint16_t        TV_VideoMode;							// Only used by command table
	uint16_t        VRAM_Info;								// Only used by command table, latest version 1.3
	uint16_t        MemoryTrainingInfo;				// Used for VBIOS and Diag utility for memory training purpose since R600. the new table rev start from 2.1
	uint16_t        IntegratedSystemInfo;			// Shared by various SW components
	uint16_t        ASIC_ProfilingInfo;				// New table name from R600, used to be called "ASIC_VDDCI_Info" for pre-R600
	uint16_t        VoltageObjectInfo;				// Shared by various SW components, latest version 1.1
	uint16_t	PowerSourceInfo;					// Shared by various SW components, latest versoin 1.1
	uint16_t	ServiceInfo;
}ATOM_MASTER_LIST_OF_DATA_TABLES;

typedef struct _ATOM_MASTER_COMMAND_TABLE
{
	ATOM_COMMON_TABLE_HEADER           sHeader;
	ATOM_MASTER_LIST_OF_COMMAND_TABLES ListOfCommandTables;
}ATOM_MASTER_COMMAND_TABLE;

typedef struct _ATOM_MASTER_DATA_TABLE
{ 
	ATOM_COMMON_TABLE_HEADER sHeader;  
	ATOM_MASTER_LIST_OF_DATA_TABLES   ListOfDataTables;
}ATOM_MASTER_DATA_TABLE;

typedef struct _ATOM_Vega10_POWERPLAYTABLE
{
    ATOM_COMMON_TABLE_HEADER sHeader;

    uint8_t ucTableRevision;
    uint16_t usTableSize;        /* the size of header structure */
    uint32_t ulGoldenPPID;       /* PPGen use only */
    uint32_t ulGoldenRevision;   /* PPGen use only */
    uint16_t usFormatID;         /* PPGen use only */
    uint32_t ulPlatformCaps;     /* See ATOM_Vega10_CAPS_* */
    uint32_t ulMaxODEngineClock; /* For Overdrive. */
    uint32_t ulMaxODMemoryClock; /* For Overdrive. */
    uint16_t usPowerControlLimit;
    uint16_t usUlvVoltageOffset; /* in mv units */
    uint16_t usUlvSmnclkDid;
    uint16_t usUlvMp1clkDid;
    uint16_t usUlvGfxclkBypass;
    uint16_t usGfxclkSlewRate;
    uint8_t ucGfxVoltageMode;
    uint8_t ucSocVoltageMode;
    uint8_t ucUclkVoltageMode;
    uint8_t ucUvdVoltageMode;
    uint8_t ucVceVoltageMode;
    uint8_t ucMp0VoltageMode;
    uint8_t ucDcefVoltageMode;
    uint16_t usStateArrayOffset;             /* points to ATOM_Vega10_State_Array */
    uint16_t usFanTableOffset;               /* points to ATOM_Vega10_Fan_Table */
    uint16_t usThermalControllerOffset;      /* points to ATOM_Vega10_Thermal_Controller */
    uint16_t usSocclkDependencyTableOffset;  /* points to ATOM_Vega10_SOCCLK_Dependency_Table */
    uint16_t usMclkDependencyTableOffset;    /* points to ATOM_Vega10_MCLK_Dependency_Table */
    uint16_t usGfxclkDependencyTableOffset;  /* points to ATOM_Vega10_GFXCLK_Dependency_Table */
    uint16_t usDcefclkDependencyTableOffset; /* points to ATOM_Vega10_DCEFCLK_Dependency_Table */
    uint16_t usVddcLookupTableOffset;        /* points to ATOM_Vega10_Voltage_Lookup_Table */
    uint16_t usVddmemLookupTableOffset;      /* points to ATOM_Vega10_Voltage_Lookup_Table */
    uint16_t usMMDependencyTableOffset;      /* points to ATOM_Vega10_MM_Dependency_Table */
    uint16_t usVCEStateTableOffset;          /* points to ATOM_Vega10_VCE_State_Table */
    uint16_t usReserve;                      /* No PPM Support for Vega10 */
    uint16_t usPowerTuneTableOffset;         /* points to ATOM_Vega10_PowerTune_Table */
    uint16_t usHardLimitTableOffset;         /* points to ATOM_Vega10_Hard_Limit_Table */
    uint16_t usVddciLookupTableOffset;       /* points to ATOM_Vega10_Voltage_Lookup_Table */
    uint16_t usPCIETableOffset;              /* points to ATOM_Vega10_PCIE_Table */
    uint16_t usPixclkDependencyTableOffset;  /* points to ATOM_Vega10_PIXCLK_Dependency_Table */
    uint16_t usDispClkDependencyTableOffset; /* points to ATOM_Vega10_DISPCLK_Dependency_Table */
    uint16_t usPhyClkDependencyTableOffset;  /* points to ATOM_Vega10_PHYCLK_Dependency_Table */
} ATOM_Vega10_POWERPLAYTABLE;

typedef struct _ATOM_Vega10_State
{
    uint8_t ucSocClockIndexHigh;
    uint8_t ucSocClockIndexLow;
    uint8_t ucGfxClockIndexHigh;
    uint8_t ucGfxClockIndexLow;
    uint8_t ucMemClockIndexHigh;
    uint8_t ucMemClockIndexLow;
    uint16_t usClassification;
    uint32_t ulCapsAndSettings;
    uint16_t usClassification2;
} ATOM_Vega10_State;

typedef struct _ATOM_Vega10_State_Array
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;        /* Number of entries. */
    ATOM_Vega10_State states[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_State_Array;

typedef struct _ATOM_Vega10_CLK_Dependency_Record
{
    uint32_t ulClk;   /* Frequency of Clock */
    uint8_t ucVddInd; /* Base voltage */
} ATOM_Vega10_CLK_Dependency_Record;

typedef struct _ATOM_Vega10_GFXCLK_Dependency_Record
{
    uint32_t ulClk;                  /* Clock Frequency */
    uint8_t ucVddInd;                /* SOC_VDD index */
    uint16_t usCKSVOffsetandDisable; /* Bits 0~30: Voltage offset for CKS, Bit 31: Disable/enable
                                        for the GFXCLK level. */
    uint16_t usAVFSOffset;           /* AVFS Voltage offset */
} ATOM_Vega10_GFXCLK_Dependency_Record;

typedef struct _ATOM_Vega10_GFXCLK_Dependency_Record_V2
{
    uint32_t ulClk;
    uint8_t ucVddInd;
    uint16_t usCKSVOffsetandDisable;
    uint16_t usAVFSOffset;
    uint8_t ucACGEnable;
    uint8_t ucReserved[3];
} ATOM_Vega10_GFXCLK_Dependency_Record_V2;

typedef struct _ATOM_Vega10_MCLK_Dependency_Record
{
    uint32_t ulMemClk;   /* Clock Frequency */
    uint8_t ucVddInd;    /* SOC_VDD index */
    uint8_t ucVddMemInd; /* MEM_VDD - only non zero for MCLK record */
    uint8_t ucVddciInd;  /* VDDCI   = only non zero for MCLK record */
} ATOM_Vega10_MCLK_Dependency_Record;

typedef struct _ATOM_Vega10_GFXCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                            /* Number of entries. */
    ATOM_Vega10_GFXCLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_GFXCLK_Dependency_Table;

typedef struct _ATOM_Vega10_GFXCLK_Dependency_Table_V2
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                               /* Number of entries. */
    ATOM_Vega10_GFXCLK_Dependency_Record_V2 entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_GFXCLK_Dependency_Table_V2;

typedef struct _ATOM_Vega10_MCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                          /* Number of entries. */
    ATOM_Vega10_MCLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_MCLK_Dependency_Table;

typedef struct _ATOM_Vega10_SOCCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                         /* Number of entries. */
    ATOM_Vega10_CLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_SOCCLK_Dependency_Table;

typedef struct _ATOM_Vega10_DCEFCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                         /* Number of entries. */
    ATOM_Vega10_CLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_DCEFCLK_Dependency_Table;

typedef struct _ATOM_Vega10_PIXCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                         /* Number of entries. */
    ATOM_Vega10_CLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_PIXCLK_Dependency_Table;

typedef struct _ATOM_Vega10_DISPCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                         /* Number of entries.*/
    ATOM_Vega10_CLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_DISPCLK_Dependency_Table;

typedef struct _ATOM_Vega10_PHYCLK_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                         /* Number of entries. */
    ATOM_Vega10_CLK_Dependency_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_PHYCLK_Dependency_Table;

typedef struct _ATOM_Vega10_MM_Dependency_Record
{
    uint8_t ucVddcInd; /* SOC_VDD voltage */
    uint32_t ulDClk;   /* UVD D-clock */
    uint32_t ulVClk;   /* UVD V-clock */
    uint32_t ulEClk;   /* VCE clock */
    uint32_t ulPSPClk; /* PSP clock */
} ATOM_Vega10_MM_Dependency_Record;

typedef struct _ATOM_Vega10_MM_Dependency_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                        /* Number of entries */
    ATOM_Vega10_MM_Dependency_Record entries[1]; /* Dynamically allocate entries */
} ATOM_Vega10_MM_Dependency_Table;

typedef struct _ATOM_Vega10_PCIE_Record
{
    uint32_t ulLCLK;         /* LClock */
    uint8_t ucPCIEGenSpeed;  /* PCIE Speed */
    uint8_t ucPCIELaneWidth; /* PCIE Lane Width */
} ATOM_Vega10_PCIE_Record;

typedef struct _ATOM_Vega10_PCIE_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;               /* Number of entries */
    ATOM_Vega10_PCIE_Record entries[1]; /* Dynamically allocate entries. */
} ATOM_Vega10_PCIE_Table;

typedef struct _ATOM_Vega10_Voltage_Lookup_Record
{
    uint16_t usVdd; /* Base voltage */
} ATOM_Vega10_Voltage_Lookup_Record;

typedef struct _ATOM_Vega10_Voltage_Lookup_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;                         /* Number of entries */
    ATOM_Vega10_Voltage_Lookup_Record entries[1]; /* Dynamically allocate entries */
} ATOM_Vega10_Voltage_Lookup_Table;

typedef struct _ATOM_Vega10_Fan_Table
{
    uint8_t ucRevId; /* Change this if the table format changes or version changes so that the other
                        fields are not the same. */
    uint16_t usFanOutputSensitivity; /* Sensitivity of fan reaction to temepature changes. */
    uint16_t usFanRPMMax;            /* The default value in RPM. */
    uint16_t usThrottlingRPM;
    uint16_t usFanAcousticLimit;  /* Minimum Fan Controller Frequency Acoustic Limit. */
    uint16_t usTargetTemperature; /* The default ideal temperature in Celcius. */
    uint16_t usMinimumPWMLimit;   /* The minimum PWM that the advanced fan controller can set. */
    uint16_t usTargetGfxClk;      /* The ideal Fan Controller GFXCLK Frequency Acoustic Limit. */
    uint16_t usFanGainEdge;
    uint16_t usFanGainHotspot;
    uint16_t usFanGainLiquid;
    uint16_t usFanGainVrVddc;
    uint16_t usFanGainVrMvdd;
    uint16_t usFanGainPlx;
    uint16_t usFanGainHbm;
    uint8_t ucEnableZeroRPM;
    uint16_t usFanStopTemperature;
    uint16_t usFanStartTemperature;
} ATOM_Vega10_Fan_Table;

typedef struct _ATOM_Vega10_Fan_Table_V2
{
    uint8_t ucRevId;
    uint16_t usFanOutputSensitivity;
    uint16_t usFanAcousticLimitRpm;
    uint16_t usThrottlingRPM;
    uint16_t usTargetTemperature;
    uint16_t usMinimumPWMLimit;
    uint16_t usTargetGfxClk;
    uint16_t usFanGainEdge;
    uint16_t usFanGainHotspot;
    uint16_t usFanGainLiquid;
    uint16_t usFanGainVrVddc;
    uint16_t usFanGainVrMvdd;
    uint16_t usFanGainPlx;
    uint16_t usFanGainHbm;
    uint8_t ucEnableZeroRPM;
    uint16_t usFanStopTemperature;
    uint16_t usFanStartTemperature;
    uint8_t ucFanParameters;
    uint8_t ucFanMinRPM;
    uint8_t ucFanMaxRPM;
} ATOM_Vega10_Fan_Table_V2;

typedef struct _ATOM_Vega10_Thermal_Controller
{
    uint8_t ucRevId;
    uint8_t ucType;    /* one of ATOM_VEGA10_PP_THERMALCONTROLLER_*/
    uint8_t ucI2cLine; /* as interpreted by DAL I2C */
    uint8_t ucI2cAddress;
    uint8_t ucFanParameters; /* Fan Control Parameters. */
    uint8_t ucFanMinRPM;     /* Fan Minimum RPM (hundreds) -- for display purposes only.*/
    uint8_t ucFanMaxRPM;     /* Fan Maximum RPM (hundreds) -- for display purposes only.*/
    uint8_t ucFlags;         /* to be defined */
} ATOM_Vega10_Thermal_Controller;

typedef struct _ATOM_Vega10_VCE_State_Record
{
    uint8_t ucVCEClockIndex; /*index into usVCEDependencyTableOffset of
                                'ATOM_Vega10_MM_Dependency_Table' type */
    uint8_t ucFlag;          /* 2 bits indicates memory p-states */
    uint8_t ucSCLKIndex;     /* index into ATOM_Vega10_SCLK_Dependency_Table */
    uint8_t ucMCLKIndex;     /* index into ATOM_Vega10_MCLK_Dependency_Table */
} ATOM_Vega10_VCE_State_Record;

typedef struct _ATOM_Vega10_VCE_State_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;
    ATOM_Vega10_VCE_State_Record entries[1];
} ATOM_Vega10_VCE_State_Table;

typedef struct _ATOM_Vega10_PowerTune_Table
{
    uint8_t ucRevId;
    uint16_t usSocketPowerLimit;
    uint16_t usBatteryPowerLimit;
    uint16_t usSmallPowerLimit;
    uint16_t usTdcLimit;
    uint16_t usEdcLimit;
    uint16_t usSoftwareShutdownTemp;
    uint16_t usTemperatureLimitHotSpot;
    uint16_t usTemperatureLimitLiquid1;
    uint16_t usTemperatureLimitLiquid2;
    uint16_t usTemperatureLimitHBM;
    uint16_t usTemperatureLimitVrSoc;
    uint16_t usTemperatureLimitVrMem;
    uint16_t usTemperatureLimitPlx;
    uint16_t usLoadLineResistance;
    uint8_t ucLiquid1_I2C_address;
    uint8_t ucLiquid2_I2C_address;
    uint8_t ucVr_I2C_address;
    uint8_t ucPlx_I2C_address;
    uint8_t ucLiquid_I2C_LineSCL;
    uint8_t ucLiquid_I2C_LineSDA;
    uint8_t ucVr_I2C_LineSCL;
    uint8_t ucVr_I2C_LineSDA;
    uint8_t ucPlx_I2C_LineSCL;
    uint8_t ucPlx_I2C_LineSDA;
    uint16_t usTemperatureLimitTedge;
} ATOM_Vega10_PowerTune_Table;

typedef struct _ATOM_Vega10_PowerTune_Table_V2
{
    uint8_t ucRevId;
    uint16_t usSocketPowerLimit;
    uint16_t usBatteryPowerLimit;
    uint16_t usSmallPowerLimit;
    uint16_t usTdcLimit;
    uint16_t usEdcLimit;
    uint16_t usSoftwareShutdownTemp;
    uint16_t usTemperatureLimitHotSpot;
    uint16_t usTemperatureLimitLiquid1;
    uint16_t usTemperatureLimitLiquid2;
    uint16_t usTemperatureLimitHBM;
    uint16_t usTemperatureLimitVrSoc;
    uint16_t usTemperatureLimitVrMem;
    uint16_t usTemperatureLimitPlx;
    uint16_t usLoadLineResistance;
    uint8_t ucLiquid1_I2C_address;
    uint8_t ucLiquid2_I2C_address;
    uint8_t ucLiquid_I2C_Line;
    uint8_t ucVr_I2C_address;
    uint8_t ucVr_I2C_Line;
    uint8_t ucPlx_I2C_address;
    uint8_t ucPlx_I2C_Line;
    uint16_t usTemperatureLimitTedge;
} ATOM_Vega10_PowerTune_Table_V2;

typedef struct _ATOM_Vega10_PowerTune_Table_V3
{
    uint8_t ucRevId;
    uint16_t usSocketPowerLimit;
    uint16_t usBatteryPowerLimit;
    uint16_t usSmallPowerLimit;
    uint16_t usTdcLimit;
    uint16_t usEdcLimit;
    uint16_t usSoftwareShutdownTemp;
    uint16_t usTemperatureLimitHotSpot;
    uint16_t usTemperatureLimitLiquid1;
    uint16_t usTemperatureLimitLiquid2;
    uint16_t usTemperatureLimitHBM;
    uint16_t usTemperatureLimitVrSoc;
    uint16_t usTemperatureLimitVrMem;
    uint16_t usTemperatureLimitPlx;
    uint16_t usLoadLineResistance;
    uint8_t ucLiquid1_I2C_address;
    uint8_t ucLiquid2_I2C_address;
    uint8_t ucLiquid_I2C_Line;
    uint8_t ucVr_I2C_address;
    uint8_t ucVr_I2C_Line;
    uint8_t ucPlx_I2C_address;
    uint8_t ucPlx_I2C_Line;
    uint16_t usTemperatureLimitTedge;
    uint16_t usBoostStartTemperature;
    uint16_t usBoostStopTemperature;
    uint32_t ulBoostClock;
    uint32_t Reserved[2];
} ATOM_Vega10_PowerTune_Table_V3;

typedef struct _ATOM_Vega10_Hard_Limit_Record
{
    uint32_t ulSOCCLKLimit;
    uint32_t ulGFXCLKLimit;
    uint32_t ulMCLKLimit;
    uint16_t usVddcLimit;
    uint16_t usVddciLimit;
    uint16_t usVddMemLimit;
} ATOM_Vega10_Hard_Limit_Record;

typedef struct _ATOM_Vega10_Hard_Limit_Table
{
    uint8_t ucRevId;
    uint8_t ucNumEntries;
    ATOM_Vega10_Hard_Limit_Record entries[1];
} ATOM_Vega10_Hard_Limit_Table;

typedef struct _Vega10_PPTable_Generic_SubTable_Header
{
    uint8_t ucRevId;
} Vega10_PPTable_Generic_SubTable_Header;
#pragma pack(pop)
