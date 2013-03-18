/* Copyright (C) 2000-2001 Fujitsu Siemens Computers
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

/* $Id: main.c,v 1.2 2002/09/03 10:33:12 bretthauert Exp $
 *
 * $Log: main.c,v $
 * Revision 1.2  2002/09/03 10:33:12  bretthauert
 * fixed a bug with 2.4 kernels (changed kmalloc parameter from
 * GFP_BUFFER to GFP_KERNEL
 *
 * Revision 1.1  2001/09/15 14:52:43  bretthauert
 * initial release
 *
 */

/** \file main.c
 *  kernel interface functions for smbios kernel module
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 *  \version 0.1
 */
#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#define __NO_VERSION__		/* don't define kernel_verion in module.h */
#include <linux/module.h>
#include <linux/version.h>

char kernel_version[] = UTS_RELEASE;

#include <linux/kernel.h>	/* printk() */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <asm/io.h>		    /* ioremap() */

#include "strgdef.h"        /* holds all the interpreted/cooked string definitions */
#include "bios.h"		    /* local definitions */

EXPORT_NO_SYMBOLS;

/*
 *   Module stuff
 */

/** \fn int init_module (void)
 *  \brief module initialization
 *  \return 0 if successful initialization, an error code otherwise
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \author Joachim Braeuer
 *  \date Januar 2001
 */
