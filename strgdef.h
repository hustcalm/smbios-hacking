/* Copyright (C) 2001-2001 Fujitsu Siemens Computers
   Joachim Braeuer
   This file is part of smbios

   smbios is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   smbios is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with smbios; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

/* $Id: strgdef.h,v 1.2 2002/09/03 10:33:12 bretthauert Exp $
 *
 * $Log: strgdef.h,v $
 * Revision 1.2  2002/09/03 10:33:12  bretthauert
 * fixed a bug with 2.4 kernels (changed kmalloc parameter from
 * GFP_BUFFER to GFP_KERNEL
 *
 * Revision 1.1  2001/09/15 14:52:43  bretthauert
 * initial release
 *
 */

/** \file strgdef.h
 *  String definitions for cooked text.
 *  This file contains the string definitions for human readable output
 *  (directory names, file names, SMBios structure names, attribute names
 *  and attribute values and enums).
 *
 *  \author Joachim Braeuer
 *  \version 0.1
 *  \date March 2001
 */

#ifndef __STRGDEF_H__
#define __STRGDEF_H__


/** name of the diretory in the proc file system that holds the readable
 *   (interpreted, cooked) types */
#define PROC_DIR_STRING                     "smbios"
/** name of the directory in the proc file system that holds the raw (binary)
 *   types */
#define PROC_DIR_STRING_RAW			        "raw"
/** name of the directory in the proc file system that holds the cooked (ascii)
 *   types */
#define PROC_DIR_STRING_COOKED			    "cooked"



/** readable names for smbios structures, they serve as filenames in the /proc file system */
#define RD_BIOS								"bios"
#define RD_SYSTEM							"system"
#define RD_BASEBOARD						"baseboard"
#define RD_ENCLOSURE						"enclosure"
#define RD_PROCESSOR						"processor"
#define RD_MEMCTRL							"memory_controller"
#define RD_MEMMOD							"memory_module"
#define RD_CACHE							"cache"
#define RD_PORT								"port_connector"
#define RD_SLOT								"system_slot"
#define RD_ONBOARD							"onboard_device"
#define RD_OEMSTRINGS						"oem_strings"
#define RD_SYSTEMCONFIG					    "system_configuration"
#define RD_BIOSLANG							"bios_language"
#define RD_GROUPASSOC						"group_association"
#define RD_EVENTLOG							"system_event_log"
#define RD_MEMARRAY							"physical_memory_array"
#define RD_MEMDEV							"physical_memory_device"
#define RD_32MEMERR							"32bit_memory_error_information"
#define RD_MEMMAPPEDADR					    "memory_array_mapped_address"
#define RD_MEMMAPPEDDEV					    "memory_device_mapped_address"
#define RD_POINTINGDEV					    "pointing_device"
#define RD_BATTERY							"portable_battery"
#define RD_RESET							"system_reset"
#define RD_SECURITY							"hardware_security"
#define RD_PWRCTRL							"system_power_controls"
#define RD_VOLTAGE							"voltage_probe"
#define RD_COOLINGDEV						"cooling_device"
#define RD_TEMP								"temperature_probe"
#define RD_CURRENT							"current_probe"
#define RD_RMTACCESS						"out_of_band_remote_access"
#define RD_BIS								"boot_integrity_services"
#define RD_BOOT_INFO						"system_boot_information"
#define RD_64MEMERR							"64bit_memory_error_information"
#define RD_MANAGDEV							"management_device"
#define RD_MANAGDEVCOMP					    "management_device_component"
#define RD_MANAGDEVTHRESH				    "management_device_thresholds"
#define RD_MEMCHANNEL						"memory_channel"
#define RD_IPMI								"ipmi_information"
#define RD_PWRSUP							"system_power_supply"
#define RD_INACTIVE							"inactive"
#define RD_EOT								"end_of_table"


/** readable names of Fujitsu Siemsens smbios structures - system specific area */

#define RDP_OMF_BOARD_ID                "omf_board_id"              // 176
#define RDP_OMF_BIOS_ID                 "omf_bios_id"               // 177
#define RDP_HD_USER_TYPE                "hard_disk_user_type"       // 178
#define RDP_SER_PAR_CONFIG              "serial_parallel_config"    // 179
#define RDP_APM_CONFIG                  "apm_config"                // 180
#define RDP_SYSMON                      "system_monitoring"         // 185
#define RDP_DISPLAY_ID                  "display_id"                // 186
#define RDP_BIOS_ERROR_LOG              "bios_error_log"            // 187
#define RDP_BIOS_ID                     "bios_id"                   // 188
#define RDP_HW_CONTROL                  "hw_control"                // 189

/** readable names of Fujitsu Siemsens smbios structures - manufacturer area */
#define RDP_BOARD_CONFIG                "board_config"              // 208
#define RDP_INT_DEV                     "internal_device"           // 209
#define RDP_EL_COMP                     "electronic_components"     // 210
#define RDP_ADDON_CARD                  "addon_card"                // 211
#define RDP_TEST_RESULTS                "test_results"              // 212
#define RDP_SCSI_DEV                    "scsi_devices"              // 213

#define RDP_BINARY_DATA                 "binary_data"               // 254
#define RDP_FREE_FORM_TEXT              "freeform_text"             // 255


/** system monitoring subtypes (subtypes of 185) */

#define RDP_SM_HOUSING                  "housing"                   // 0
#define RDP_SM_HOUSING_SWITCH           "housing_switch"            // 1
#define RDP_SM_GEOMETRY                 "geometry"                  // 2
#define RDP_SM_FAN_CPU1                 "fan_cpu1"                  // 3
#define RDP_SM_FAN_CPU2                 "fan_cpu2"                  // 4
#define RDP_SM_FAN_AUX                  "fan_aux"                   // 5
#define RDP_SM_FAN_PWR                  "fan_pwr"                   // 6
#define RDP_SM_TEMP_CPU1                "temp_cpu1"                 // 7
#define RDP_SM_TEMP_CPU2                "temp_cpu2"                 // 8
#define RDP_SM_TEMP_AUX                 "temp_aux"                  // 9
#define RDP_SM_WATCHDOG                 "watchdog"                  // 10
#define RDP_SM_VOLTAGES                 "voltages"                  // 11
#define RDP_SM_PWR_CTRL                 "power_control"             // 12
#define RDP_SM_PCI_USAGE                "pci_usage"                 // 13
#define RDP_SM_ECC                      "ecc"                       // 14
#define RDP_SM_USB_SHORT                "usb_short"                 // 15
#define RDP_SM_VBAT                     "voltage_battery"           // 16
#define RDP_SM_PWR_ID                   "power_id"                  // 17
#define RDP_SM_MAN                      "manufacturer"              // 18
#define RDP_SM_MUL_OFF                  "multiplyer_offset"         // 19
#define RDP_SM_PLATTER_ID               "platter_id"                // 20
#define RDP_SM_TIMER                    "timer"                     // 21
#define RDP_SM_TEMP_OUT                 "temp_out"                  // 22
#define RDP_SM_MON_OUT                  "monitor_outlet"            // 23
#define RDP_SM_TEMP_IN                  "temp_in"                   // 24
#define RDP_SM_ADD_MULT_OFF             "add_multiplyer_offset"     // 25
#define RDP_SM_FAN_SYS                  "fan_system"                // 26
#define RDP_SM_BIOS_LOG_MEDIA           "bios_log_media"            // 27
#define RDP_SM_SYSMON_MEDIA             "sysmon_media"              // 28
#define RDP_SM_SYSMON_VCORE             "voltage_core"              // 29
#define RDP_SM_SYSMON_DEVICEID          "device_id"                 // 30
#define RDP_SM_SYSMON_MEMMOD_ID         "memory_module_id"          // 31
#define RDP_SM_SYSMON_MEMPROM_ID        "memory_prom_id"            // 32
#define RDP_SM_SYSMON_IDPROMS_ID        "idprom_id"                 // 33


