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

/* $Id: bios.h,v 1.2 2002/09/03 10:33:12 bretthauert Exp $
 *
 * $Log: bios.h,v $
 * Revision 1.2  2002/09/03 10:33:12  bretthauert
 * fixed a bug with 2.4 kernels (changed kmalloc parameter from
 * GFP_BUFFER to GFP_KERNEL
 *
 * Revision 1.1  2001/09/15 14:52:43  bretthauert
 * initial release
 *
 */

/** \file bios.h
 *  declarations and prototypes of DMI-BIOS and SM-BIOS stuff
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \author Joachim Braeuer
 *  \version 0.11
 *  \date January 2001
 */
     
#ifndef __BIOS_H__
#define __BIOS_H__

/*
 *   Macros to help debugging
 */
/** to help debugging ... */
#undef PDEBUG /* undef it, just in case */
#ifdef _DEBUG_
#  define PDEBUG(fmt, args...) printk( KERN_DEBUG "smbios: " fmt, ## args)
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

/*
 *   Magic numbers
 */

/** start address of BIOS segment to scanned for SM-BIOS and DMI-BIOS */
#define BIOS_START_ADDRESS      0xF0000
/** length of the scanned BIOS area for SM-BIOS and DMI-BIOS */
#define BIOS_MAP_LENGTH         0x10000
/** magic 4 bytes to identify SM-BIOS entry point, paragraph boundary */
#define SMBIOS_MAGIC_DWORD      0x5F4D535F /* anchor string "_SM_" */
/** magic 4 bytes to identify DMI-BIOS entry point, byte boundary */
#define DMIBIOS_MAGIC_DWORD     0x494d445f /* anchor string "_DMI" */
/** identifier for SM-BIOS structures within SM-BIOS entry point */
#define DMI_STRING              "_DMI_"
/** list of types which are known to have subtyes; expandable! */
#define TYPES_WITH_SUBTYPES     185, 187, 208, 209, 210, 211, 212, 254
/** maximum block size for proc read function */
#define PROC_BLOCK_SIZE         (3*1024)


/** mode raw/cooked */
#define FILE_MODE_RAW       0
#define FILE_MODE_COOKED    1




/*
 *   Structures
 */

/** SM-BIOS entry point structure 
 * the SMBIOS Entry Point structure described below can be located by
 * application software by searching for the anchor string on paragraph
 * (16 byte) boundaries within the physical memory address range 000F0000h to
 * 000FFFFFh. This entry point encapsulates an intermediate anchor string
 * that is used by some existing DMI browsers.
 *
 * @note While the SMBIOS Major and Minor Versions (offsets 06h and 07h)
 * currently duplicate the information present in the SMBIOS BCD Revision
 * (offset 1Dh), they provide a path for future growth in this specification.
 * The BCD Revision, for example, provides only a single digit for each of
 * the major and minor version numbers.
 */
typedef struct smbios_entry_point_struct
{
	/** "_SM_", specified as four ASCII characters (5F 53 4D 5F) */
  __u32 anchor_string                  __attribute__ ((packed));
	/** checksum of the Entry Point Structure (EPS). This value, when added to 
	 * all other bytes in the EPS, will result in the value 00h (using 8 bit
	 * addition calculations). Values in the EPS are summed starting at offset
	 * 00h, for Entry Point Length bytes.*/
  __u8  entry_point_checksum           __attribute__ ((packed));
	/** Length of the Entry Point Structure, starting with the Anchor String 
	 * field, in bytes, currently 1Fh. */
  __u8  entry_point_length             __attribute__ ((packed));
	/** identifies the major version of this specification implemented in
	 * the table structures, e.g. the value will be 0Ah for revision 10.22
	 * and 02h for revision 2.1 */
  __u8  major_version                  __attribute__ ((packed));
	/** identifies the minor version of this specification implemented in
	 * the table structures, e.g. the value will be 16h for revision 10.22
	 * and 01h for revision 2.1 */
  __u8  minor_version                  __attribute__ ((packed));
	/** size of the largest SMBIOS structure, in bytes, and encompasses the
	 * structure's formatted area and text strings. This is the value returned
	 * as StructureSize from the Plug-n-Play 'Get SMBIOS Information' function */
	__u16 max_struct_size                __attribute__ ((packed));
	/** identifies the EPS revision implemented in this structure and identifies
	 * the formatting of offsets 0Bh to 0Fh, one of:
	 * 00h     Entry Point based on SMBIOS 2.1 definition; formatted area is
	 *         reserved and set to all 00h.
	 * 01h-FFh reserved for assignment via this specification */
  __u8  revision                       __attribute__ ((packed));
	/** the value present in the Entry Point Revision field defines the
	 * interpretation to be placed upon these5 bytes. */
  __u8  formated_area[5]               __attribute__ ((packed));
	/** "_DMI_", specified as five ASCII characters (5F 44 4D 49 5F) */
  __u8  intermediate_string[5]         __attribute__ ((packed));
	/** checksum of the Intermediate Entry Point Structure (IEPS). This value,
	 * when added to all other bytes in the IEPS, will result in the value 00h
	 * (using 8 bit addition calculations). Values in the IEPS are summed
	 * starting at offset 10h, for 0Fh bytes */
  __u8  intermediate_checksum          __attribute__ ((packed));
	/** the 32 bit physical starting address of the read-only SMBIOS Structure
	 * Table, that can start at any 32 bit address. This area contains all of the
	 * SMBIOS structures fully packed together. These structures can then be
	 * parsed to produce exactly the same format as that returned from a 'Get
	 * SMBIOS Structure' function call. */
  __u16 struct_table_length            __attribute__ ((packed));
  __u32 struct_table_address           __attribute__ ((packed));
  __u16 no_of_structures               __attribute__ ((packed));
  __u8  bcd_revision                   __attribute__ ((packed));
} smbios_entry_point_struct;