int
init_module (void)
{
    int err = 0;


    PDEBUG ("starting module initialization\n");

    /*
     *  map the SMBIOS memory segment
     *
     *  ioremap (kernel) maps a physical address to a virtual address.
     *  bios start address is e.g. the Bios F-Segment
     */

    if (!(smbios_base = ioremap (BIOS_START_ADDRESS, BIOS_MAP_LENGTH)))
    {
        PDEBUG ("ioremap() for entry point failed\n");
        err = -ENXIO;
        goto ioremap_for_entry_point_failed;
    }

    PDEBUG ("BIOS base set to 0x%p\n", smbios_base);




    /*
     *   search for SM-BIOS or DMI-BIOS entry point
	 *		
	 * 	smbios_base holds the virtual starting address of the F-Segment.
	 *  first check for the smbios entry point. only if we can't find it
	 *	we look for the proprietary dmi bios entry point. This is a entry
	 *  point defined by Siemens Nixdorf before the 'real' interface has
	 *  been introduced.
     */

	/* smbios/dmibios_entry_point points to the beginning of the entry point
     * table. However, they are of different structure (real - proprietary)
     */
    if (!(smbios_entry_point = smbios_find_entry_point (smbios_base)))
    {
        PDEBUG ("SM-BIOS entry point not found\n");

        if (!(dmibios_entry_point = dmibios_find_entry_point (smbios_base)))
        {
	        PDEBUG ("DMI-BIOS entry point not found. Aborting...\n");
	        err = -ENXIO;
	        goto find_entry_point_failed;
        }
    }

    /*
     *  for SM-BIOS:
     *  check if Pointer to DMI structures exist.
     *  intermediate_string (_DMI_) is not '\0' terminated,
     *  so strncmp() with sizeof(DMI_STRING) - 1 is needed.
     */
    if (smbios_entry_point)
    {
        if (strncmp((char *) &(smbios_entry_point->intermediate_string),
						DMI_STRING, sizeof (DMI_STRING) - 1))
		{
	        PDEBUG ("Pointer to DMI structures not found!\n");
	        err = -ENXIO;
	        goto check_dmi_failed;
        }
    }

    /*
     *  map the SM-BIOS structures physical address range.
	 *  the 'real' smbios_structures_base contains the starting
	 *  address, where the instances of dmi structures are located.
     */
    if (smbios_entry_point)
    {
        if (!(smbios_structures_base =
	          ioremap (smbios_entry_point->struct_table_address,
		        (unsigned long) smbios_entry_point->struct_table_length)))
		{
	        PDEBUG ("ioremap() for structures table failed\n");
	        err = -ENXIO;
	        goto ioremap_for_structures_table_failed;
        }
    }

    /*
     * On the other hand, if we have the proprietary DMI Bios, smbios_structures_base
	 * contains a pointer to a table. This table contains an offset to the sm/dmi bios
     * structures for each single instance.
     */
    if (dmibios_entry_point)
    {
        if (!(smbios_structures_base = dmibios_entry_point->entry))
        {
	        PDEBUG ("invalid structure table entry%p,%p\n", smbios_structures_base,
		        dmibios_entry_point->entry);
	        err = -ENXIO;
	        goto ioremap_for_structures_table_failed;
        }
    }
    PDEBUG ("DMI structures base set to 0x%p\n", smbios_structures_base);



	/*
	 * assumption: we have one single pointer that points to the beginning of the
     * smbios/dmibios structures regardless what kind of bios we have.
     * (except DMIBios 2.0 - the interface we use was introduced with DMIBios 2.1)
     */


    /*
     *  create /proc entries
     */

    /* make /proc/smbios directory */
    if (!(smbios_proc_dir =
	       create_proc_entry (PROC_DIR_STRING, S_IFDIR, &proc_root)))
	{
        err = -ENOMEM;
        PDEBUG ("failed to create /proc/smbios directory entry\n");
        goto create_smbios_dir_failed;
    }
    PDEBUG ("/proc/smbios directory created.\n");
   
    /* make /proc/smbios/raw directory */
    if (!(smbios_raw_proc_dir =
				 create_proc_entry (PROC_DIR_STRING_RAW, S_IFDIR, smbios_proc_dir)))
	{
        err = -ENOMEM;
        PDEBUG ("failed to create /proc/smbios/raw directory entry\n");
        goto create_smbios_raw_dir_failed;
    }
    PDEBUG ("/proc/smbios/raw directory created.\n");

    /* make /proc/smbios/cooked directory */
    if (!(smbios_cooked_proc_dir =
				 create_proc_entry (PROC_DIR_STRING_COOKED, S_IFDIR, smbios_proc_dir)))
    {
        err = -ENOMEM;
		PDEBUG ("failed to create /proc/smbios/cooked directory entry\n");
		goto create_smbios_cooked_dir_failed;
    }
    PDEBUG ("/proc/smbios/cooked directory created.\n");

    /* create version file */
    if (smbios_entry_point)
    {
        if ((err = smbios_make_version_entry (smbios_proc_dir)))
	        goto smbios_make_version_entry_failed;
    }


   /* make raw files for SM-BIOS */
   if (smbios_entry_point)
   {
        if ((err = smbios_make_dir_entries (smbios_proc_dir, smbios_raw_proc_dir, smbios_cooked_proc_dir)))
	        goto make_smbios_dir_entries_failed;
   }
	
   /* make files for DMI-BIOS */
   if (dmibios_entry_point)
   {
        if ((err = dmibios_make_dir_entries (smbios_proc_dir, smbios_raw_proc_dir, smbios_cooked_proc_dir)))
	        goto make_smbios_dir_entries_failed;
   }

	

   PDEBUG ("module loaded succesfully\n");
   return 0;


/*
 * reached only if an error occured...
 *
 * whenever we need/create system resources and we run into an error, we clean up
 * in the oposite direction. Sample:
 *
 * allocate resource 1, 2, 3, 4, ...
 * error occurs
 * clean up resources ..., 4, 3, 2, 1
 *
 * the code above jumps into the correct position of the cleanup chain.
 */

make_smbios_dir_entries_failed:
    /* remove /proc/smbios/cooked files */
    smbios_destroy_dir_entries (smbios_cooked_proc_dir);
    /* remove /proc/smbios/raw files */
    smbios_destroy_dir_entries (smbios_raw_proc_dir);
    /* remove /proc/smbios files */
    smbios_destroy_dir_entries (smbios_proc_dir);
	
smbios_make_version_entry_failed:

    /* remove /proc/smbios/cooked directory */
    remove_proc_entry(PROC_DIR_STRING_COOKED, smbios_proc_dir);
	
create_smbios_cooked_dir_failed	:
    /* remove /proc/smbios/raw directory */
    remove_proc_entry(PROC_DIR_STRING_RAW, smbios_proc_dir);

create_smbios_raw_dir_failed:
    /* remove /proc/smbios directory */
    remove_proc_entry(PROC_DIR_STRING, &proc_root);
	
create_smbios_dir_failed:
    /* unmap the virtual to physical memory binding */
    if (smbios_entry_point)
        iounmap (smbios_structures_base);

ioremap_for_structures_table_failed:
check_dmi_failed:
find_entry_point_failed:
    /* unmap the virtual to physical memory binding */
    iounmap (smbios_base);

ioremap_for_entry_point_failed:
    return err;
}


/** \fn int cleanup_module (void)
 *  \brief module cleanup
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 */
void
cleanup_module (void)
{
    /* remove /proc/smbios/cooked files */
    smbios_destroy_dir_entries (smbios_cooked_proc_dir);
    /* remove /proc/smbios/raw files */
    smbios_destroy_dir_entries (smbios_raw_proc_dir);
    /* remove /proc/smbios files */
    smbios_destroy_dir_entries (smbios_proc_dir);
	
    /* remove /proc/smbios/cooked directory */
    remove_proc_entry(PROC_DIR_STRING_COOKED, smbios_proc_dir);
    /* remove /proc/smbios/raw directory */
    remove_proc_entry(PROC_DIR_STRING_RAW, smbios_proc_dir);
    /* remove /proc/smbios directory */
    remove_proc_entry(PROC_DIR_STRING, &proc_root);
	
    /* unmap the virtual to physical memory binding */
    if (smbios_entry_point)
        iounmap (smbios_structures_base);

    /* unmap the virtual to physical memory binding */
    iounmap (smbios_base);

    PDEBUG ("module unloaded\n");
}