/** internal device subtypes (subtypes of 209) */

#define RDP_ID_OTHER                    "other"                     // 0
#define RDP_ID_PWRSUP                   "powersupply"               // 1
#define RDP_ID_HD                       "harddisk"                  // 2
#define RDP_ID_FLOPPY                   "floppy"                    // 3
#define RDP_ID_CD                       "cd"                        // 4
#define RDP_ID_MO                       "mo"                        // 5
#define RDP_ID_STREAMER                 "streamer"                  // 6
#define RDP_ID_FLOPTICAL                "floptical"                 // 7
#define RDP_ID_CHIPCARD                 "chipcard"                  // 8

/** electronic component (subtypes of 210) */

#define RDP_EC_OTHER                    "other"                     // 0
#define RDP_EC_CACHE                    "cacher"                    // 1
#define RDP_EC_DRAM                     "dram"                      // 2
#define RDP_EC_VIDEOMEM                 "videomem"                  // 3


/** adapter card (subtypes of 211) */

#define RDP_AC_OTHER                    "other"                     // 0
#define RDP_AC_VIDEO                    "video"                     // 1
#define RDP_AC_LAN                      "lan"                       // 2
#define RDP_AC_SCSI                     "scsi"                      // 3
#define RDP_AC_AUDIO                    "audio"                     // 4
#define RDP_AC_FAX                      "fax"                       // 5
#define RDP_AC_MODEM                    "modem"                     // 6
#define RDP_AC_TV                       "tv"                        // 7
#define RDP_AC_MPEG                     "mpeg"                      // 8


/** test results (subtypes of 212) */

#define RDP_TR_BIOS                     "bios"                      // 0
#define RDP_TR_TDS                      "tds"                       // 1


/** binary data (subtypes of 254) */

#define RDP_BD_MAGIC_NUM                "magic_number"              // 0
#define RDP_BD_FREEFORM1                "freeform1"                 // 1
#define RDP_BD_FREEFORM2                "freeform2"                 // 2



/** 
  * String definitions for the various smbios types.
  * You could use them to get the exact names/tokens/contents
  * of the /proc file system file contents.
  */


/*
 * Header
 */
#define TYPE                            "Type"
#define LENGTH                          "Length"
#define HANDLE                          "Handle"

/*
 * Type 0 - Bios
 */
#define TYPE0_NAME                      "(BIOS Information)"
#define TYPE0_VENDOR                    "Vendor"
#define TYPE0_VERSION                   "Version"
#define TYPE0_ADR_SEG                   "Starting Adr Seg"
#define TYPE0_REL_DATE                  "Rel. Date"
#define TYPE0_ROM_SIZE                  "ROM Size"

#define TYPE0_CHAR                      "Characteristics"
#define TYPE0_CHAR_RES                  "Reserved"
#define TYPE0_CHAR_UNKNOWN              "Unknown"
#define TYPE0_CHAR_NOTSUP               "Not Supported"
#define TYPE0_CHAR_ISA                  "ISA"
#define TYPE0_CHAR_MCA                  "MCA"
#define TYPE0_CHAR_EISA                 "EISA"
#define TYPE0_CHAR_PCI                  "PCI"
#define TYPE0_CHAR_PCMCIA               "PCMCIA"
#define TYPE0_CHAR_PNP                  "Plug and Play"
#define TYPE0_CHAR_APM                  "Advanced Power Management"
#define TYPE0_CHAR_FLASH                "Flash"
#define TYPE0_CHAR_SHADOWING            "Shadowing"
#define TYPE0_CHAR_VL                   "VL-Vesa"
#define TYPE0_CHAR_ESCD                 "ESCD"
#define TYPE0_CHAR_BOOTCD               "Boot from CD"
#define TYPE0_CHAR_SELBOOT              "Selectable Boot"
#define TYPE0_CHAR_BIOS_IS_SOCKETED     "Bios Rom is socketed"
#define TYPE0_CHAR_PCMCIA_BOOT          "Boot from PCMCIA"
#define TYPE0_CHAR_ENH_DISK_DRIVE       "Enhanced Disk Drive"
#define TYPE0_CHAR_FD_NEC               "Int13h - japanese Floppy NEC 1,2 MB"
#define TYPE0_CHAR_FD_TOSHIBA           "Int13h - japanese Floppy Toshiba 1,2 MB"
#define TYPE0_CHAR_360                  "Int13h - 360 kB Floppy"
#define TYPE0_CHAR_1200                 "Int13h - 1,2 MB Floppy"
#define TYPE0_CHAR_720                  "Int13h - 720 kB Floppy"
#define TYPE0_CHAR_2880                 "Int13h - 2,88 MB Floppy"
#define TYPE0_CHAR_PRINT_SCREEN         "Int5h - Print Screen"
#define TYPE0_CHAR_KEYBOARD             "Int9h - 8042 Keyboard"
#define TYPE0_CHAR_SER_SERVICES         "Int14h - Serial Services"
#define TYPE0_CHAR_PRINT_SERVICES       "Int17h - Printer Services"
#define TYPE0_CHAR_VIDEO_SERVICES       "Int10h - CGA,Mono Video Services"
#define TYPE0_CHAR_PC98                 "NEC PC-98"

#define TYPE0_EXT1_ACPI                 "ACPI"
#define TYPE0_EXT1_USB                  "USB Legacy"
#define TYPE0_EXT1_AGP                  "AGP"
#define TYPE0_EXT1_I2O_BOOT             "I2O Boot"
#define TYPE0_EXT1_LS120                "LS-120"
#define TYPE0_EXT1_ATAPI_ZIP_BOOT       "ATAPI ZIP Boot"
#define TYPE0_EXT1_1394_BOOT            "1394 Boot"
#define TYPE0_EXT1_SMART_BATTERY        "Smart Battery"

#define TYPE0_EXT2_BBS                  "Bios Boot Spec."
#define TYPE0_EXT2_NETWORK_BOOT         "Function key initiated Network Service Boot"



/*
 * Type 1 - System
 */
#define TYPE1_NAME                      "(System Information)"
#define TYPE1_MANUFACTURER              "Manufacturer"
#define TYPE1_PRODUCT_NAME              "Product Name"
#define TYPE1_VERSION                   "Version"
#define TYPE1_SERIAL_NUMBER             "Serial Number"
#define TYPE1_UUID                      "UUID"
#define TYPE1_WAKEUP                    "Wakeup Type"

#define TYPE1_WT_RESERVED               "Reserved"
#define TYPE1_WT_OTHER                  "Other"
#define TYPE1_WT_UNKNOWN                "Unknown"
#define TYPE1_WT_APM                    "APM Timer"
#define TYPE1_WT_MODEM                  "Modem Ring"
#define TYPE1_WT_LAN                    "LAN Remote"
#define TYPE1_WT_POWER_SWITCH           "Power Switch"
#define TYPE1_WT_PCI_PME                "PCI PME"
#define TYPE1_WT_AC_PWR_RESTORE         "AC Power Restored"



/*
 * Type 2 - BaseBoard
 */
#define TYPE2_NAME                      "(Baseboard Information)"
#define TYPE2_MANUFACTURER              "Manufacturer"
#define TYPE2_PRODUCT                   "Product"
#define TYPE2_VERSION                   "Version"
#define TYPE2_SERIAL_NUMBER             "Serial Number"



/*
 * Type 3 - Chassis, Enclosure
 */