/** SM-BIOS and DMI-BIOS structure header */
typedef struct smbios_struct
{
  __u8  type                           __attribute__ ((packed));
  __u8  length                         __attribute__ ((packed));
  __u16 handle                         __attribute__ ((packed));
  __u8  subtype                        __attribute__ ((packed));
        /* ... other fields are structure dependend ... */
} smbios_struct;

/** DMI-BIOS structure header */
typedef struct dmibios_table_entry_struct
{
  __u16 size                           __attribute__ ((packed));
  __u16 handle                         __attribute__ ((packed));
  __u32 procedure                      __attribute__ ((packed));
} dmibios_table_entry_struct;

/** DMI-BIOS entry point structure */
typedef struct dmibios_entry_point_struct
{
  __u8  signature[10]                  __attribute__ ((packed));
  __u8  revision                       __attribute__ ((packed));
  dmibios_table_entry_struct entry[1]  __attribute__ ((packed));
} dmibios_entry_point_struct;


/*
 *   Variables
 */
extern struct proc_dir_entry * smbios_proc_dir;             /* /proc/smbios */
extern struct proc_dir_entry * smbios_raw_proc_dir;         /* /proc/smbios/raw */
extern struct proc_dir_entry * smbios_cooked_proc_dir;      /* /proc/smbios/cooked */
extern void * smbios_base;                                  /* F-Segment */
extern smbios_entry_point_struct * smbios_entry_point;      /* start of SMBIOS within the F-Segment */
extern dmibios_entry_point_struct * dmibios_entry_point;    /* start of DMIBIOS within the F-Segment */
extern void * smbios_structures_base;                       /* base of SMBIOS raw structures */
extern unsigned char smbios_types_with_subtypes[];
extern char smbios_version_string[32];                      /* e.g. V2.31 */

/*
 *   Functions
 */

/* for the description see the implementation file */
smbios_entry_point_struct * smbios_find_entry_point(void * base);
dmibios_entry_point_struct * dmibios_find_entry_point(void * base);
unsigned char smbios_check_entry_point(void * addr);

int smbios_type_has_subtype(unsigned char type);

int smbios_get_struct_length(smbios_struct * struct_ptr);
int dmibios_get_struct_length(smbios_struct * struct_ptr);

int smbios_version_proc (char *page, char **start, off_t off, int count, int *eof, void *data);
int bios_read_raw_proc(char * page, char ** start, off_t off, int count, int * eof, void * data);
int bios_read_cooked_proc(char *page, char **start, off_t off, int count, int *eof, void *data);
int dmibios_read_raw_proc(char * page, char ** start, off_t off, int count, int * eof, void * data);
int dmibios_read_cooked_proc(char *page, char **start, off_t off, int count, int *eof, void *data);

int smbios_make_dir_entries(struct proc_dir_entry *smbiosdir, struct proc_dir_entry *rawdir, struct proc_dir_entry *cookeddir);
int smbios_make_version_entry(struct proc_dir_entry *smbiosdir);
int dmibios_make_dir_entries(struct proc_dir_entry * smbiosdir, struct proc_dir_entry * rawdir, struct proc_dir_entry * cookeddir);

void smbios_destroy_dir_entries(struct proc_dir_entry * dir);

unsigned int smbios_get_readable_name_ext(char *readable_name, smbios_struct *struct_ptr);
unsigned int smbios_get_readable_name(char *readable_name, smbios_struct *struct_ptr);
int make_file_entries (char *filename, struct proc_dir_entry *dir, smbios_struct *struct_ptr, int mode);

#endif /* __BIOS_H__ */
