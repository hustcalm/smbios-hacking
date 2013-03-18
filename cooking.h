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

/* $Id: cooking.h,v 1.2 2002/09/03 10:33:12 bretthauert Exp $
 *
 * $Log: cooking.h,v $
 * Revision 1.2  2002/09/03 10:33:12  bretthauert
 * fixed a bug with 2.4 kernels (changed kmalloc parameter from
 * GFP_BUFFER to GFP_KERNEL
 *
 * Revision 1.1  2001/09/15 14:52:43  bretthauert
 * initial release
 *
 */

/** \file cooking.h
 *  declarations and prototypes for interpreting DMI-BIOS and SM-BIOS types
 *
 *  \author Thomas Bretthauer
 *  \author Joachim Braeuer
 *  \version 0.1
 *  \date January 2001
 */
     
#ifndef __COOKING_H__
#define __COOKING_H__

/*
 * function prototypes
 */

/* description: see implementation file */
unsigned char * bios_cook (smbios_struct * smbiostype, unsigned int *length);

unsigned char * bios_cook_type_0 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_1 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_2 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_3 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_4 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_5 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_6 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_7 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_8 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_9 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_10 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_11 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_12 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_13 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_16 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_17 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_19 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_20 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_32 (smbios_struct * smbiostype, unsigned int *length);
unsigned char * bios_cook_type_127 (smbios_struct * smbiostype, unsigned int *length);



/**
 * cooked structures
 */

typedef struct smbios_header
{
    __u8    type                           __attribute__ ((packed));
    __u8    length                         __attribute__ ((packed));
    __u16   handle                         __attribute__ ((packed));
} smbios_header;

/* type 0 - bios information */
typedef struct smbios_type_0
{
	smbios_header	header;
    __u8	vendor							__attribute__ ((packed));
	__u8	version							__attribute__ ((packed));
	__u16	startaddr						__attribute__ ((packed));
	__u8	reldate							__attribute__ ((packed));
	__u8	romsize							__attribute__ ((packed));
	__u64	characteristics					__attribute__ ((packed));
    __u8	ext1							__attribute__ ((packed));
    __u8	ext2							__attribute__ ((packed));
} smbios_type_0;

/* type 1 - system information */
typedef struct smbios_type_1
{
	smbios_header	header;
    __u8	manufacturer					__attribute__ ((packed));
	__u8	productname						__attribute__ ((packed));
	__u8	version							__attribute__ ((packed));
	__u8	serialnumber					__attribute__ ((packed));
	__u8	uuid[16]						__attribute__ ((packed));
	__u8	wakeuptype						__attribute__ ((packed));
} smbios_type_1;

/* type 2 - base board information */
typedef struct smbios_type_2
{
	smbios_header	header;
    __u8	manufacturer					__attribute__ ((packed));
	__u8	product							__attribute__ ((packed));
	__u8	version							__attribute__ ((packed));
	__u8	serialnumber					__attribute__ ((packed));
} smbios_type_2;

/* type 3 - system enclosure information */
typedef struct smbios_type_3
{
	smbios_header	header;
    __u8	manufacturer					__attribute__ ((packed));
	__u8	type							__attribute__ ((packed));
	__u8	version							__attribute__ ((packed));
	__u8	serialnumber					__attribute__ ((packed));
	__u8	asset_tag						__attribute__ ((packed));
	__u8	bootup_state					__attribute__ ((packed));
	__u8	pwr_supply_state				__attribute__ ((packed));
	__u8	thermal_state					__attribute__ ((packed));
	__u8	security_state					__attribute__ ((packed));
	__u32	oem_defined						__attribute__ ((packed));
} smbios_type_3;

/* type 4 - processor information */
typedef struct smbios_type_4
{
	smbios_header	header;
    __u8	socket_designation				__attribute__ ((packed));
	__u8	type							__attribute__ ((packed));
	__u8	family							__attribute__ ((packed));
	__u8	manufacturer					__attribute__ ((packed));
	__u8	id[8]							__attribute__ ((packed));
	__u8	version							__attribute__ ((packed));
	__u8	voltage							__attribute__ ((packed));
	__u16	external_clock					__attribute__ ((packed));
	__u16	max_speed						__attribute__ ((packed));
	__u16	current_speed					__attribute__ ((packed));
	__u8	status							__attribute__ ((packed));
	__u8	upgrade							__attribute__ ((packed));
	__u16	l1_cache_handle					__attribute__ ((packed));
    __u16	l2_cache_handle					__attribute__ ((packed));
    __u16	l3_cache_handle					__attribute__ ((packed));
    __u8	serialnumber					__attribute__ ((packed));
    __u8	asset_tag						__attribute__ ((packed));
    __u8	part_number						__attribute__ ((packed));
} smbios_type_4;

/* type 5 - memory controller information - nowadays obsolete */
typedef struct smbios_type_5
{
	smbios_header	header;

    __u8	error_detection					__attribute__ ((packed));
	__u8	ecc								__attribute__ ((packed));
	__u8	supported_interleave			__attribute__ ((packed));
	__u8	current_interleave				__attribute__ ((packed));
	__u8	max_mem_module_size				__attribute__ ((packed));
	__u16	supported_speeds				__attribute__ ((packed));
	__u16	supported_types					__attribute__ ((packed));
	__u8	voltage							__attribute__ ((packed));
	__u8    nr_assoc_slots					__attribute__ ((packed));
} smbios_type_5;

/* type 6 - memory module information - nowadays obsolete */
typedef struct smbios_type_6
{
	smbios_header	header;

    __u8	designation						__attribute__ ((packed));
	__u8	connection						__attribute__ ((packed));
	__u8	current_speed					__attribute__ ((packed));
	__u16	current_type					__attribute__ ((packed));
	__u8	installed_size					__attribute__ ((packed));
	__u8	enabled_size					__attribute__ ((packed));
	__u8	status							__attribute__ ((packed));
} smbios_type_6;