#define TYPE3_NAME                      "(Enclosure, Chassis)"
#define TYPE3_MANUFACTURER              "Manufacturer"
#define TYPE3_VERSION                   "Version"
#define TYPE3_SERIAL_NUMBER             "Serial Number"
#define TYPE3_ASSET_TAG                 "Asset Tag"

#define TYPE3_CHASSIS_LOCK              "Chassis Lock"
#define TYPE3_CL_PRESENT                "present"
#define TYPE3_CL_NOT_PRESENT            "not present or unknown"

#define TYPE3_TYPE                      "Type"
#define TYPE3_TYPE_OTHER                "Other"
#define TYPE3_TYPE_UNKNOWN              "Unknown"
#define TYPE3_TYPE_DESKTOP              "Desktop"
#define TYPE3_TYPE_LOW_PROFILE          "Low Profile Desktop"
#define TYPE3_TYPE_PIZZA_BOX            "Pizza Box"
#define TYPE3_TYPE_MINI_TOWER           "Mini Tower"
#define TYPE3_TYPE_TOWER                "Tower"
#define TYPE3_TYPE_PORTABLE             "Portable"
#define TYPE3_TYPE_LAPTOP               "LapTop"
#define TYPE3_TYPE_NOTEBOOK             "Notebook"
#define TYPE3_TYPE_HANDHELD             "Hand Held"
#define TYPE3_TYPE_DOCKING_STATION      "Docking Station"
#define TYPE3_TYPE_ALL_IN_ONE           "All in one"
#define TYPE3_TYPE_SUB_NOTEBOOK         "Sub Notebook"
#define TYPE3_TYPE_SPACE_SAVING         "Space Saving"
#define TYPE3_TYPE_LUNCH_BOX            "Lunch Box"
#define TYPE3_TYPE_MAIN_SERVER_CHASSIS  "Main Server Chassis"
#define TYPE3_TYPE_EXPANSION_CHASSIS    "Expansion Chassis"
#define TYPE3_TYPE_SUB_CHASSIS          "Sub Chassis"
#define TYPE3_TYPE_BUS_EXP_CHASSIS      "Bus Expansion Chassis"
#define TYPE3_TYPE_PERIPHERAL_CHASSIS   "Peripheral Chassis"
#define TYPE3_TYPE_RAID_CHASSIS         "RAID Chassis"
#define TYPE3_TYPE_RACK_MOUNT_CHASSIS   "Rack Mount Chassis"
#define TYPE3_TYPE_SEALED_CASE_PC       "Sealed Case PC"

#define TYPE3_BOOT_STATE                "Bootup State"
#define TYPE3_POWER_SUPPLY_STATE        "Power Supply State"
#define TYPE3_THERMAL_STATE             "Thermal State"
#define TYPE3_SECURITY_STATE            "Security State"

#define TYPE3_STATE_OTHER               "Other"
#define TYPE3_STATE_UNKNOWN             "Unknown"
#define TYPE3_STATE_SAFE                "Safe"
#define TYPE3_STATE_WARNING             "Warning"
#define TYPE3_STATE_CRITICAL            "Critical"
#define TYPE3_STATE_NON_RECOVERABLE     "Non-recoverable"

#define TYPE3_SEC_OTHER                 "Other"
#define TYPE3_SEC_UNKNOWN               "Unknown"
#define TYPE3_SEC_NONE                  "None"
#define TYPE3_SEC_EXT_LOCKED_OUT        "External Interface Locked Out"
#define TYPE3_SEC_EXT_ENABLED           "External Interface Enabled"

#define TYPE3_OEM_DEFINED               "OEM defined"



/*
 * Type 4 - Processor
 */
#define TYPE4_NAME                      "(Processor)"
#define TYPE4_SOCKET_DESIGNATION        "Socket Designation"

#define TYPE4_TYPE                      "Type"
#define TYPE4_TYPE_OTHER                "Other"
#define TYPE4_TYPE_UNKNOWN              "Unknown"
#define TYPE4_TYPE_CENTRAL              "Central Processor"
#define TYPE4_TYPE_MATH                 "Math Processor"
#define TYPE4_TYPE_DSP                  "DSP Processor"
#define TYPE4_TYPE_VIDEO                "Video Processor"

#define TYPE4_FAMILY                    "Family"
#define TYPE4_FAMILY_OHTER              "Other"
#define TYPE4_FAMILY_UNKNOWN            "Unknown"
#define TYPE4_FAMILY_8086               "8086"
#define TYPE4_FAMILY_80286              "80286"
#define TYPE4_FAMILY_I386               "Intel 386 (TM)"
#define TYPE4_FAMILY_I486               "Intel 486 (TM)"
#define TYPE4_FAMILY_8087               "8087"
#define TYPE4_FAMILY_80287              "80287"
#define TYPE4_FAMILY_80387              "80387"
#define TYPE4_FAMILY_80487              "80487"
#define TYPE4_FAMILY_PENTIUM            "Pentium (R) family"
#define TYPE4_FAMILY_PENTIUM_PRO        "Pentium (R) Pro"
#define TYPE4_FAMILY_PENTIUM_II         "Pentium (R) II"
#define TYPE4_FAMILY_PENTIUM_MMX        "Pentium (R) with MMX"
#define TYPE4_FAMILY_CELERON            "Celeron (TM)"
#define TYPE4_FAMILY_PENTIUM_II_XEON    "Pentium (R) II XEON (TM)"
#define TYPE4_FAMILY_PENTIUM_III        "Pentium (R) III"
#define TYPE4_FAMILY_M1                 "M1 family"
#define TYPE4_FAMILY_M2                 "M2 family"
#define TYPE4_FAMILY_M1_RES             "Reserved for specific M1 versions"
#define TYPE4_FAMILY_DURON              "AMD Duron (TM) Processor Family"
#define TYPE4_FAMILY_K5                 "K5 family"
#define TYPE4_FAMILY_K6                 "K6 family"
#define TYPE4_FAMILY_K62                "K6-2"
#define TYPE4_FAMILY_K63                "K6-3"
#define TYPE4_FAMILY_ATHLON             "AMD Athlon (TM) Processor Family"
#define TYPE4_FAMILY_29000              "AMD 29000 family"
#define TYPE4_FAMILY_K62P               "K6-2+"
#define TYPE4_FAMILY_POWERPC            "Power PC family"
#define TYPE4_FAMILY_POWERPC_601        "Power PC 601"
#define TYPE4_FAMILY_POWERPC_603        "Power PC 603"
#define TYPE4_FAMILY_POWERPC_603P       "Power PC 603+"
#define TYPE4_FAMILY_POWERPC_604        "Power PC 604"
#define TYPE4_FAMILY_POWERPC_620        "Power PC 620"
#define TYPE4_FAMILY_POWERPC_X704       "Power PC X704"
#define TYPE4_FAMILY_POWERPC_750        "Power PC 750"
#define TYPE4_FAMILY_ALPHA              "Alpha"
#define TYPE4_FAMILY_ALPHA_21064        "Alpha 21064"
#define TYPE4_FAMILY_ALPHA_21066        "Alpha 21066"
#define TYPE4_FAMILY_ALPHA_21164        "Alpha 21164"
#define TYPE4_FAMILY_ALPHA_21164PC      "Alpha 21164PC"
#define TYPE4_FAMILY_ALPHA_21164a       "Alpha"
#define TYPE4_FAMILY_ALPHA_21264        "Alpha 21264"
#define TYPE4_FAMILY_ALPHA_21364        "Alpha 21364"
#define TYPE4_FAMILY_MIPS               "MIPS"
#define TYPE4_FAMILY_MIPS_R4000         "MIPS R4000"
#define TYPE4_FAMILY_MIPS_R4200         "MIPS R4200"
#define TYPE4_FAMILY_MIPS_R4400         "MIPS R4400"
#define TYPE4_FAMILY_MIPS_R4600         "MIPS R4600"
#define TYPE4_FAMILY_MIPS_R10000        "MIPS R10000"
#define TYPE4_FAMILY_SPARC              "SPARC"
#define TYPE4_FAMILY_SPARC_SUPER        "SuperSPARC"
#define TYPE4_FAMILY_SPARC_MICRO_II     "microSPARC II"
#define TYPE4_FAMILY_SPARC_MICRO_IIEP   "microSPARC IIep"
#define TYPE4_FAMILY_SPARC_ULTRA        "UltraSPARC"
#define TYPE4_FAMILY_SPARC_ULTRA_II     "UltraSPARC II"
#define TYPE4_FAMILY_SPARC_ULTRA_II_I   "UltraSPARC IIi"
#define TYPE4_FAMILY_SPARC_ULTRA_III    "UltraSPARC III"
#define TYPE4_FAMILY_SPARC_ULTRA_III_I  "UltraSPARC IIIi"
#define TYPE4_FAMILY_68040              "68040 family"
#define TYPE4_FAMILY_68XXX              "68xxx"
#define TYPE4_FAMILY_68000              "68000"
#define TYPE4_FAMILY_68010              "68010"
#define TYPE4_FAMILY_68020              "68020"
#define TYPE4_FAMILY_68030              "68030"
#define TYPE4_FAMILY_HOBBIT             "Hobbit family"
#define TYPE4_FAMILY_CRUSOE_TM5000      "Crusoe (TM) TM5000 Family"
#define TYPE4_FAMILY_CRUSOE_TM3000      "Crusoe (TM) TM3000 Family"
#define TYPE4_FAMILY_WEITEK             "Weitek"
#define TYPE4_FAMILY_ITANIUM            "Itanium (TM) Processor"
#define TYPE4_FAMILY_PARISC             "PA-RISC family"
#define TYPE4_FAMILY_PARISC_8500        "PA-RISC 8500"
#define TYPE4_FAMILY_PARISC_8000        "PA-RISC 8000"
#define TYPE4_FAMILY_PARISC_7300LC      "PA-RISC 7300LC"
#define TYPE4_FAMILY_PARISC_7200        "PA-RISC 7200"
#define TYPE4_FAMILY_PARISC_7100LC      "PA-RISC 7100LC"
#define TYPE4_FAMILY_PARISC_7100        "PA-RISC 7100"
#define TYPE4_FAMILY_V30                "V30 family"
#define TYPE4_FAMILY_PENTIUM_III_XEON   "Pentium (R) III XEON (TM)"
#define TYPE4_FAMILY_PENTIUM_III_SPEED  "Pentium (R) III with Intel (R) SpeedStep (TM) Technology"
#define TYPE4_FAMILY_PENTIUM_4          "Pentium (R) 4 Processor"
#define TYPE4_FAMILY_AS400              "AS400 Family"
#define TYPE4_FAMILY_IBM390             "IBM390 Family"
#define TYPE4_FAMILY_G4                 "G4"
#define TYPE4_FAMILY_G5                 "G5"
#define TYPE4_FAMILY_I860               "i860"
#define TYPE4_FAMILY_I960               "i960"
#define TYPE4_FAMILY_G4                 "G4"
#define TYPE4_FAMILY_SH3                "SH-3"
#define TYPE4_FAMILY_SH4                "SH-4"
#define TYPE4_FAMILY_ARM                "ARM"
#define TYPE4_FAMILY_STRONG_ARM         "StrongARM"
#define TYPE4_FAMILY_6X86               "6x86"
#define TYPE4_FAMILY_MEDIAGX            "MediaGX"
#define TYPE4_FAMILY_MII                "MII"
#define TYPE4_FAMILY_WINCHIP            "WinChip"
#define TYPE4_FAMILY_DSP                "DSP"
#define TYPE4_FAMILY_VIDEO              "Video Processor"

#define TYPE4_MANUFACTURER              "Manufacturer"
#define TYPE4_ID                        "ID"
#define TYPE4_VERSION                   "Version"
#define TYPE4_CURRENT_VOLTAGE           "Current Voltage"

#define TYPE4_VOLTAGE                   "Voltage"
#define TYPE4_VOLTAGE_50                "5V supported"
#define TYPE4_VOLTAGE_33                "3.3V supported"
#define TYPE4_VOLTAGE_29                "2.9V supported"

#define TYPE4_EXTERNAL_CLOCK            "External Clock"
#define TYPE4_MAX_SPEED                 "Max Speed"
#define TYPE4_CURRENT_SPEED             "Current Speed"

#define TYPE4_STATUS                    "Status"
#define TYPE4_STATUS_UNKNOWN            "Unknown"
#define TYPE4_STATUS_ENABLED            "CPU enabled"
#define TYPE4_STATUS_DISABLED_USER      "CPU disabled by User via BIOS Setup"
#define TYPE4_STATUS_DISABLED_POST      "CPU disabled by Bios (POST error)"
#define TYPE4_STATUS_IDLE               "CPU is idle, waiting to be enabled"
#define TYPE4_STATUS_OTHER              "Other"

#define TYPE4_SOCKET                    "Socket"
#define TYPE4_SOCKET_POPULATED          "populated"
#define TYPE4_SOCKET_UNPOPULATED        "unpopulated"

#define TYPE4_UPGRADE                   "Upgrade"
#define TYPE4_UPGRADE_OTHER             "Other"
#define TYPE4_UPGRADE_UNKNOWN           "Unknown"
#define TYPE4_UPGRADE_DAUGHTER          "Daughter Board"
#define TYPE4_UPGRADE_ZIFF              "ZIF Socket"
#define TYPE4_UPGRADE_PIGGY_BACK        "Replaceable Piggy Bag"
#define TYPE4_UPGRADE_NONE              "None"
#define TYPE4_UPGRADE_LIF               "LIF Socket"
#define TYPE4_UPGRADE_SLOT1             "Slot 1"
#define TYPE4_UPGRADE_SLOT2             "Slot 2"
#define TYPE4_UPGRADE_370               "370 Pin Socket"
#define TYPE4_UPGRADE_A                 "Slot A"
#define TYPE4_UPGRADE_M                 "Slot M"
#define TYPE4_UPGRADE_423               "Socket 423"
#define TYPE4_UPGRADE_A462              "Socket A (Socket 462)"


#define TYPE4_L1_CACHE_HANDLE           "L1 Cache Handle"
#define TYPE4_L2_CACHE_HANDLE           "L2 Cache Handle"
#define TYPE4_L3_CACHE_HANDLE           "L3 Cache Handle"

#define TYPE4_SERIAL_NUMBER             "Serial Number"
#define TYPE4_ASSET_TAG                 "Asset Tag"
#define TYPE4_PART_NUMBER               "Part Number"



/*
 * Type 5 - Memory Controller
 */
#define TYPE5_NAME                      "(Memory Controller Information)"

#define TYPE5_ERROR_DETECTION           "Error Detection"
#define TYPE5_ED_OTHER                  "Other"
#define TYPE5_ED_UNKNOWN                "Unknown"
#define TYPE5_ED_NONE                   "None"
#define TYPE5_ED_PARITY                 "8-Bit Parity"
#define TYPE5_ED_32ECC                  "32-Bit ECC"
#define TYPE5_ED_64ECC                  "64-Bit ECC"
#define TYPE5_ED_128ECC                 "128-Bit ECC"
#define TYPE5_ED_CRC                    "CRC"