/* type 7 - cache information */
typedef struct smbios_type_7
{
	smbios_header	header;

    __u8	designation						__attribute__ ((packed));
	__u16	configuration					__attribute__ ((packed));
	__u16	max_size	    				__attribute__ ((packed));
	__u16	installed_size					__attribute__ ((packed));
	__u16	supported_sram					__attribute__ ((packed));
	__u16	current_sram					__attribute__ ((packed));
	__u8	speed							__attribute__ ((packed));
	__u8	ecc_type		        		__attribute__ ((packed));
	__u8	cache_type						__attribute__ ((packed));
	__u8	associativity					__attribute__ ((packed));
} smbios_type_7;

/* type 8 - port connector information */
typedef struct smbios_type_8
{
	smbios_header	header;

    __u8	internal_designation			__attribute__ ((packed));
	__u8	internal_type					__attribute__ ((packed));
	__u8	external_designation	    	__attribute__ ((packed));
	__u8	external_type					__attribute__ ((packed));
	__u8	type					        __attribute__ ((packed));
} smbios_type_8;

/* type 9 - system slot information */
typedef struct smbios_type_9
{
	smbios_header	header;

    __u8	designation			            __attribute__ ((packed));
	__u8	type					        __attribute__ ((packed));
	__u8	width	    	                __attribute__ ((packed));
	__u8	usage					        __attribute__ ((packed));
	__u8	length					        __attribute__ ((packed));
	__u8	id1					            __attribute__ ((packed));
	__u8	id2					            __attribute__ ((packed));
	__u8	characteristics1				__attribute__ ((packed));
	__u8	characteristics2				__attribute__ ((packed));
} smbios_type_9;

/* type 10 - onboard devices */
/* due to the nature of the structure it is better to discard the
 * header for this structure
 */
typedef struct smbios_type_10
{
	/* do not use the header for this structure */
	/* smbios_header	header; */

	__u8	type					        __attribute__ ((packed));
	__u8	description    	                __attribute__ ((packed));
} smbios_type_10;

/* type 11 - oem strings */
typedef struct smbios_type_11
{
	smbios_header	header;

    __u8	nr_of_strings		            __attribute__ ((packed));
} smbios_type_11;

/* type 12 - system configuration options */
typedef struct smbios_type_12
{
	smbios_header	header;

    __u8	nr_of_strings		            __attribute__ ((packed));
} smbios_type_12;

/* type 13 - bios language information */
typedef struct smbios_type_13
{
	smbios_header	header;

    __u8	nr_of_strings   	            __attribute__ ((packed));
    __u8    flags                           __attribute__ ((packed));
    __u8    reserved[15]                    __attribute__ ((packed));
    __u8    current_language                __attribute__ ((packed));
} smbios_type_13;

/* type 16 - physical memory array */
typedef struct smbios_type_16
{
	smbios_header	header;

    __u8	location   	                    __attribute__ ((packed));
    __u8    use                             __attribute__ ((packed));
    __u8    ecc                             __attribute__ ((packed));
    __u32   max_capacity                    __attribute__ ((packed));
    __u16   error_handle                    __attribute__ ((packed));
    __u16   nr_of_devices                   __attribute__ ((packed));
} smbios_type_16;

/* type 17 - memory device */
typedef struct smbios_type_17
{
	smbios_header	header;

    __u16	memory_array_handle             __attribute__ ((packed));
    __u16   error_handle                    __attribute__ ((packed));
    __u16   total_width                     __attribute__ ((packed));
    __u16   data_width                      __attribute__ ((packed));
    __u16   size                            __attribute__ ((packed));
    __u8    form_factor                     __attribute__ ((packed));
    __u8    device_set                      __attribute__ ((packed));
    __u8    device_locator                  __attribute__ ((packed));
    __u8    bank_locator                    __attribute__ ((packed));
    __u8    mem_type                        __attribute__ ((packed));
    __u16   type_detail                     __attribute__ ((packed));
    __u16   speed                           __attribute__ ((packed));
    __u8    manufacturer                    __attribute__ ((packed));
    __u8    serial_number                   __attribute__ ((packed));
    __u8    asset_tag                       __attribute__ ((packed));
    __u8    part_number                     __attribute__ ((packed));
} smbios_type_17;

typedef struct smbios_type_19
{
	smbios_header	header;

    __u32	starting_adr                    __attribute__ ((packed));
    __u32   ending_adr                      __attribute__ ((packed));
    __u16   mem_array_handle                __attribute__ ((packed));
    __u8    partition_width                 __attribute__ ((packed));
} smbios_type_19;

typedef struct smbios_type_20
{
	smbios_header	header;

    __u32	starting_adr                    __attribute__ ((packed));
    __u32   ending_adr                      __attribute__ ((packed));
    __u16   mem_dev_handle                  __attribute__ ((packed));
    __u16   mem_array_mapped_adr_handle     __attribute__ ((packed));
    __u8    partition_row_position          __attribute__ ((packed));
    __u8    interleave_position             __attribute__ ((packed));
    __u8    interleaved_data_depth          __attribute__ ((packed));
} smbios_type_20;

typedef struct smbios_type_32
{
	smbios_header	header;

    __u8	reserved[6]                     __attribute__ ((packed));
    __u8    status                          __attribute__ ((packed));
} smbios_type_32;



/**
 * helper functions
 */

char * GetString(smbios_struct *structure, unsigned int stringnr);
unsigned int pow2(unsigned int);

#endif /* __COOKING_H__ */