#define TYPE5_ECC                       "Supported ECC"
#define TYPE5_ECC_OTHER                 "Other"
#define TYPE5_ECC_UNKNOWN               "Unknown"
#define TYPE5_ECC_NONE                  "None"
#define TYPE5_ECC_SINGLE_BIT            "Single Bit Error Correction"
#define TYPE5_ECC_DOUBLE_BIT            "Double Bit Error Correction"
#define TYPE5_ECC_SCRUBBING             "Error Scrubbing"

#define TYPE5_SUPPORTED_INTERLEAVE      "Supported Interleave"
#define TYPE5_CURRENT_INTERLEAVE        "Current Interleave"
#define TYPE5_I_OTHER                   "Other"
#define TYPE5_I_UNKNOWN                 "Unknown"
#define TYPE5_I_ONE_WAY                 "One Way Interleave"
#define TYPE5_I_TWO_WAY                 "Two Way Interleave"
#define TYPE5_I_FOUR_WAY                "Four Way Interleave"
#define TYPE5_I_EIGHT_WAY               "Eight Way Interleave"
#define TYPE5_I_SIXTEEN_WAY             "Sixteen Way Interleave"

#define TYPE5_MAX_MEM_MOD_SIZE          "Max Memory Module Size"

#define TYPE5_SUPPORTED_SPEEDS          "Supported Speeds"
#define TYPE5_SSPEED_OTHER              "Other"
#define TYPE5_SSPEED_UNKNOWN            "Unknown"
#define TYPE5_SSPEED_70                 "70 ns"
#define TYPE5_SSPEED_60                 "60 ns"
#define TYPE5_SSPEED_50                 "50 ns"

#define TYPE5_SUPPORTED_TYPES           "Supported Types"
#define TYPE5_ST_OTHER                  "Other"
#define TYPE5_ST_UNKNOWN                "Unknown"
#define TYPE5_ST_STANDARD               "Standard"
#define TYPE5_ST_FAST_PAGE              "Fast Page Mode"
#define TYPE5_ST_EDO                    "EDO"
#define TYPE5_ST_PARITY                 "Parity"
#define TYPE5_ST_ECC                    "ECC"
#define TYPE5_ST_SIMM                   "SIMM"
#define TYPE5_ST_DIMM                   "DIMM"
#define TYPE5_ST_BURST_EDO              "Burst EDO"
#define TYPE5_ST_SDRAM                  "SDRAM"

#define TYPE5_SUPPORTED_VOLTAGES        "Supported Voltages"
#define TYPE5_SV_50                     "5V"
#define TYPE5_SV_33                     "3.3V"
#define TYPE5_SV_29                     "2.9V"

#define TYPE5_NR_ASSOC_SLOTS            "Nr of associated Slots"
#define TYPE5_SLOTS                     "Slots"
#define TYPE5_MEM_MOD_HANDLES           "Memory Module Handles"

#define TYPE5_ENABLED_ECC               "Enabled ECC"



/*
 * Type 6 - Memory Module
 */
#define TYPE6_NAME                      "(Memory Module Information)"
#define TYPE6_SOCKET_DESIGNATION        "Socket Designation"
#define TYPE6_RAS                       "RAS#"
#define TYPE6_CURRENT_SPEED             "Current Speed"

#define TYPE6_CURRENT_TYPE              "Current Type"
#define TYPE6_CT_OTHER                  "Other"
#define TYPE6_CT_UNKNOWN                "Unknown"
#define TYPE6_CT_STANDARD               "Standard"
#define TYPE6_CT_FAST_PAGE              "Fast Page Mode"
#define TYPE6_CT_EDO                    "EDO"
#define TYPE6_CT_PARITY                 "Parity"
#define TYPE6_CT_ECC                    "ECC"
#define TYPE6_CT_SIMM                   "SIMM"
#define TYPE6_CT_DIMM                   "DIMM"
#define TYPE6_CT_BURST_EDO              "Burst EDO"
#define TYPE6_CT_SDRAM                  "SDRAM"

#define TYPE6_INSTALLED_SIZE            "Installed Size"
#define TYPE6_ENABLED_SIZE              "Enabled Size"
#define TYPE6_S_DBL_BANK                "Double Bank Connection"
#define TYPE6_S_SINGLE_BANK             "Single Bank Connection"
#define TYPE6_S_NOT_INSTALLED           "Not Installed"
#define TYPE6_S_NOT_ENABLED             "Installed, but not enabled"
#define TYPE6_S_NOT_DETERMINABLE        "Not Determinable"

#define TYPE6_STATUS                    "Status"
#define TYPE6_STATUS_UNCORRECTABLE      "Uncorrectable error detected. Memory has been disabled."
#define TYPE6_STATUS_CORRECTABLE        "Correctable error detected."
#define TYPE6_STATUS_EVENTLOG           "Get Status from event log. Rest of status is invalid."



/*
 * Type 7 - Cache
 */
#define TYPE7_NAME                      "(Cache Information)"
#define TYPE7_SOCKET_DESIGNATION        "Socket Designation"

#define TYPE7_MODE                      "Operational Mode"
#define TYPE7_MODE_WT                   "Write Through"
#define TYPE7_MODE_WB                   "Write Back"
#define TYPE7_MODE_VAR                  "Varies with address"
#define TYPE7_MODE_UNK                  "Unknown"

#define TYPE7_EN                        "Enabled at boot time"

#define TYPE7_LOCATION                  "Location"
#define TYPE7_LOC_INT                   "Internal"
#define TYPE7_LOC_EXT                   "External"
#define TYPE7_LOC_RES                   "Reserved"
#define TYPE7_LOC_UNK                   "Unknown"

#define TYPE7_SOCKETED                  "Socketed"

#define TYPE7_LEVEL                     "Level"

#define TYPE7_MAX_SIZE                  "Max. Size"
#define TYPE7_INST_SIZE                 "Installed Size"

#define TYPE7_SUP_SRAM_TYPE             "Supported SRAM Type"
#define TYPE7_CUR_SRAM_TYPE             "Current SRAM Type"
#define TYPE7_ST_OTHER                  "Other"
#define TYPE7_ST_UNKNOWN                "Unknown"
#define TYPE7_ST_NON_BURST              "Non-Burst"
#define TYPE7_ST_BURST                  "Burst"
#define TYPE7_ST_PIPELINE_BURST         "Pipline Burst"
#define TYPE7_ST_SYNCHRONOUS            "Synchronous"
#define TYPE7_ST_ASYNCHRONOUS           "Asynchronous"

#define TYPE7_SPEED                     "Speed"

#define TYPE7_ECC_TYPE                  "Error Correction Type"
#define TYPE7_ECC_OTHER                 "Other"
#define TYPE7_ECC_UNKNOWN               "Unknown"
#define TYPE7_ECC_NONE                  "None"
#define TYPE7_ECC_PARITY                "Parity"
#define TYPE7_ECC_SINGLE_ECC            "Single Bit ECC"
#define TYPE7_ECC_MULIT_ECC             "Multi Bit ECC"

#define TYPE7_CACHE_TYPE                "Cache Type"
#define TYPE7_TYPE_OTHER                "Other"
#define TYPE7_TYPE_UNKNOWN              "Unknown"
#define TYPE7_TYPE_INSTRUCTION          "Instruction"
#define TYPE7_TYPE_DATA                 "Data"
#define TYPE7_TYPE_UNIFIED              "Unified"

#define TYPE7_ASSOCIATIVITY             "Associativity"
#define TYPE7_ASSOC_OTHER               "Other"
#define TYPE7_ASSOC_UNKNOWN             "Unknown"
#define TYPE7_ASSOC_DIREC_MAPPED        "Direct Mapped"
#define TYPE7_ASSOC_2WAY                "2 Way Set Associative"
#define TYPE7_ASSOC_4WAY                "4 Way Set Associative"
#define TYPE7_ASSOC_FULL                "Fully Associative"
#define TYPE7_ASSOC_8WAY                "8 Way Set Associative"
#define TYPE7_ASSOC_16WAY               "16 Way Set Associative"



/*
 * Type 8 - Port Connector
 */
#define TYPE8_NAME                      "(Port Connector Information)"
#define TYPE8_INT_REF_DESIGNATOR        "Internal Reference Designator"
#define TYPE8_INT_TYPE                  "Internal Connector Type"
#define TYPE8_EXT_REF_DESIGNATOR        "External Reference Designator"
#define TYPE8_EXT_TYPE                  "External Connector Type"
#define TYPE8_TYPE                      "Port Type"

#define TYPE8_CT_NONE                   "None"
#define TYPE8_CT_CENTRONICS             "Centronics"
#define TYPE8_CT_MINI_CENTRONICS        "Mini Centronics"
#define TYPE8_CT_PROPRIETARY            "Proprietary"
#define TYPE8_CT_DB25_MALE              "DB-25 male"
#define TYPE8_CT_DB25_FEMALE            "DB-25 female"
#define TYPE8_CT_DB15_MALE              "DB-15 male"
#define TYPE8_CT_DB15_FEMALE            "DB-15 female"
#define TYPE8_CT_DB9_MALE               "DB-9 male"
#define TYPE8_CT_DB9_FEMALE             "DB-9 female"
#define TYPE8_CT_RJ11                   "RJ-11"
#define TYPE8_CT_RJ45                   "RJ-45"
#define TYPE8_CT_MINI_SCSI              "50 Pin Mini SCSI"
#define TYPE8_CT_MINI_DIN               "Mini DIN"
#define TYPE8_CT_MICRO_DIN              "Micro DIN"
#define TYPE8_CT_PS2                    "PS/2"
#define TYPE8_CT_INFRARED               "Infrared"
#define TYPE8_CT_HPHIL                  "HP-HIL"
#define TYPE8_CT_USB                    "Access Bus (USB)"
#define TYPE8_CT_SSA_SCSI               "SSA SCSI"
#define TYPE8_CT_DIN8_MALE              "Circular DIN-8 male"
#define TYPE8_CT_DIN8_FEMALE            "Circular DIN-8 female"
#define TYPE8_CT_ONBOARD_IDE            "On Board IDE"
#define TYPE8_CT_ONBOARD_FLOPPY         "On Board Floppy"
#define TYPE8_CT_9PIN_DIL               "9 Pin Dual Inline (Pin 10 cut)"
#define TYPE8_CT_25PIN_DIL              "25 Pin Dual Inline (Pin 26 cut)"
#define TYPE8_CT_50PIN_DIL              "50 Pin Dual Inline"
#define TYPE8_CT_68PIN_DIL              "68 Pin Dual Inline"
#define TYPE8_CT_ONBOARD_SND_CD         "On Board Sound Input From CD"
#define TYPE8_CT_MINI_CENTRONICS_14     "Mini Centronics Type 14"
#define TYPE8_CT_MINI_CENTRONICS_26     "Mini Centronics Type 26"
#define TYPE8_CT_MINI_JACK_PHONES       "Mini Jack (Headphones)"
#define TYPE8_CT_BNC                    "BNC"
#define TYPE8_CT_1394                   "1394"
#define TYPE8_CT_PC98                   "PC-98"
#define TYPE8_CT_PC98HIRESO             "PC-98Hireso"
#define TYPE8_CT_PCH98                  "PC-H98"
#define TYPE8_CT_PC98NOTE               "PC-98Note"
#define TYPE8_CT_PC98FULL               "PC-98Full"
#define TYPE8_CT_OTHER                  "Other - look at Reference Designator"

#define TYPE8_TYPE_NONE                 "None"
#define TYPE8_TYPE_PAR_XTAT             "Parallel Port XT/AT Compatible"
#define TYPE8_TYPE_PAR_PS2              "Parallel Port PS/2"
#define TYPE8_TYPE_PAR_ECP              "Parallel Port ECP"
#define TYPE8_TYPE_PAR_EPP              "Parallel Port EPP"
#define TYPE8_TYPE_PAR_ECP_EPP          "Parallel Port ECP/EPP"
#define TYPE8_TYPE_SER_XTAT             "Serial Port XT/AT Compatible"
#define TYPE8_TYPE_SER_16450            "Serial Port 16450 Compatible"
#define TYPE8_TYPE_SER_16550            "Serial Port 16550 Compatible"
#define TYPE8_TYPE_SER_16550A           "Serial Port 16550A Compatible"
#define TYPE8_TYPE_SCSI                 "SCSI Port"
#define TYPE8_TYPE_MIDI                 "Midi Port"
#define TYPE8_TYPE_JOYSTICK             "Joystick Port"
#define TYPE8_TYPE_KEYBOARD             "Keyboard Port"
#define TYPE8_TYPE_MOUSE                "Mouse Port"
#define TYPE8_TYPE_SSA_SCSI             "SSA SCSI"
#define TYPE8_TYPE_USB                  "USB"
#define TYPE8_TYPE_1394                 "FireWire (IEEE P1394)"
#define TYPE8_TYPE_PCMCIA_I             "PCMCIA Type I"
#define TYPE8_TYPE_PCMCIA_II            "PCMCIA Type II"
#define TYPE8_TYPE_PCMCIA_III           "PCMCIA Type III"
#define TYPE8_TYPE_CARDBUS              "Cardbus"
#define TYPE8_TYPE_ACCESSBUS            "Access Bus Port"
#define TYPE8_TYPE_SCSI_II              "SCSI II"
#define TYPE8_TYPE_SCSI_WIDE            "SCSI Wide"
#define TYPE8_TYPE_PC98                 "PC-98"
#define TYPE8_TYPE_PC98_HIRESO          "PC-98 Hireso"
#define TYPE8_TYPE_PCH98                "PC-H98"
#define TYPE8_TYPE_VIDEO                "Video Port"
#define TYPE8_TYPE_AUDIO                "Audio Port"
#define TYPE8_TYPE_MODEM                "Modem Port"
#define TYPE8_TYPE_NET                  "Network Port"
#define TYPE8_TYPE_8251                 "8251 Compatible"
#define TYPE8_TYPE_8251_FIFO            "8251 FIFO Compatible"
#define TYPE8_TYPE_OTHER                "Other"



/*
 * Type 9 - System Slots
 */
#define TYPE9_NAME                      "(System Slot Information)"
#define TYPE9_DESIGNATION               "Designation"

#define TYPE9_TYPE                      "Type"
#define TYPE9_TYPE_OTHER                "Other"
#define TYPE9_TYPE_UNKNOWN              "Unknown"
#define TYPE9_TYPE_ISA                  "ISA"
#define TYPE9_TYPE_MCA                  "MCA"
#define TYPE9_TYPE_EISA                 "EISA"
#define TYPE9_TYPE_PCI                  "PCI"
#define TYPE9_TYPE_PCMCIA               "PCMCIA"
#define TYPE9_TYPE_VL                   "VL-VESA"
#define TYPE9_TYPE_PROPRIETARY          "Proprietary"
#define TYPE9_TYPE_PROCESSOR            "Processor Card Slot"
#define TYPE9_TYPE_MEMORY               "Proprietary Memory Card Slot"
#define TYPE9_TYPE_RISER                "I/O Riser Card Slot"
#define TYPE9_TYPE_NUBUS                "NuBus"
#define TYPE9_TYPE_PCI66                "PCI-66MHz Capable"
#define TYPE9_TYPE_AGP                  "AGP"
#define TYPE9_TYPE_AGP2                 "AGP 2x"
#define TYPE9_TYPE_AGP4                 "AGP 4x"
#define TYPE9_TYPE_PCI_X                "PCI-X"
#define TYPE9_TYPE_PC98_C20             "PC-98/C20"
#define TYPE9_TYPE_PC98_C24             "PC-98/C24"
#define TYPE9_TYPE_PC98_E               "PC-98/E"
#define TYPE9_TYPE_PC98_LOCAL_BUS       "PC-98/Local Bus"
#define TYPE9_TYPE_PC98_CARD            "PC-98/Card"

#define TYPE9_BUS_WIDTH                 "Data Bus Width"
#define TYPE9_BW_OTHER                  "Other"
#define TYPE9_BW_UNKNOWN                "Unknown"
#define TYPE9_BW_8                      "8 Bit"
#define TYPE9_BW_16                     "16 Bit"
#define TYPE9_BW_32                     "32 Bit"
#define TYPE9_BW_64                     "64 Bit"
#define TYPE9_BW_128                    "128 Bit"

#define TYPE9_CURRENT_USAGE             "Current Usage"
#define TYPE9_CU_OTHER                  "Other"
#define TYPE9_CU_UNKNOWN                "Unknown"
#define TYPE9_CU_AVAILABLE              "Available"
#define TYPE9_CU_IN_USE                 "In Use"

#define TYPE9_LENGTH                    "Slot Length"
#define TYPE9_LENGTH_OTHER              "Other"
#define TYPE9_LENGTH_UNKNOWN            "Unknown"
#define TYPE9_LENGTH_SHORT              "Short Length"
#define TYPE9_LENGTH_LONG               "Long Length"

#define TYPE9_ID_MCA_SLOT_NR            "MCA Slot Number"
#define TYPE9_ID_EISA_SLOT_NR           "EISA Slot Number"
#define TYPE9_ID_PCI_SLOT_NR            "Slot Number"
#define TYPE9_ID_PCMCIA_ADAPTER_NR      "PCMCIA Adapter Adapter Number"
#define TYPE9_ID_PCMCIA_SOCKET_NR       "PCMCIA Socket Number"

#define TYPE9_CHARACTERISTICS           "Characteristics"
#define TYPE9_CHAR_UNKNOWN              "Unknown"
#define TYPE9_CHAR_5V                   "Provides 5.0 V"
#define TYPE9_CHAR_33V                  "Provides 3.3 V"
#define TYPE9_CHAR_SHARED               "Shared"
#define TYPE9_CHAR_PC_CARD_16           "PC Card Slot supports PC Card-16"
#define TYPE9_CHAR_CARDBUS              "PC Card Slot supports CardBus"
#define TYPE9_CHAR_ZOOM_VIDEO           "PC Card Slot supports Zoom Video"
#define TYPE9_CHAR_MODEM_RESUME         "PC Card Slot supports Modem Ring Resume"
#define TYPE9_CHAR_PME                  "PCI slot supports PME"
#define TYPE9_CHAR_HOT_PLUG             "Slot supports hot plug devices"
#define TYPE9_CHAR_SMBUS                "PCI slot supports SMBus signal"



/*
 * Type 10 - Onboard Devices
 */
#define TYPE10_NAME                     "(Onboard Device Information)"
#define TYPE10_NR                       "Device Number"
#define TYPE10_STATUS                   "Status"
#define TYPE10_TYPE                     "Type"
#define TYPE10_DEV_NAME                 "Device Name"

#define TYPE10_STATUS_ENABLED           "Enabled"
#define TYPE10_STATUS_DISABLED          "Disabled"

#define TYPE10_TYPE_OTHER               "Other"
#define TYPE10_TYPE_UNKNOWN             "Unknown"
#define TYPE10_TYPE_VIDEO               "Video"
#define TYPE10_TYPE_SCSI                "SCSI Controller"
#define TYPE10_TYPE_ETHERNET            "Ethernet"
#define TYPE10_TYPE_TOKEN_RING          "Token Ring"
#define TYPE10_TYPE_SOUND               "Sound"



/*
 * Type 11 - OEM Strings
 */
#define TYPE11_NAME                     "(OEM Strings)"
#define TYPE11_NR                       "String"



/*
 * Type 12 - System Configuration Options
 */
#define TYPE12_NAME                     "(System Configuration Options)"
#define TYPE12_OPTION                   "Option"



/*
 * Type 13 - Bios Language Information
 */
#define TYPE13_NAME                     "(Bios Language Information)"

#define TYPE13_FORMAT                   "Format"
#define TYPE13_F_SHORT                  "Short"
#define TYPE13_F_LONG                   "Long"

#define TYPE13_SUP_LANG                 "Supported Languages"

#define TYPE13_CUR_LANG                 "Current Language"



/*
 * Type 16 - Physical Memory Array
 */
#define TYPE16_NAME                     "(Physical Memory Array)"

#define TYPE16_LOCATION                 "Location"
#define TYPE16_L_OTHER                  "Other"
#define TYPE16_L_UNKNOWN                "Unknown"
#define TYPE16_L_MOTHERBOARD            "Motherboard"
#define TYPE16_L_ISA                    "ISA add-on card"
#define TYPE16_L_EISA                   "EISA add-on card"
#define TYPE16_L_PCI                    "PCI add-on card"
#define TYPE16_L_MCA                    "MCA add-on card"
#define TYPE16_L_PCMCIA                 "PCMCIA add-on card"
#define TYPE16_L_PRORIETARY             "Proprietary add-on card"
#define TYPE16_L_NUBUS                  "NuBus add-on card"
#define TYPE16_L_PC98C20                "PC-98/C20 add-on card"
#define TYPE16_L_PC98C24                "PC-98/C24 add-on card"
#define TYPE16_L_PC98E                  "PC-98/E add-on card"
#define TYPE16_L_PC98LOCAL              "PC-98/Local Bus add-on card"
#define TYPE16_L_PC98CARD               "PC-98/Card Slot add-on card"

#define TYPE16_USE                      "Used for"
#define TYPE16_USE_OTHER                "Other"
#define TYPE16_USE_UNKNOWN              "Unknown"
#define TYPE16_USE_SYSTEM               "System Memory"
#define TYPE16_USE_VIDEO                "Video Memory"
#define TYPE16_USE_FLASH                "Flash Memory"
#define TYPE16_USE_NON_VOLATILE         "Non Volatile Memory"
#define TYPE16_USE_CACHE                "Cache Memory"

#define TYPE16_ECC                      "ECC"
#define TYPE16_ECC_OTHER                "Other"
#define TYPE16_ECC_UNKNOWN              "Unknown"
#define TYPE16_ECC_NONE                 "None"
#define TYPE16_ECC_PARITY               "Parity"
#define TYPE16_ECC_SINGLE_ECC           "Single Bit ECC"
#define TYPE16_ECC_DOUBLE_ECC           "Double Bit ECC"
#define TYPE16_ECC_CRC                  "CRC"

#define TYPE16_MAX_CAPACITY             "Max. Capacity"
#define TYPE16_MC_UNKNOWN               "Unknown"

#define TYPE16_ERROR_HANDLE             "Memory Error Handle"
#define TYPE16_EH_NOT_SUPPORTED         "Error Information Structure is not supported."
#define TYPE16_EH_NO_ERROR              "No error detected."
#define TYPE16_EH_HANDLE                "Handle"

#define TYPE16_NR_DEVICES               "Nr. of Slots available"



/*
 * Type 17 - Memory Device
 */
#define TYPE17_NAME                     "(Memory Device)"
#define TYPE17_MEM_ARRAY_HANDLE         "Physical Memory Array Handle"

#define TYPE17_ERROR_HANDLE             "Memory Error Handle"
#define TYPE17_EH_NOT_SUPPORTED         "Error Information Structure is not supported."
#define TYPE17_EH_NO_ERROR              "No error detected."
#define TYPE17_EH_HANDLE                "Handle"

#define TYPE17_TOTAL_WIDTH              "Total Width (incl. ECC Bits)"
#define TYPE17_TW_UNKNOWN               "Unknown"

#define TYPE17_DATA_WIDTH               "Data Width"
#define TYPE17_DW_UNKNOWN               "Unknown"

#define TYPE17_SIZE                     "Size"
#define TYPE17_SIZE_UNKNOWN             "Unknown"
#define TYPE17_SIZE_NOT_INSTALLED       "No Memory Device Installed"

#define TYPE17_FORM_FACTOR              "Form Factor"
#define TYPE17_FF_OTHER                 "Other"
#define TYPE17_FF_UNKNOWN               "Unknown"
#define TYPE17_FF_SIMM                  "SIMM"
#define TYPE17_FF_SIP                   "SIP"
#define TYPE17_FF_CHIP                  "Chip"
#define TYPE17_FF_DIP                   "DIP"
#define TYPE17_FF_ZIP                   "ZIP"
#define TYPE17_FF_PROPRIETARY           "Proprietary Card"
#define TYPE17_FF_DIMM                  "DIMM"
#define TYPE17_FF_TSOP                  "TSOP"
#define TYPE17_FF_ROW                   "Row of chips"
#define TYPE17_FF_RIMM                  "RIMM"
#define TYPE17_FF_SODIMM                "SODIMM"
#define TYPE17_FF_SRIMM                 "SRIMM"

#define TYPE17_DEVICE_SET               "Device Set"
#define TYPE17_DEVICE_SET_UNKONWN       "Unknown"
#define TYPE17_DEVICE_SET_NOT_IN_SET    "Not part of a set"

#define TYPE17_DEVICE_LOCATOR           "Device Locator"
#define TYPE17_BANK_LOCATOR             "Bank Locator"

#define TYPE17_MEM_TYPE                 "Memory Type"
#define TYPE17_MT_OTHER                 "Other"
#define TYPE17_MT_UNKNOWN               "Unknown"
#define TYPE17_MT_DRAM                  "DRAM"
#define TYPE17_MT_EDRAM                 "EDRAM"
#define TYPE17_MT_VRAM                  "VRAM"
#define TYPE17_MT_SRAM                  "SRAM"
#define TYPE17_MT_RAM                   "RAM"
#define TYPE17_MT_ROM                   "ROM"
#define TYPE17_MT_FLASH                 "FLASH"
#define TYPE17_MT_EEPROM                "EEPROM"
#define TYPE17_MT_FEPROM                "FEPROM"
#define TYPE17_MT_EPROM                 "EPROM"
#define TYPE17_MT_CDRAM                 "CDRAM"
#define TYPE17_MT_3DRAM                 "3DRAM"
#define TYPE17_MT_SDRAM                 "SDRAM"
#define TYPE17_MT_SGRAM                 "SGRAM"
#define TYPE17_MT_RDRAM                 "RDRAM"
#define TYPE17_MT_DDR                   "DDR"

#define TYPE17_TYPE_DETAIL              "Type Detail"
#define TYPE17_TD_RESERVED              "Reserved"
#define TYPE17_TD_OTHER                 "Other"
#define TYPE17_TD_UNKNOWN               "Unknown"
#define TYPE17_TD_FAST_PAGE             "Fast Paged"
#define TYPE17_TD_STATIC_COLUMN         "Static Column"
#define TYPE17_TD_PSEUDO_STATIC         "Pseudo Static"
#define TYPE17_TD_RAMBUS                "RAMBUS"
#define TYPE17_TD_SYNCHRONOUS           "Synchronous"
#define TYPE17_TD_CMOS                  "CMOS"
#define TYPE17_TD_EDO                   "EDO"
#define TYPE17_TD_WINDOW_DRAM           "Window DRAM"
#define TYPE17_TD_CACHE_DRAM            "Cache DRAM"
#define TYPE17_TD_NON_VOLATILE          "Non Volatile"

#define TYPE17_SPEED                    "Speed"
#define TYPE17_MANUFACTURER             "Manufacturer"
#define TYPE17_SERIAL_NUMBER            "Serial Number"
#define TYPE17_ASSET_TAG                "Asset Tag"
#define TYPE17_PART_NUMBER              "Part Number"



/*
 * Type 19 - Memory Array Mapped Address
 */
#define TYPE19_NAME                     "(Memory Array Mapped Address)"
#define TYPE19_STARTING_ADR             "Physical Starting Address"
#define TYPE19_ENDING_ADR               "Physical Ending Address"
#define TYPE19_MEMORY_ARRAY_HANDLE      "Physical Memory Array Handle"
#define TYPE19_PARTITION_WIDTH          "Partition Width"



/*
 * Type 20 - Memory Device Mapped Address
 */
#define TYPE20_NAME                     "(Memory Device Mapped Address)"
#define TYPE20_START_ADR                "Starting Address"
#define TYPE20_END_ADR                  "Ending Address"
#define TYPE20_MEM_DEV_HANDLE           "Memory Device Handle"
#define TYPE20_MEM_ARRAY_MAPPED_HANDLE  "Memory Array Mapped Address Handle"

#define TYPE20_PARTITION_ROW_POSITION   "Partition Row Position"
#define TYPE20_PRP_UNKNOWN              "Unknown"

#define TYPE20_INTERLEAVE_POSITION      "Interleave Position"
#define TYPE20_IP_UNKNOWN               "Unknown"
#define TYPE20_IP_NOT_INTERLEAVED       "Not Interleaved"

#define TYPE20_INTERLEAVED_DATA_DEPTH   "Interleaved Data Depth"
#define TYPE20_IDD_UNKNOWN              "Unknown"
#define TYPE20_IDD_NOT_INTERLEAVED      "Not Interleaved"



/*
 * Type 32 - System Boot Information
 */
#define TYPE32_NAME                     "(System Boot Information)"

#define TYPE32_STATUS                   "Status"
#define TYPE32_STATUS_NO_ERROR          "No errors detected"
#define TYPE32_STATUS_NO_BOOT_MEDIA     "No bootable media"
#define TYPE32_STATUS_NO_OS             "Operating System failed to load"
#define TYPE32_STATUS_FIRMWARE_HW       "Firmware detected HW failure"
#define TYPE32_STATUS_OS_HW             "Operating System detected HW failure"
#define TYPE32_STATUS_USER              "User requested boot (e.g. by keystroke)"
#define TYPE32_STATUS_SECURITY          "System security violation"
#define TYPE32_STATUS_REQUEST           "Previously requested image (e.g. via PXE)"
#define TYPE32_STATUS_WATCHDOG          "A expired watchdog caused the system to reboot."



/*
 * Type 127 - System Boot Information
 */
#define TYPE127_NAME                    "(End of Table)"
#define TYPE127_DESC                    "Description"
#define TYPE127_DESC_TEXT               "This is the very last structure of the SMBios structure table."


/*
 * misc
 */
#define BIT                             "Bit"
#define BYTES                           "Bytes"
#define KB                              "kB"
#define MB                              "MB"
#define MHZ                             "MHz"
#define NS                              "ns"
#define SMB_TRUE                        "True"
#define SMB_FALSE                       "False"
#define NOT_SUPPORTED                   "This structure is not supported so far."

#define SEP1                            ":"
#define SEP2                            ">"



#endif
