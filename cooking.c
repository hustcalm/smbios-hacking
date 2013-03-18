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

/* $Id: cooking.c,v 1.2 2002/09/03 10:33:12 bretthauert Exp $
 *
 * $Log: cooking.c,v $
 * Revision 1.2  2002/09/03 10:33:12  bretthauert
 * fixed a bug with 2.4 kernels (changed kmalloc parameter from
 * GFP_BUFFER to GFP_KERNEL
 *
 * Revision 1.1  2001/09/15 14:52:43  bretthauert
 * initial release
 *
 */

/** \file cooking.c
 *  DMI-BIOS and SM-BIOS interpreting (cooking) functions
 *  The functions defined in this file generate a human readable output of the
 *  raw binary DMI- and SM-BIOS types. However, every time the specification of
 *  DMI- and SM-BIOS (types) change, these changes have to be adopted in these
 *  routines, too.
 *  
 *  \author Thomas Bretthauer
 *  \author Joachim Braeuer
 *  \version 0.1
 *  \date January 2001
 */

#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#define __NO_VERSION__		/* don't define kernel_version in module.h */
#include <linux/module.h>

#include <linux/kernel.h>	/* ... for 'printk()' */
#include <linux/errno.h>	/* ... error codes */
#include <linux/types.h>	/* ... fixed size types definitions, '__u8'... */
#include <linux/proc_fs.h>  /* ... /proc file system ... */
#include <linux/string.h>	/* ... for 'memcpy()', 'strncmp()' */


#include "strgdef.h"        /* human readable output string definitions for directories,
                             * files and file contents
                             */
#include "bios.h"		    /* ... local declarations for DMI-, SM-BIOS */
#include "cooking.h"	    /* ... local declarations for interpreting DMI- and SM-BIOS types */

EXPORT_NO_SYMBOLS;




/** \fn unsigned char * bios_cook (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief huge switch that calls the cooking functions for every SMBIOS structure
  * \param smbiosstruct pointer to SMBIOS raw structure
  * \param plength amount of memory allocated by functions called from this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS structure. it calls the appropriate cooking function
  * whitch interpretes the raw data and builds a string with the interpreted data.
  * the caller is responsible to free the memory allocated by the "sub"-functions.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook (smbios_struct *smbiosstruct, unsigned int * plength)
{
	unsigned char *scratch = NULL;      /* pointer that holds the interpreded data */


	/* do we have a valid SMBIOS? */
	if ( !smbiosstruct )
	   return NULL;

	/* switch between the different smbios types to interpret and call the appropriate
	 * cook function.
	 */
	switch (smbiosstruct->type)
	{
        case 0:
	        scratch = bios_cook_type_0 (smbiosstruct, plength);
	        break;
	    case 1:
	        scratch = bios_cook_type_1 (smbiosstruct, plength);
	        break;
	    case 2:
	        scratch = bios_cook_type_2 (smbiosstruct, plength);
	        break;
	    case 3:
	        scratch = bios_cook_type_3 (smbiosstruct, plength);
	        break;
	    case 4:
	        scratch = bios_cook_type_4 (smbiosstruct, plength);
	        break;
	    case 5:
	        scratch = bios_cook_type_5 (smbiosstruct, plength);
	        break;
	    case 6:
	        scratch = bios_cook_type_6 (smbiosstruct, plength);
	        break;
	    case 7:
	        scratch = bios_cook_type_7 (smbiosstruct, plength);
	        break;
	    case 8:	
	        scratch = bios_cook_type_8 (smbiosstruct, plength);	
	        break;	
	    case 9:	
	        scratch = bios_cook_type_9 (smbiosstruct, plength);	
	        break;
	    case 10:	
	        scratch = bios_cook_type_10 (smbiosstruct, plength);	
	        break;	
	    case 11:	
	        scratch = bios_cook_type_11 (smbiosstruct, plength);	
	        break;
	    case 12:	
	        scratch = bios_cook_type_12 (smbiosstruct, plength);	
	        break;    	
	    case 13:	
	        scratch = bios_cook_type_13 (smbiosstruct, plength);	
	        break;
	    case 16:	
	        scratch = bios_cook_type_16 (smbiosstruct, plength);	
	        break;
	    case 17:	
	        scratch = bios_cook_type_17 (smbiosstruct, plength);	
	        break;
	    case 19:	
	        scratch = bios_cook_type_19 (smbiosstruct, plength);	
	        break;
	    case 20:	
	        scratch = bios_cook_type_20 (smbiosstruct, plength);	
	        break;
	    case 32:	
	        scratch = bios_cook_type_32 (smbiosstruct, plength);	
	        break;
	    case 127:	
	        scratch = bios_cook_type_127 (smbiosstruct, plength);	
	        break;        	
	

		/* if it's none of the supported types, just display the standard
		 * DMI header */
	    default:
	        {
                unsigned char line[512];
                unsigned char line_type[128];     /* type */
                unsigned char line_length[128];   /* length */
                unsigned char line_handle[128];   /* handle */
                unsigned char line_text[128];     /* not supported message */

                /* prepare type, length, handle and not supported message */
                sprintf (line_type, "%20s : %d\n", TYPE, smbiosstruct->type);
                sprintf (line_length, "%20s : %d %s\n", LENGTH, smbiosstruct->length, BYTES);
                sprintf (line_handle, "%20s : %d\n", HANDLE, smbiosstruct->handle);
	            sprintf (line_text, "\n%s\n\n", NOT_SUPPORTED);

                sprintf (line, "%s%s%s%s", line_type, line_length, line_handle, line_text);
                *plength = strlen (line);

                /* allocate memory ... */
                scratch = kmalloc (*plength+1, GFP_KERNEL);
                if (scratch == NULL)
                {
                    *plength = 0;
                    return NULL;
                }
	
                /* ... and copy the string */
                memcpy (scratch, line, *plength);
            }
	} /* end switch */
	
	/* return a string with all the interpreted data for the given raw structure */
	/* the caller is responsible to free the memory. */
	return scratch;
}


/** \fn unsigned char * bios_cook_type_0 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 0 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 0 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 0 (Bios) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_0 (smbios_struct * smbiosstruct, unsigned int * plength)
{
	smbios_type_0 *type0;
    unsigned char * scratch;

	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];


    /* cast raw data to type 0 structure */
    type0 = (smbios_type_0 *)smbiosstruct;

    /* prepare header string */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE0_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

	/* prepare strings with type 0 information */
	sprintf (line, "%-35s%s %s\n", TYPE0_VENDOR, SEP1, GetString(smbiosstruct, (unsigned int)(type0->vendor)));
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE0_VERSION, SEP1, GetString(smbiosstruct, (unsigned int)(type0->version)));
	strcat(file, line);
	sprintf (line, "%-35s%s %X\n", TYPE0_ADR_SEG, SEP1, type0->startaddr);
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE0_REL_DATE, SEP1, GetString(smbiosstruct, (unsigned int)(type0->reldate)));
	strcat(file, line);
	sprintf (line, "%-35s%s %d kB\n", TYPE0_ROM_SIZE, SEP1, ((type0->romsize)+1) * 64);
	strcat(file, line);
	

    /* bios characteristics interpretation */
    sprintf(line, "%-35s%s\n", TYPE0_CHAR, SEP1);
    strcat(file, line);
    if( (type0->characteristics) & 0x0000000000000001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_RES);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_RES);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_UNKNOWN);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_NOTSUP);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_ISA);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_MCA);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_EISA);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000080 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PCI);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000100 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PCMCIA);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000200 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PNP);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000400 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_APM);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000000800 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_FLASH);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000001000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_SHADOWING);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000002000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_VL);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000004000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_ESCD);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000008000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_BOOTCD);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000010000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_SELBOOT);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000020000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_BIOS_IS_SOCKETED);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000040000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PCMCIA_BOOT);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000080000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_ENH_DISK_DRIVE);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000100000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_FD_NEC);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000200000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_FD_TOSHIBA);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000400000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_360);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000000800000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_1200);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000001000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_720);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000002000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_2880);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000004000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PRINT_SCREEN);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000008000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_KEYBOARD);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000010000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_SER_SERVICES);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000020000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PRINT_SERVICES);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000040000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_VIDEO_SERVICES);
        strcat(file, line);
    }
    if( (type0->characteristics) & 0x0000000080000000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_CHAR_PC98);
        strcat(file, line);
    }

    if(smbiosstruct->length > 0x12)
    {
        /* interpretation of extension byte 1 */
        if( (type0->ext1) & 0x01 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_ACPI);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x02 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_USB);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x04 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_AGP);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x08 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_I2O_BOOT);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x10 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_LS120);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x20 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_ATAPI_ZIP_BOOT);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x40 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_1394_BOOT);
            strcat(file, line);
        }
        if( (type0->ext1) & 0x80 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT1_SMART_BATTERY);
            strcat(file, line);
        }
    }

    if(smbiosstruct->length > 0x13)
    {
        /* interpretation of extension byte 2 */
        if( (type0->ext2) & 0x01 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT2_BBS);
            strcat(file, line);
        }
        if( (type0->ext2) & 0x02 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE0_EXT2_NETWORK_BOOT);
            strcat(file, line);
        }
    }

	
    /* now line contains a string with fully interpreted type 0 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */	
	/* the caller is responsible to free the memory. */	
	return scratch;
	
}


/** \fn unsigned char * bios_cook_type_1 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 1 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 1 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 1 (System) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_1 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */

	smbios_type_1 *type1;
    unsigned char * scratch;

	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];

    unsigned char help[8];                  /* helper string */
    int i;


    /* cast our data ptr to a structure ptr of SMBIOS type 1 */
    type1 = (smbios_type_1 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE1_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
	
	/* prepare interpreted type 1 string */
	sprintf (line, "%-35s%s %s\n", TYPE1_MANUFACTURER, SEP1, GetString(smbiosstruct, (unsigned int)(type1->manufacturer)));
    strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE1_PRODUCT_NAME, SEP1, GetString(smbiosstruct, (unsigned int)(type1->productname)));
	strcat(file, line);	
	sprintf (line, "%-35s%s %s\n", TYPE1_VERSION, SEP1, GetString(smbiosstruct, (unsigned int)(type1->version)));
	strcat(file, line);	
	sprintf (line, "%-35s%s %s\n", TYPE1_SERIAL_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type1->serialnumber)));
	strcat(file, line);

	
    /* if smbios 2.1 or higher */
    if(smbiosstruct->length > 0x08)		
	{
  	    sprintf(line, "%-35s%s ", TYPE1_UUID, SEP1);
  	    strcat(file, line);
  	    for (i=0; i<16; i++)
 		{
			sprintf(help, "%.2X ", type1->uuid[i]);
  	        strcat(file, help);
		}
		strcat(file, "\n");

		switch(type1->wakeuptype)
		{
			case 0:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_RESERVED);
						break;
			case 1:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_OTHER);
						break;
			case 2:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_UNKNOWN);
						break;
			case 3:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_APM);
						break;
			case 4:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_MODEM);
						break;
			case 5:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_LAN);
						break;
			case 6:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_POWER_SWITCH);
						break;
			case 7:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_PCI_PME);
						break;
			case 8:		sprintf (line, "%-35s%s %s\n", TYPE1_WAKEUP, SEP1, TYPE1_WT_AC_PWR_RESTORE);
						break;
			default:    sprintf (line, "%-35s%s %d\n", TYPE1_WAKEUP, SEP1, type1->wakeuptype);
		}
  	    strcat(file, line);
	} /* end - if smbios 2.1 or higher */

	/* now line contains a string with fully interpreted type 1 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */	
    /* the caller is responsible to free the memory. */	
    return scratch;

}


/** \fn unsigned char * bios_cook_type_2 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 2 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 2 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 2 (BaseBoard) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_2 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_2 *type2;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];


    /* cast our data ptr to a structure ptr of SMBIOS type 2 */
    type2 = (smbios_type_2 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE2_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

	/* prepare interpreted type 2 strings */
	sprintf (line, "%-35s%s %s\n", TYPE2_MANUFACTURER, SEP1, GetString(smbiosstruct, (unsigned int)(type2->manufacturer)));
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE2_PRODUCT, SEP1, GetString(smbiosstruct, (unsigned int)(type2->product)));
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE2_VERSION, SEP1, GetString(smbiosstruct, (unsigned int)(type2->version)));
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE2_SERIAL_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type2->serialnumber)));
	strcat(file, line);


    /* now line contains a string with fully interpreted type 2 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_3 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 3 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 3 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 3 (Enclosure, Chassis) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_3 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_3 *type3;
    unsigned char * scratch;
	
    /* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];


    /* cast our data ptr to a structure ptr of SMBIOS type 3 */
    type3 = (smbios_type_3 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE3_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

	sprintf (line, "%-35s%s %s\n", TYPE3_MANUFACTURER, SEP1, GetString(smbiosstruct, (unsigned int)(type3->manufacturer)));
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE3_VERSION, SEP1, GetString(smbiosstruct, (unsigned int)(type3->version)));
	strcat(file, line);
	sprintf (line, "%-35s%s %s\n", TYPE3_SERIAL_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type3->serialnumber)));
	strcat(file, line);
    sprintf (line, "%-35s%s %s\n", TYPE3_ASSET_TAG, SEP1, GetString(smbiosstruct, (unsigned int)(type3->asset_tag)));
    strcat(file, line);

 	
    if(type3->type &0x80)
        sprintf (line, "%-35s%s %s\n", TYPE3_CHASSIS_LOCK, SEP1, TYPE3_CL_PRESENT);
    else
        sprintf (line, "%-35s%s %s\n", TYPE3_CHASSIS_LOCK, SEP1, TYPE3_CL_NOT_PRESENT);
 	strcat(file, line);

    switch(type3->type & 0x7F)
	{
		case 1:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_OTHER);
					break;
		case 2:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_UNKNOWN);
					break;
		case 3:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_DESKTOP);
					break;
		case 4:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_LOW_PROFILE);
					break;
		case 5:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_PIZZA_BOX);
					break;
		case 6:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_MINI_TOWER);
					break;
		case 7:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_TOWER);
					break;
		case 8:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_PORTABLE);
					break;
		case 9:		sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_LAPTOP);
					break;
		case 10:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_NOTEBOOK);
					break;
		case 11:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_HANDHELD);
					break;
		case 12:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_DOCKING_STATION);
					break;
        case 13:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_ALL_IN_ONE);
					break;
		case 14:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_SUB_NOTEBOOK);
					break;
		case 15:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_SPACE_SAVING);
					break;
		case 16:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_LUNCH_BOX);
					break;
		case 17:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_MAIN_SERVER_CHASSIS);
					break;
		case 18:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_EXPANSION_CHASSIS);
					break;
		case 19:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_SUB_CHASSIS);
					break;
		case 20:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_BUS_EXP_CHASSIS);
					break;
		case 21:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_PERIPHERAL_CHASSIS);
					break;
		case 22:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_RAID_CHASSIS);
					break;
		case 23:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_RACK_MOUNT_CHASSIS);
					break;
		case 24:	sprintf (line, "%-35s%s %s\n", TYPE3_TYPE, SEP1, TYPE3_TYPE_SEALED_CASE_PC);
					break;
		default:    sprintf (line, "%-35s%s %d\n", TYPE3_TYPE, SEP1, type3->type & 0x7F);
	}
    strcat(file, line);

    /* SMBIOS 2.1 and higher */
    if(smbiosstruct->length > 0x09)
    {
 		switch(type3->bootup_state)
		{
			case 1:		sprintf (line, "%-35s%s %s\n", TYPE3_BOOT_STATE, SEP1, TYPE3_STATE_OTHER);
						break;
			case 2:		sprintf (line, "%-35s%s %s\n", TYPE3_BOOT_STATE, SEP1, TYPE3_STATE_UNKNOWN);
						break;
			case 3:		sprintf (line, "%-35s%s %s\n", TYPE3_BOOT_STATE, SEP1, TYPE3_STATE_SAFE);
						break;
			case 4:		sprintf (line, "%-35s%s %s\n", TYPE3_BOOT_STATE, SEP1, TYPE3_STATE_WARNING);
						break;
			case 5:		sprintf (line, "%-35s%s %s\n", TYPE3_BOOT_STATE, SEP1, TYPE3_STATE_CRITICAL);
						break;
			case 6:		sprintf (line, "%-35s%s %s\n", TYPE3_BOOT_STATE, SEP1, TYPE3_STATE_NON_RECOVERABLE);
						break;
			default:    sprintf (line, "%-35s%s %d\n", TYPE3_BOOT_STATE, SEP1, type3->bootup_state);
		}
  	    strcat(file,line);

        switch(type3->pwr_supply_state)
		{
			case 1:		sprintf (line, "%-35s%s %s\n", TYPE3_POWER_SUPPLY_STATE, SEP1, TYPE3_STATE_OTHER);
						break;
			case 2:		sprintf (line, "%-35s%s %s\n", TYPE3_POWER_SUPPLY_STATE, SEP1, TYPE3_STATE_UNKNOWN);
						break;
		    case 3:		sprintf (line, "%-35s%s %s\n", TYPE3_POWER_SUPPLY_STATE, SEP1, TYPE3_STATE_SAFE);
						break;
			case 4:		sprintf (line, "%-35s%s %s\n", TYPE3_POWER_SUPPLY_STATE, SEP1, TYPE3_STATE_WARNING);
						break;
			case 5:		sprintf (line, "%-35s%s %s\n", TYPE3_POWER_SUPPLY_STATE, SEP1, TYPE3_STATE_CRITICAL);
						break;
			case 6:		sprintf (line, "%-35s%s %s\n", TYPE3_POWER_SUPPLY_STATE, SEP1, TYPE3_STATE_NON_RECOVERABLE);
						break;
			default:    sprintf (line, "%-35s%s %d\n", TYPE3_POWER_SUPPLY_STATE, SEP1, type3->bootup_state);
		}
  	    strcat(file,line);

        switch(type3->thermal_state)
		{
			case 1:		sprintf (line, "%-35s%s %s\n", TYPE3_THERMAL_STATE, SEP1, TYPE3_STATE_OTHER);
						break;
			case 2:		sprintf (line, "%-35s%s %s\n",  TYPE3_THERMAL_STATE, SEP1, TYPE3_STATE_UNKNOWN);
						break;
			case 3:		sprintf (line, "%-35s%s %s\n",  TYPE3_THERMAL_STATE, SEP1, TYPE3_STATE_SAFE);
						break;
			case 4:		sprintf (line, "%-35s%s %s\n",  TYPE3_THERMAL_STATE, SEP1, TYPE3_STATE_WARNING);
						break;
			case 5:		sprintf (line, "%-35s%s %s\n",  TYPE3_THERMAL_STATE, SEP1, TYPE3_STATE_CRITICAL);
						break;
			case 6:		sprintf (line, "%-35s%s %s\n",  TYPE3_THERMAL_STATE, SEP1, TYPE3_STATE_NON_RECOVERABLE);
						break;
			default:    sprintf (line, "%-35s%s %d\n",  TYPE3_THERMAL_STATE, SEP1, type3->thermal_state);
		}
  	    strcat(file,line);

        switch(type3->security_state)
		{
			case 1:		sprintf (line, "%-35s%s %s\n", TYPE3_SECURITY_STATE, SEP1, TYPE3_SEC_OTHER);
						break;
			case 2:		sprintf (line, "%-35s%s %s\n", TYPE3_SECURITY_STATE, SEP1, TYPE3_SEC_UNKNOWN);
						break;
			case 3:		sprintf (line, "%-35s%s %s\n", TYPE3_SECURITY_STATE, SEP1, TYPE3_SEC_NONE);
						break;
			case 4:		sprintf (line, "%-35s%s %s\n", TYPE3_SECURITY_STATE, SEP1, TYPE3_SEC_EXT_LOCKED_OUT);
						break;
			case 5:		sprintf (line, "%-35s%s %s\n", TYPE3_SECURITY_STATE, SEP1, TYPE3_SEC_EXT_ENABLED);
						break;
			default:    sprintf (line, "%-35s%s %d\n", TYPE3_SECURITY_STATE, SEP1, type3->security_state);
		}
  	    strcat(file,line);

    }

    /* SMBIOS 2.3 and higher */
    if(smbiosstruct->length > 0x0D)
    {
        sprintf(line, "%-35s%s %.8X h\n", TYPE3_OEM_DEFINED, SEP1, type3->oem_defined);
        strcat(file, line);
    }

    /* now line contains a string with fully interpreted type 3 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;

		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;

}


/** \fn unsigned char * bios_cook_type_4 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 4 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 4 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 4 (Processor) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_4 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_4 *type4;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];

    int i;
    unsigned char help[16];
    unsigned char help2[16];


    /* cast our data ptr to a structure ptr of SMBIOS type 4 */
    type4 = (smbios_type_4 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE4_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);



	sprintf (line, "%-35s%s %s\n", TYPE4_SOCKET_DESIGNATION, SEP1, GetString(smbiosstruct, (unsigned int)(type4->socket_designation)));
	strcat(file, line);

	switch(type4->type)
	{
	    case 1:		sprintf (line, "%-35s%s %s\n", TYPE4_TYPE, SEP1, TYPE4_TYPE_OTHER);
					break;
		case 2:		sprintf (line, "%-35s%s %s\n", TYPE4_TYPE, SEP1, TYPE4_TYPE_UNKNOWN);
					break;
		case 3:		sprintf (line, "%-35s%s %s\n", TYPE4_TYPE, SEP1, TYPE4_TYPE_CENTRAL);
					break;
		case 4:		sprintf (line, "%-35s%s %s\n", TYPE4_TYPE, SEP1, TYPE4_TYPE_MATH);
					break;
		case 5:		sprintf (line, "%-35s%s %s\n", TYPE4_TYPE, SEP1, TYPE4_TYPE_DSP);
					break;
		case 6:		sprintf (line, "%-35s%s %s\n", TYPE4_TYPE, SEP1, TYPE4_TYPE_VIDEO);
					break;
		default:    sprintf (line, "%-35s%s %d\n", TYPE4_TYPE, SEP1, type4->type);
	}
	strcat(file, line);

    switch(type4->family)
	{
        case 1:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_OHTER);
        			break;
		case 2:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_UNKNOWN);
					break;
		case 3:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_8086);
					break;
		case 4:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_80286);
					break;
		case 5:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_I386);
					break;
		case 6:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_I486);
					break;
		case 7:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_8087);
					break;
		case 8:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_80287);
					break;
		case 9:		sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_80387);
					break;
		case 10:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_80487);
					break;
		case 11:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM);
					break;
		case 12:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_PRO);
					break;
		case 13:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_II);
					break;
		case 14:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_MMX);
					break;
		case 15:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_CELERON);
					break;
		case 16:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_II_XEON);
					break;
		case 17:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_III);
					break;
		case 18:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_M1);
					break;
		case 19:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_M2);
					break;
		case 20:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_M1_RES);
					break;
		case 21:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_M1_RES);
					break;
		case 22:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_M1_RES);
					break;
		case 23:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_M1_RES);
					break;
		case 24:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_DURON);
					break;
		case 25:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_K5);
					break;
		case 26:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_K6);
					break;
		case 27:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_K62);
					break;
		case 28:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_K63);
					break;
		case 29:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ATHLON);
					break;
		case 30:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_29000);
					break;
		case 31:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_K62P);
					break;
		case 32:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC);
					break;
		case 33:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_601);
					break;
		case 34:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_603);
					break;
		case 35:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_603P);
					break;
		case 36:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_604);
					break;
	    case 37:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_620);
					break;
		case 38:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_X704);
					break;
		case 39:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_POWERPC_750);
					break;
		case 48:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA);
					break;
		case 49:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21064);
					break;
		case 50:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21066);
					break;
		case 51:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21164);
					break;
		case 52:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21164PC);
					break;
		case 53:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21164a);
					break;
		case 54:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21264);
					break;
		case 55:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ALPHA_21364);
					break;
		case 64:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MIPS);
					break;
	    case 65:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MIPS_R4000);
					break;
		case 66:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MIPS_R4200);
					break;
		case 67:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MIPS_R4400);
					break;
		case 68:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MIPS_R4600);
					break;
		case 69:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MIPS_R10000);
					break;
		case 80:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC);
					break;
		case 81:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_SUPER);
					break;
	    case 82:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_MICRO_II);
					break;
		case 83:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_MICRO_IIEP);
					break;
		case 84:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_ULTRA);
					break;
		case 85:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_ULTRA_II);
					break;
		case 86:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_ULTRA_II_I);
					break;
		case 87:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_ULTRA_III);
					break;
		case 88:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SPARC_ULTRA_III_I);
					break;
		case 96:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_68040);
					break;
		case 97:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_68XXX);
					break;
		case 98:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_68000);
					break;
		case 99:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_68010);
					break;
		case 100:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_68020);
					break;
		case 101:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_68030);
					break;
		case 112:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_HOBBIT);
					break;
		case 120:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_CRUSOE_TM5000);
					break;
		case 121:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_CRUSOE_TM3000);
					break;
		case 128:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_WEITEK);
					break;
		case 130:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ITANIUM);
					break;
		case 144:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC);
					break;
		case 145:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC_8500);
					break;
		case 146:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC_8000);
					break;
		case 147:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC_7300LC);
					break;
		case 148:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC_7200);
					break;
		case 149:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC_7100LC);
					break;
		case 150:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PARISC_7100);
					break;
		case 160:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_V30);
					break;
		case 176:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_III_XEON);
					break;
	    case 177:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_III_SPEED);
					break;
	    case 178:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_PENTIUM_4);
					break;
		case 180:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_AS400);
					break;
		case 200:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_IBM390);
					break;
		case 201:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_G4);
					break;
		case 202:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_G5);
					break;
		case 250:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_I860);
					break;
		case 251:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_I960);
					break;
		case 260:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SH3);
					break;
		case 261:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_SH4);
					break;
		case 280:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_ARM);
					break;
		case 281:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_STRONG_ARM);
					break;
		case 300:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_6X86);
					break;
	    case 301:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MEDIAGX);
					break;
	    case 302:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_MII);
					break;
		case 320:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_WINCHIP);
					break;
		case 350:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_DSP);
					break;
	    case 500:	sprintf (line, "%-35s%s %s\n", TYPE4_FAMILY, SEP1, TYPE4_FAMILY_VIDEO);
					break;
        default:    sprintf (line, "%-35s%s %d\n", TYPE4_FAMILY, SEP1, type4->family);
	}
    strcat(file, line);

    sprintf (line, "%-35s%s %s\n", TYPE4_MANUFACTURER, SEP1, GetString(smbiosstruct, (unsigned int)(type4->manufacturer)));
	strcat(file, line);

    sprintf(line, "%-35s%s ", TYPE4_ID, SEP1);
    strcat(file, line);
    for (i=0; i<8; i++)
 	{
		sprintf(help, "%.2X ", (unsigned char)(type4->id[i]));
        strcat(file, help);
	}
	strcat(file, "\n");

    sprintf (line, "%-35s%s %s\n", TYPE4_VERSION, SEP1, GetString(smbiosstruct, (unsigned int)(type4->version)));
	strcat(file, line);

    if((type4->voltage) & 0x80)
    {
        if(((type4->voltage) & 0x7F) > 10)
        {
            sprintf(help, "%d", (type4->voltage) & 0x7F);
            strncpy(help2, help, strlen(help)-1);
            *(help2 + strlen(help)-1) = 0x00;
        }
        else
        {
            strcpy(help2, "0");
        }

        strcat(help2, ".");

        if(((type4->voltage) & 0x7F) > 10)
        {
            strcat(help2, help + strlen(help)-1);
        }
        else
        {
            strcat(help2, help);
        }

        sprintf(line, "%-35s%s %sV\n", TYPE4_CURRENT_VOLTAGE, SEP1, help2);
        strcat(file, line);
    }
    else
    {
        sprintf (line, "%-35s%s\n", TYPE4_VOLTAGE, SEP1);
	    strcat(file, line);
        if(type4->voltage & 0x01)
        {
            sprintf (line, "%-35s  %s %s\n", "", SEP2, TYPE4_VOLTAGE_50);
	        strcat(file, line);
        }
        if(type4->voltage & 0x02)
        {
            sprintf (line, "%-35s  %s %s\n", "", SEP2, TYPE4_VOLTAGE_33);
	        strcat(file, line);
		}
        if(type4->voltage & 0x04)
        {
            sprintf (line, "%-35s  %s %s\n", "", SEP2, TYPE4_VOLTAGE_29);
	        strcat(file, line);
        }
    }

    if(type4->external_clock > 0)
    {
        sprintf (line, "%-35s%s %d %s\n", TYPE4_EXTERNAL_CLOCK, SEP1, type4->external_clock, MHZ);
        strcat(file, line);
    }

    if(type4->max_speed > 0)
    {
        sprintf (line, "%-35s%s %d %s\n", TYPE4_MAX_SPEED, SEP1, type4->max_speed, MHZ);
	    strcat(file, line);
    }

    if(type4->current_speed > 0)
    {
        sprintf (line, "%-35s%s %d %s\n", TYPE4_CURRENT_SPEED, SEP1, type4->current_speed, MHZ);
	    strcat(file, line);
    }

    switch(type4->status &0x07)
	{
	    case 0:		sprintf (line, "%-35s%s %s\n", TYPE4_STATUS, SEP1, TYPE4_STATUS_UNKNOWN);
					break;
        case 1:		sprintf (line, "%-35s%s %s\n", TYPE4_STATUS, SEP1, TYPE4_STATUS_ENABLED);
					break;
        case 2:		sprintf (line, "%-35s%s %s\n", TYPE4_STATUS, SEP1, TYPE4_STATUS_DISABLED_USER);
					break;
		case 3:		sprintf (line, "%-35s%s %s\n", TYPE4_STATUS, SEP1, TYPE4_STATUS_DISABLED_POST);
					break;
	    case 4:		sprintf (line, "%-35s%s %s\n", TYPE4_STATUS, SEP1, TYPE4_STATUS_IDLE);
					break;
		case 7:		sprintf (line, "%-35s%s %s\n", TYPE4_STATUS, SEP1, TYPE4_STATUS_OTHER);
					break;
  	    default:    sprintf (line, "%-35s%s %d\n", TYPE4_STATUS, SEP1, type4->status &0x07);
	}
	strcat(file, line);

    if(type4->status &0x40)
        sprintf (line, "%-35s%s %s\n", TYPE4_SOCKET, SEP1, TYPE4_SOCKET_POPULATED);
    else
        sprintf (line, "%-35s%s %s\n", TYPE4_SOCKET, SEP1, TYPE4_SOCKET_UNPOPULATED);
    strcat(file, line);

    switch(type4->upgrade)
	{
        case 1:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_OTHER);
					break;
		case 2:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_UNKNOWN);
					break;
		case 3:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_DAUGHTER);
					break;
		case 4:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_ZIFF);
					break;
		case 5:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_PIGGY_BACK);
					break;
		case 6:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_NONE);
					break;
		case 7:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_LIF);
					break;
		case 8:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_SLOT1);
					break;
		case 9:		sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_SLOT2);
					break;
		case 10:	sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_370);
					break;
		case 11:	sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_A);
					break;
		case 12:	sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_M);
					break;
		case 13:	sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_423);
			    	break;
		case 14:	sprintf (line, "%-35s%s %s\n", TYPE4_UPGRADE, SEP1, TYPE4_UPGRADE_A462);
					break;
        default:    sprintf (line, "%-35s%s %d\n", TYPE4_UPGRADE, SEP1, type4->upgrade);
	}
	strcat(file, line);


    /* SMBIOS 2.1 and higher */
    if(smbiosstruct->length > 0x1A)
    {
        if(type4->l1_cache_handle != 0xFFFF)
        {
            sprintf(line, "%-35s%s %d\n", TYPE4_L1_CACHE_HANDLE, SEP1, type4->l1_cache_handle);
            strcat(file, line);
        }

        if(type4->l2_cache_handle != 0xFFFF)
        {
            sprintf(line, "%-35s%s %d\n", TYPE4_L2_CACHE_HANDLE, SEP1, type4->l2_cache_handle);
            strcat(file, line);
        }

        if(type4->l3_cache_handle != 0xFFFF)
        {
            sprintf(line, "%-35s%s %d\n", TYPE4_L3_CACHE_HANDLE, SEP1, type4->l3_cache_handle);
            strcat(file, line);
        }

    }


    /* SMBIOS 2.3 and higher */
    if(smbiosstruct->length > 0x20)
    {
        sprintf (line, "%-35s%s %s\n", TYPE4_SERIAL_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type4->serialnumber)));
  	    strcat(file, line);

        sprintf (line, "%-35s%s %s\n", TYPE4_ASSET_TAG, SEP1, GetString(smbiosstruct, (unsigned int)(type4->asset_tag)));
	    strcat(file, line);

        sprintf (line, "%-35s%s %s\n", TYPE4_PART_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type4->part_number)));
	    strcat(file, line);

    }


    /* now line contains a string with fully interpreted type 4 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;

}


/** \fn unsigned char * bios_cook_type_5 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 5 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 5 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 5 (Memory Controller) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_5 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_5 *type5;
    unsigned char * scratch;

    /* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];

    unsigned char *ptr;
    int i;


    /* cast our data ptr to a structure ptr of SMBIOS type 5 */
    type5 = (smbios_type_5 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE5_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);


    switch(type5->error_detection)
	{
	    case 1:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_OTHER);
					break;
        case 2:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_UNKNOWN);
					break;
        case 3:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_NONE);
					break;
        case 4:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_PARITY);
					break;
        case 5:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_32ECC);
					break;
        case 6:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_64ECC);
					break;
        case 7:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_128ECC);
					break;
        case 8:		sprintf (line, "%-35s%s %s\n", TYPE5_ERROR_DETECTION, SEP1, TYPE5_ED_CRC);
					break;
    	default:    sprintf (line, "%-35s%s %d\n", TYPE5_ERROR_DETECTION, SEP1, type5->error_detection);
	}
  	strcat(file, line);

    sprintf(line, "%-35s%s\n", TYPE5_ECC, SEP1);
    strcat(file, line);
    if( (type5->ecc) & 0x01 )
    {   sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_OTHER);
        strcat(file, line);
    }
    if( (type5->ecc) & 0x02 )
    {   sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_UNKNOWN);
        strcat(file, line);
    }
    if( (type5->ecc) & 0x04 )
    {   sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_NONE);
        strcat(file, line);
    }
    if( (type5->ecc) & 0x08 )
    {   sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_SINGLE_BIT);
        strcat(file, line);
    }
    if( (type5->ecc) & 0x10 )
    {   sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_DOUBLE_BIT);
        strcat(file, line);
    }
    if( (type5->ecc) & 0x20 )
    {   sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_SCRUBBING);
        strcat(file, line);
    }

    switch(type5->supported_interleave)
	{
	    case 1:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_OTHER);
					break;
        case 2:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_UNKNOWN);
					break;
        case 3:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_ONE_WAY);
					break;
        case 4:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_TWO_WAY);
					break;
        case 5:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_FOUR_WAY);
					break;
        case 6:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_EIGHT_WAY);
					break;
        case 7:		sprintf (line, "%-35s%s %s\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, TYPE5_I_SIXTEEN_WAY);
					break;
    	default:    sprintf (line, "%-35s%s %d\n", TYPE5_SUPPORTED_INTERLEAVE, SEP1, type5->supported_interleave);
	}
  	strcat(file, line);

    switch(type5->current_interleave)
	{

	    case 1:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_OTHER);
					break;
        case 2:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_UNKNOWN);
					break;
        case 3:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_ONE_WAY);
					break;
        case 4:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_TWO_WAY);
					break;
        case 5:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_FOUR_WAY);
					break;
        case 6:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_EIGHT_WAY);
					break;
        case 7:		sprintf (line, "%-35s%s %s\n", TYPE5_CURRENT_INTERLEAVE, SEP1, TYPE5_I_SIXTEEN_WAY);
					break;
    	default:    sprintf (line, "%-35s%s %d\n", TYPE5_CURRENT_INTERLEAVE, SEP1, type5->current_interleave);
	}
  	strcat(file, line);

    sprintf(line, "%-35s%s %d %s\n", TYPE5_MAX_MEM_MOD_SIZE, SEP1, pow2(type5->max_mem_module_size), MB);
    strcat(file, line);

    sprintf(line, "%-35s%s\n", TYPE5_SUPPORTED_SPEEDS, SEP1);
    strcat(file, line);
    if( (type5->supported_speeds) & 0x01 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SSPEED_OTHER);
        strcat(file, line);
    }
    if( (type5->supported_speeds) & 0x02 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SSPEED_UNKNOWN);
        strcat(file, line);
    }
    if( (type5->supported_speeds) & 0x04 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SSPEED_70);
        strcat(file, line);
    }
    if( (type5->supported_speeds) & 0x08 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SSPEED_60);
        strcat(file, line);
    }
    if( (type5->supported_speeds) & 0x10 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SSPEED_50);
        strcat(file, line);
    }

    sprintf(line, "%-35s%s\n", TYPE5_SUPPORTED_TYPES, SEP1);
    strcat(file, line);
    if( (type5->supported_types) & 0x0001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_OTHER);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_UNKNOWN);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_STANDARD);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_FAST_PAGE);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_EDO);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_PARITY);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_ECC);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0080 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_SIMM);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0100 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_DIMM);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0200 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_BURST_EDO);
        strcat(file, line);
    }
    if( (type5->supported_types) & 0x0400 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ST_SDRAM);
        strcat(file, line);
    }

    sprintf(line, "%-35s%s\n", TYPE5_SUPPORTED_VOLTAGES, SEP1);
    strcat(file, line);
    if( (type5->voltage) & 0x01 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SV_50);
        strcat(file, line);
    }
    if( (type5->voltage) & 0x02 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SV_33);
        strcat(file, line);
    }
    if( (type5->voltage) & 0x04 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_SV_29);
        strcat(file, line);
    }

    sprintf(line, "%-35s%s %d %s \n", TYPE5_NR_ASSOC_SLOTS, SEP1, type5->nr_assoc_slots, TYPE5_SLOTS);
    strcat(file, line);

    ptr = (unsigned char *)smbiosstruct;
    ptr += 0x0f;

    sprintf(line, "%-35s%s \n", TYPE5_MEM_MOD_HANDLES, SEP1);
    strcat(file, line);
    for (i=0; i < (int)(type5->nr_assoc_slots); i++)
    {
        sprintf(line, "%-35s  %s %s %d \n", "", SEP2, HANDLE, (unsigned int)(*ptr));
        strcat(file, line);
        ptr += 2;
    }


    /* SMBIOS 2.1 and higher */
    if( smbiosstruct->length == ((type5->nr_assoc_slots * 2) + 16) )
    {
        ptr = (unsigned char *) smbiosstruct;
        ptr = ptr + ((type5->nr_assoc_slots * 2) + 0x0F);

        sprintf(line, "%-35s%s\n", TYPE5_ENABLED_ECC, SEP1);
        strcat(file, line);
        if( (*ptr) & 0x01 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_OTHER);
            strcat(file, line);
        }
        if( (*ptr) & 0x02 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_UNKNOWN);
            strcat(file, line);
        }
        if( (*ptr) & 0x04 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_NONE);
            strcat(file, line);
        }
        if( (*ptr) & 0x08 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_SINGLE_BIT);
            strcat(file, line);
        }
        if( (*ptr) & 0x10 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_DOUBLE_BIT);
            strcat(file, line);
        }
        if( (*ptr) & 0x20 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE5_ECC_SCRUBBING);
            strcat(file, line);
        }
    }

    /* now line contains a string with fully interpreted type 4 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;

}


/** \fn unsigned char * bios_cook_type_6 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 6 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 6 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 6 (Memory Module) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_6 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_6 *type6;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];


    /* cast our data ptr to a structure ptr of SMBIOS type 6 */
    type6 = (smbios_type_6 *)smbiosstruct;

    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE6_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

	sprintf (line, "%-35s%s %s\n", TYPE6_SOCKET_DESIGNATION, SEP1, GetString(smbiosstruct, (unsigned int)(type6->designation)));
	strcat(file, line);

    sprintf(line, "%-35s%s ", TYPE6_RAS, SEP1);
    strcat(file, line);
    if( (type6->connection & 0xF0) != 0xF0 )
    {
        sprintf(line, "%d", (type6->connection & 0xF0) >> 4);
        strcat(file, line);
    }
    if( (type6->connection & 0x0F) != 0x0F )
    {
        sprintf(line, " & %d", (type6->connection & 0x0F));
        strcat(file, line);
    }
    strcat(file, "\n");

    if(type6->current_speed > 0)
    {
        sprintf(line, "%-35s%s %d %s \n", TYPE6_CURRENT_SPEED, SEP1, type6->current_speed, NS);
        strcat(file, line);
    }
    
    sprintf(line, "%-35s%s\n", TYPE6_CURRENT_TYPE, SEP1);
    strcat(file, line);
    if( (type6->current_type) & 0x0001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_OTHER);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_UNKNOWN);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_STANDARD);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_FAST_PAGE);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_EDO);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_PARITY);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_ECC);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0080 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_SIMM);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0100 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_DIMM);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0200 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_BURST_EDO);
        strcat(file, line);
    }
    if( (type6->current_type) & 0x0400 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_CT_SDRAM);
        strcat(file, line);
    }

    if(type6->installed_size & 0x80)
        sprintf(line, "%-35s%s %s \n", TYPE6_INSTALLED_SIZE, SEP1, TYPE6_S_DBL_BANK);
    else
        sprintf(line, "%-35s%s %s \n", TYPE6_INSTALLED_SIZE, SEP1, TYPE6_S_SINGLE_BANK);
    strcat(file, line);
    if((type6->installed_size & 0x7F) == 0x7F)
    {
        sprintf(line, "%-35s%s %s \n", TYPE6_INSTALLED_SIZE, SEP1, TYPE6_S_NOT_INSTALLED);
        strcat(file, line);
    }
    if((type6->installed_size & 0x7F) == 0x7E)
    {
        sprintf(line, "%-35s%s %s \n", TYPE6_INSTALLED_SIZE, SEP1, TYPE6_S_NOT_ENABLED);
        strcat(file, line);
    }
    if((type6->installed_size & 0x7F) == 0x7D)
    {
        sprintf(line, "%-35s%s %s \n", TYPE6_INSTALLED_SIZE, SEP1, TYPE6_S_NOT_DETERMINABLE);
        strcat(file, line);
    }

    if((type6->installed_size & 0x7F) < 0x7D)
    {
        sprintf(line, "%-35s%s %d %s\n", TYPE6_INSTALLED_SIZE, SEP1, pow2(type6->installed_size & 0x7F), MB);
        strcat(file, line);
    }

    if(type6->enabled_size & 0x80)
        sprintf(line, "%-35s%s %s \n", TYPE6_ENABLED_SIZE, SEP1, TYPE6_S_DBL_BANK);
    else
        sprintf(line, "%-35s%s %s \n", TYPE6_ENABLED_SIZE, SEP1, TYPE6_S_SINGLE_BANK);
    strcat(file, line);
    if((type6->enabled_size & 0x7F) == 0x7F)
    {
        sprintf(line, "%-35s%s %s \n", TYPE6_ENABLED_SIZE, SEP1, TYPE6_S_NOT_INSTALLED);
        strcat(file, line);
    }
    if((type6->enabled_size & 0x7F) == 0x7E)
    {
        sprintf(line, "%-35s%s %s \n", TYPE6_ENABLED_SIZE, SEP1, TYPE6_S_NOT_ENABLED);
        strcat(file, line);
    }

    if((type6->enabled_size & 0x7F) == 0x7D)
    {
        sprintf(line, "%-35s%s %s \n", TYPE6_ENABLED_SIZE, SEP1, TYPE6_S_NOT_DETERMINABLE);
        strcat(file, line);
    }

    if((type6->enabled_size & 0x7F) < 0x7D)
    {
        sprintf(line, "%-35s%s %d %s\n", TYPE6_ENABLED_SIZE, SEP1, pow2(type6->enabled_size & 0x7F), MB);
        strcat(file, line);
    }

    sprintf(line, "%-35s%s\n", TYPE6_STATUS, SEP1);
    strcat(file, line);
    if( (type6->status) & 0x01 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_STATUS_UNCORRECTABLE);
        strcat(file, line);
    }
    if( (type6->status) & 0x02 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_STATUS_CORRECTABLE);
        strcat(file, line);
    }
    if( (type6->status) & 0x04 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE6_STATUS_EVENTLOG);
        strcat(file, line);
    }


    /* now line contains a string with fully interpreted type 4 data */	
	*plength = strlen(file);

	/* allocate the memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */
	return scratch;
}


/** \fn unsigned char * bios_cook_type_7 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 7 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 7 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 7 (Cache) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_7 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_7 *type7;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];


    /* cast our data ptr to a structure ptr of SMBIOS type 7 */
    type7 = (smbios_type_7 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE7_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file,line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

	
	/* prepare interpreted type 7 string */
	sprintf (line, "%-35s%s %s\n", TYPE7_SOCKET_DESIGNATION, SEP1, GetString(smbiosstruct, (unsigned int)(type7->designation)));
 	strcat(file, line);
 	
	switch( ((type7->configuration) & 0x0300) >> 8 )
	{
	    case 0:     sprintf (line, "%-35s%s %s\n", TYPE7_MODE, SEP1, TYPE7_MODE_WT);
	                break;
	    case 1:     sprintf (line, "%-35s%s %s\n", TYPE7_MODE, SEP1, TYPE7_MODE_WB);
	                break;
	    case 2:     sprintf (line, "%-35s%s %s\n", TYPE7_MODE, SEP1, TYPE7_MODE_VAR);
	                break;
	    case 3:     sprintf (line, "%-35s%s %s\n", TYPE7_MODE, SEP1, TYPE7_MODE_UNK);
	                break;
	    default:    sprintf (line, "%-35s%s %d\n", TYPE7_MODE, SEP1, ((type7->configuration) & 0x0300) >> 8);
 	};
 	strcat(file, line);

	switch( ((type7->configuration) & 0x0080) >> 7 )
	{
	    case 0:     sprintf (line, "%-35s%s %s\n", TYPE7_EN, SEP1, SMB_FALSE);
	                break;
	    case 1:     sprintf (line, "%-35s%s %s\n", TYPE7_EN, SEP1, SMB_TRUE);
	                break;
	    default:    sprintf (line, "%-35s%s %d\n", TYPE7_EN, SEP1, ((type7->configuration) & 0x0080) >> 7);
 	};
 	strcat(file, line);

	switch( ((type7->configuration) & 0x0060) >> 5 )
	{
	    case 0:     sprintf (line, "%-35s%s %s\n", TYPE7_LOCATION, SEP1, TYPE7_LOC_INT);
	                break;
	    case 1:     sprintf (line, "%-35s%s %s\n", TYPE7_LOCATION, SEP1, TYPE7_LOC_EXT);
	                break;
	    case 2:     sprintf (line, "%-35s%s %s\n", TYPE7_LOCATION, SEP1, TYPE7_LOC_RES);
	                break;
	    case 3:     sprintf (line, "%-35s%s %s\n", TYPE7_LOCATION, SEP1, TYPE7_LOC_UNK);
	                break;
        default:    sprintf (line, "%-35s%s %d\n", TYPE7_LOCATION, SEP1, ((type7->configuration) & 0x0060) >> 5);	
 	};
 	strcat(file, line);

	switch( ((type7->configuration) & 0x0008) >> 3 )
	{
	    case 0:     sprintf (line, "%-35s%s %s\n", TYPE7_SOCKETED, SEP1, SMB_FALSE);
	                break;
	    case 1:     sprintf (line, "%-35s%s %s\n", TYPE7_SOCKETED, SEP1, SMB_TRUE);
	                break;
	    default:    sprintf (line, "%-35s%s %d\n", TYPE7_SOCKETED, SEP1, ((type7->configuration) & 0x0008) >> 3);
 	};
 	strcat(file, line);
 	
 	sprintf (line, "%-35s%s %d\n", TYPE7_LEVEL, SEP1, ((type7->configuration) & 0x0007) + 1 );
 	strcat(file, line);
 	
 	if((type7->max_size) & 0x8000)
 	    sprintf(line, "%-35s%s %d %s\n", TYPE7_MAX_SIZE, SEP1, ((type7->max_size) & 0x7FFF) * 64, KB);
 	else
 	    sprintf(line, "%-35s%s %d %s\n", TYPE7_MAX_SIZE, SEP1, (type7->max_size) & 0x7FFF, KB);
 	strcat(file, line);

 	if((type7->installed_size) & 0x8000)
 	    sprintf(line, "%-35s%s %d %s\n", TYPE7_INST_SIZE, SEP1, ((type7->installed_size) & 0x7FFF) * 64, KB);
 	else
 	    sprintf(line, "%-35s%s %d %s\n", TYPE7_INST_SIZE, SEP1, (type7->installed_size) & 0x7FFF, KB);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s\n", TYPE7_SUP_SRAM_TYPE, SEP1);
 	strcat(file, line);
    if( (type7->supported_sram) & 0x0001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_OTHER);
        strcat(file, line);
    }
    if( (type7->supported_sram) & 0x0002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_UNKNOWN);
        strcat(file, line);
    }
    if( (type7->supported_sram) & 0x0004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_NON_BURST);
        strcat(file, line);
    }
    if( (type7->supported_sram) & 0x0008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_BURST);
        strcat(file, line);
    }
    if( (type7->supported_sram) & 0x0010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_PIPELINE_BURST);
        strcat(file, line);
    }
    if( (type7->supported_sram) & 0x0020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_SYNCHRONOUS);
        strcat(file, line);
    }
    if( (type7->supported_sram) & 0x0040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_ASYNCHRONOUS);
        strcat(file, line);
    }
 	
 	sprintf(line, "%-35s%s\n", TYPE7_CUR_SRAM_TYPE, SEP1);
 	strcat(file, line);
    if( (type7->current_sram) & 0x0001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_OTHER);
        strcat(file, line);
    }
    if( (type7->current_sram) & 0x0002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_UNKNOWN);
        strcat(file, line);
    }
    if( (type7->current_sram) & 0x0004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_NON_BURST);
        strcat(file, line);
    }
    if( (type7->current_sram) & 0x0008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_BURST);
        strcat(file, line);
    }
    if( (type7->current_sram) & 0x0010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_PIPELINE_BURST);
        strcat(file, line);
    }
    if( (type7->current_sram) & 0x0020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_SYNCHRONOUS);
        strcat(file, line);
    }
    if( (type7->current_sram) & 0x0040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE7_ST_ASYNCHRONOUS);
        strcat(file, line);
    }
 	 	 	 	

    /* if smbios 2.1 or higher */
    if(smbiosstruct->length > 0x0F)		
	{
	    if(type7->speed > 0)
	    {
	        sprintf(line, "%-35s%s %d %s \n", TYPE7_SPEED, SEP1, type7->speed, NS);
	        strcat(file, line);
	    }
	
	    switch(type7->ecc_type)
	    {
	        case 1:     sprintf(line, "%-35s%s %s \n", TYPE7_ECC_TYPE, SEP1, TYPE7_ECC_OTHER);
	                    break;
	        case 2:     sprintf(line, "%-35s%s %s \n", TYPE7_ECC_TYPE, SEP1, TYPE7_ECC_UNKNOWN);
	                    break;
	        case 3:     sprintf(line, "%-35s%s %s \n", TYPE7_ECC_TYPE, SEP1, TYPE7_ECC_NONE);
	                    break;
	        case 4:     sprintf(line, "%-35s%s %s \n", TYPE7_ECC_TYPE, SEP1, TYPE7_ECC_PARITY);
	                    break;
	        case 5:     sprintf(line, "%-35s%s %s \n", TYPE7_ECC_TYPE, SEP1, TYPE7_ECC_SINGLE_ECC);
	                    break;
	        case 6:     sprintf(line, "%-35s%s %s \n", TYPE7_ECC_TYPE, SEP1, TYPE7_ECC_MULIT_ECC);
	                    break;
	        default:    sprintf(line, "%-35s%s %d \n", TYPE7_ECC_TYPE, SEP1, type7->ecc_type);
	    }
	    strcat(file, line);
	
	    switch(type7->cache_type)
	    {
	        case 1:     sprintf(line, "%-35s%s %s \n", TYPE7_CACHE_TYPE, SEP1, TYPE7_TYPE_OTHER);
	                    break;
	        case 2:     sprintf(line, "%-35s%s %s \n", TYPE7_CACHE_TYPE, SEP1, TYPE7_TYPE_UNKNOWN);
	                    break;
	        case 3:     sprintf(line, "%-35s%s %s \n", TYPE7_CACHE_TYPE, SEP1, TYPE7_TYPE_INSTRUCTION);
	                    break;
	        case 4:     sprintf(line, "%-35s%s %s \n", TYPE7_CACHE_TYPE, SEP1, TYPE7_TYPE_DATA);
	                    break;
	        case 5:     sprintf(line, "%-35s%s %s \n", TYPE7_CACHE_TYPE, SEP1, TYPE7_TYPE_UNIFIED);
	                    break;
	        default:    sprintf(line, "%-35s%s %d \n", TYPE7_CACHE_TYPE, SEP1, type7->cache_type);
	    }
	    strcat(file, line);

	    switch(type7->associativity)
	    {
	        case 1:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_OTHER);
	                    break;
	        case 2:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_UNKNOWN);
	                    break;
	        case 3:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_DIREC_MAPPED);
	                    break;
	        case 4:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_2WAY);
	                    break;
	        case 5:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_4WAY);
	                    break;
	        case 6:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_FULL);
	                    break;
	        case 7:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_8WAY);
	                    break;
	        case 8:     sprintf(line, "%-35s%s %s \n", TYPE7_ASSOCIATIVITY, SEP1, TYPE7_ASSOC_16WAY);
	                    break;
	        default:    sprintf(line, "%-35s%s %d \n", TYPE7_ASSOCIATIVITY, SEP1, type7->associativity);
	    }
	    strcat(file, line);
	    	
	} /* end - if smbios 2.1 or higher */

	/* now line contains a string with fully interpreted type 1 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */	
    /* the caller is responsible to free the memory. */	
    return scratch;

}


/** \fn unsigned char * bios_cook_type_8 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 8 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 8 raw structure

  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 8 (Port Connector) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_8 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_8 *type8;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
	unsigned char file[2800];

	/* contains one line of the above file */
    unsigned char line[128];


    /* cast our data ptr to a structure ptr of SMBIOS type 8 */
    type8 = (smbios_type_8 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE8_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);


	/* prepare interpreted type 8 strings */
	sprintf (line, "%-35s%s %s\n", TYPE8_INT_REF_DESIGNATOR, SEP1, GetString(smbiosstruct, (unsigned int)(type8->internal_designation)));
 	strcat(file, line);

    switch(type8->internal_type)
	{
	    case 0:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_NONE);
	                break;
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_CENTRONICS);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MINI_CENTRONICS);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PROPRIETARY);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DB25_MALE);
	                break;
	    case 5:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DB25_FEMALE);
	                break;
        case 6:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DB15_MALE);
	                break;
        case 7:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DB15_FEMALE);
	                break;
        case 8:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DB9_MALE);
	                break;
        case 9:     sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DB9_FEMALE);
	                break;
        case 10:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_RJ11);
	                break;
        case 11:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_RJ45);
	                break;
        case 12:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MINI_SCSI);
	                break;
        case 13:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MINI_DIN);
	                break;
        case 14:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MICRO_DIN);
	                break;
        case 15:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PS2);
	                break;
        case 16:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_INFRARED);
	                break;
        case 17:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_HPHIL);
                    break;
        case 18:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_USB);
	                break;
        case 19:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_SSA_SCSI);
	                break;
        case 20:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DIN8_MALE);
	                break;
        case 21:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_DIN8_FEMALE);
	                break;
        case 22:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_ONBOARD_IDE);
	                break;
        case 23:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_ONBOARD_FLOPPY);
	                break;
        case 24:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_9PIN_DIL);
	                break;
        case 25:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_25PIN_DIL);
	                break;
        case 26:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_50PIN_DIL);
	                break;
        case 27:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_68PIN_DIL);
	                break;
        case 28:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_ONBOARD_SND_CD);
	                break;
        case 29:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MINI_CENTRONICS_14);
	                break;
        case 30:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MINI_CENTRONICS_26);
	                break;
        case 31:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_MINI_JACK_PHONES);
	                break;
        case 32:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_BNC);
	                break;
        case 33:    sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_1394);
	                break;
        case 160:   sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PC98);
	                break;
        case 161:   sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PC98HIRESO);
	                break;
        case 162:   sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PCH98);
	                break;
        case 163:   sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PC98NOTE);
	                break;
        case 164:   sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_PC98FULL);
	                break;
        case 255:   sprintf(line, "%-35s%s %s \n", TYPE8_INT_TYPE, SEP1, TYPE8_CT_OTHER);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE8_INT_TYPE, SEP1, type8->internal_type);
	}
	strcat(file, line);

	sprintf (line, "%-35s%s %s\n", TYPE8_EXT_REF_DESIGNATOR, SEP1, GetString(smbiosstruct, (unsigned int)(type8->external_designation)));
 	strcat(file, line);

    switch(type8->external_type)
	{
	    case 0:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_NONE);
	                break;
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_CENTRONICS);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MINI_CENTRONICS);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PROPRIETARY);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DB25_MALE);
	                break;
	    case 5:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DB25_FEMALE);
	                break;
        case 6:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DB15_MALE);
	                break;
        case 7:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DB15_FEMALE);
	                break;
        case 8:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DB9_MALE);
	                break;
        case 9:     sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DB9_FEMALE);
	                break;
        case 10:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_RJ11);
	                break;
        case 11:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_RJ45);
	                break;
        case 12:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MINI_SCSI);
	                break;
        case 13:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MINI_DIN);
	                break;
        case 14:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MICRO_DIN);
	                break;
        case 15:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PS2);
	                break;
        case 16:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_INFRARED);
	                break;
        case 17:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_HPHIL);
	                break;
        case 18:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_USB);
	                break;
        case 19:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_SSA_SCSI);
	                break;
        case 20:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DIN8_MALE);
	                break;
        case 21:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_DIN8_FEMALE);
	                break;
        case 22:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_ONBOARD_IDE);
	                break;
        case 23:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_ONBOARD_FLOPPY);
	                break;
        case 24:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_9PIN_DIL);
	                break;
        case 25:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_25PIN_DIL);
	                break;
        case 26:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_50PIN_DIL);
	                break;
        case 27:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_68PIN_DIL);
	                break;
        case 28:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_ONBOARD_SND_CD);
	                break;
        case 29:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MINI_CENTRONICS_14);
	                break;
        case 30:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MINI_CENTRONICS_26);
	                break;
        case 31:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_MINI_JACK_PHONES);
	                break;
        case 32:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_BNC);
	                break;
        case 33:    sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_1394);
	                break;
        case 160:   sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PC98);
	                break;
        case 161:   sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PC98HIRESO);
	                break;
        case 162:   sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PCH98);
	                break;
        case 163:   sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PC98NOTE);
	                break;
        case 164:   sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_PC98FULL);
	                break;
        case 255:   sprintf(line, "%-35s%s %s \n", TYPE8_EXT_TYPE, SEP1, TYPE8_CT_OTHER);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE8_EXT_TYPE, SEP1, type8->external_type);
	}
	strcat(file, line);

    switch(type8->type)
	{
	    case 0:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_NONE);
	                break;
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PAR_XTAT);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PAR_PS2);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PAR_ECP);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PAR_EPP);
	                break;
	    case 5:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PAR_ECP_EPP);
	                break;
        case 6:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SER_XTAT);
	                break;
        case 7:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SER_16450);
	                break;
        case 8:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SER_16550);
	                break;
        case 9:     sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SER_16550A);
	                break;
        case 10:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SCSI);
	                break;
        case 11:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_MIDI);
	                break;
        case 12:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_JOYSTICK);
	                break;
        case 13:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_KEYBOARD);
	                break;
        case 14:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_MOUSE);
	                break;
        case 15:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SSA_SCSI);
	                break;
        case 16:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_USB);
	                break;
        case 17:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_1394);
	                break;
        case 18:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PCMCIA_I);
	                break;
        case 19:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PCMCIA_II);
	                break;
        case 20:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PCMCIA_III);
	                break;
        case 21:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_CARDBUS);
	                break;
        case 22:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_ACCESSBUS);
	                break;
        case 23:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SCSI_II);
	                break;
        case 24:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_SCSI_WIDE);
	                break;
        case 25:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PC98);
	                break;
        case 26:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PC98_HIRESO);
	                break;
        case 27:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_PCH98);
	                break;
        case 28:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_VIDEO);
	                break;
        case 29:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_AUDIO);
	                break;
        case 30:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_MODEM);
	                break;
        case 31:    sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_NET);
	                break;
        case 160:   sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_8251);
	                break;
        case 161:   sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_8251_FIFO);
	                break;
        case 255:   sprintf(line, "%-35s%s %s \n", TYPE8_TYPE, SEP1, TYPE8_TYPE_OTHER);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE8_TYPE, SEP1, type8->type);
	}
	strcat(file, line);


    /* now line contains a string with fully interpreted type 8 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_9 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 9 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 9 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 9 (System Slot) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_9 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_9 *type9;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
	/* is bigger than one pages (4k). since linux needs some space in this */
	/* page the variables should never exceed 3kB. */
    unsigned char file[2800];
	
	/* contains one line of the above file */
	unsigned char line[128];
	

    /* cast our data ptr to a structure ptr of SMBIOS type 9 */
    type9 = (smbios_type_9 *)smbiosstruct;

    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE9_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
 	
	/* prepare interpreted type 9 strings */
	sprintf (line, "%-35s%s %s\n", TYPE9_DESIGNATION, SEP1, GetString(smbiosstruct, (unsigned int)(type9->designation)));
 	strcat(file, line);

 	switch(type9->type)
	{
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_OTHER);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_UNKNOWN);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_ISA);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_MCA);
	                break;
	    case 5:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_EISA);
	                break;
        case 6:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PCI);
	                break;
        case 7:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PCMCIA);
	                break;
        case 8:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_VL);
	                break;
        case 9:     sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PROPRIETARY);
	                break;
        case 10:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PROCESSOR);
	                break;
        case 11:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_MEMORY);
	                break;
        case 12:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_RISER);
	                break;
        case 13:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_NUBUS);
	                break;
        case 14:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PCI66);
	                break;
        case 15:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_AGP);
	                break;
        case 16:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_AGP2);
	                break;
        case 17:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_AGP4);
	                break;
        case 18:    sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PCI_X);
	                break;
        case 160:   sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PC98_C20);
	                break;
        case 161:   sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PC98_C24);
	                break;
        case 162:   sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PC98_E);
	                break;
        case 163:   sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PC98_LOCAL_BUS);
	                break;
        case 164:   sprintf(line, "%-35s%s %s \n", TYPE9_TYPE, SEP1, TYPE9_TYPE_PC98_CARD);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE9_TYPE, SEP1, type9->type);
	}
	strcat(file, line);

 	switch(type9->width)
	{
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_OTHER);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_UNKNOWN);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_8);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_16);
	                break;
	    case 5:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_32);
	                break;
        case 6:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_64);
	                break;
        case 7:     sprintf(line, "%-35s%s %s \n", TYPE9_BUS_WIDTH, SEP1, TYPE9_BW_128);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE9_BUS_WIDTH, SEP1, type9->width);
	}
	strcat(file, line);

 	switch(type9->usage)
	{
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE9_CURRENT_USAGE, SEP1, TYPE9_CU_OTHER);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE9_CURRENT_USAGE, SEP1, TYPE9_CU_UNKNOWN);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE9_CURRENT_USAGE, SEP1, TYPE9_CU_AVAILABLE);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE9_CURRENT_USAGE, SEP1, TYPE9_CU_IN_USE);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE9_CURRENT_USAGE, SEP1, type9->usage);
	}
	strcat(file, line);                	
	
	switch(type9->length)
	{
	    case 1:     sprintf(line, "%-35s%s %s \n", TYPE9_LENGTH, SEP1, TYPE9_LENGTH_OTHER);
	                break;
	    case 2:     sprintf(line, "%-35s%s %s \n", TYPE9_LENGTH, SEP1, TYPE9_LENGTH_UNKNOWN);
	                break;
	    case 3:     sprintf(line, "%-35s%s %s \n", TYPE9_LENGTH, SEP1, TYPE9_LENGTH_SHORT);
	                break;
	    case 4:     sprintf(line, "%-35s%s %s \n", TYPE9_LENGTH, SEP1, TYPE9_LENGTH_LONG);
	                break;
	    default:    sprintf(line, "%-35s%s %d \n", TYPE9_LENGTH, SEP1, type9->length);
	}
	strcat(file, line);
	
	if(type9->type == 0x04)
	{
	    if(type9->id1 > 0)
	    {
	        sprintf(line, "%-35s%s %d \n", TYPE9_ID_MCA_SLOT_NR, SEP1, type9->id1);
	        strcat(file, line);
	    }
	}
	
	if(type9->type == 0x05)
	{
	    if(type9->id1 > 0)
	    {
	        sprintf(line, "%-35s%s %d \n", TYPE9_ID_EISA_SLOT_NR, SEP1, type9->id1);
	        strcat(file, line);
	    }
	}
	
	if( ((type9->type >= 0x0F) && (type9->type <= 0x12)) ||
	    (type9->type == 0x06) )
	{
	    sprintf(line, "%-35s%s %d \n", TYPE9_ID_PCI_SLOT_NR, SEP1, type9->id1);
	    strcat(file, line);
	}
	
	if(type9->type == 0x07)
	{
	    sprintf(line, "%-35s%s %d \n", TYPE9_ID_PCMCIA_ADAPTER_NR, SEP1, type9->id1);
	    strcat(file, line);
	    sprintf(line, "%-35s%s %d \n", TYPE9_ID_PCMCIA_SOCKET_NR, SEP1, type9->id2);
	    strcat(file, line);
	}
	
    sprintf(line, "%-35s%s\n", TYPE9_CHARACTERISTICS, SEP1);
    strcat(file, line);
    if( (type9->characteristics1) & 0x0001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_UNKNOWN);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_5V);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_33V);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_SHARED);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_PC_CARD_16);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_CARDBUS);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_ZOOM_VIDEO);
        strcat(file, line);
    }
    if( (type9->characteristics1) & 0x0080 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_MODEM_RESUME);
        strcat(file, line);
    }


    /* SMBios 2.1 and higher */
    if(smbiosstruct->length > 0x0C)
    {
        if( (type9->characteristics2) & 0x0001 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_PME);
            strcat(file, line);
        }
        if( (type9->characteristics2) & 0x0002 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_HOT_PLUG);
            strcat(file, line);
        }
        if( (type9->characteristics2) & 0x0004 )
        {
            sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE9_CHAR_SMBUS);
            strcat(file, line);
        }
    }


 	
    /* now line contains a string with fully interpreted type 9 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_10 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 10 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 10 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 10 (Onboard Device) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_10 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_10 *type10;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
	/* is bigger than one pages (4k). since linux needs some space in this */
	/* page the variables should never exceed 3kB. */
    unsigned char file[2800];
	
	/* contains one line of the above file */
	unsigned char line[128];
	
	
	int nr_of_devices = 0;
	int i;


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE10_NAME);
    strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

 	
	nr_of_devices = ((int)(smbiosstruct->length) - 4) / 2;
	
	for(i=0; i < nr_of_devices; i++)
	{
        /* cast our data ptr to a structure ptr of SMBIOS type 10 */
	    type10 = (smbios_type_10 *)( (unsigned char *)smbiosstruct + sizeof(smbios_header) + (i * sizeof(smbios_type_10)) );
	
	    /* prepare interpreted type 10 strings */
	    sprintf (line, "%-35s%s %d\n", TYPE10_NR, SEP1, i);
 	    strcat(file, line);
 	
 	    if(type10->type & 0x80)
 	    {
 	        sprintf (line, "%-35s%s %s\n", TYPE10_STATUS, SEP1, TYPE10_STATUS_ENABLED);
 	        strcat(file, line);
 	    }
 	    else
 	    {
 	        sprintf (line, "%-35s%s %s\n", TYPE10_STATUS, SEP1, TYPE10_STATUS_DISABLED);
 	        strcat(file, line);
 	    }
 	
 	    switch(type10->type & 0x7F)
 	    {
 	        case 1:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_OTHER);
 	                    break;
 	        case 2:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_UNKNOWN);
 	                    break;
 	        case 3:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_VIDEO);
 	                    break;
 	        case 4:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_SCSI);
 	                    break;
 	        case 5:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_ETHERNET);
 	                    break;
 	        case 6:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_TOKEN_RING);
 	                    break;
 	        case 7:     sprintf(line, "%-35s%s %s\n", TYPE10_TYPE, SEP1, TYPE10_TYPE_SOUND);
 	                    break;
 	        default:    sprintf(line, "%-35s%s %d\n", TYPE10_TYPE, SEP1, type10->type & 0x7F);
 	    }
 	    strcat(file, line);
 	
 	    sprintf (line, "%-35s%s %s\n\n", TYPE10_DEV_NAME, SEP1, GetString(smbiosstruct, (unsigned int)(type10->description)));
 	    strcat(file, line);
	}

 	
    /* now line contains a string with fully interpreted type 10 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_11 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 11 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 11 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 11 (OEM Strings) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */


unsigned char *
bios_cook_type_11 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_11 *type11;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];

	unsigned char help[128];
	
	int nr_of_strings = 0;
	int i, j;


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE11_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);

	 	
    /* cast our data ptr to a structure ptr of SMBIOS type 11 */
    type11 = (smbios_type_11 *)smbiosstruct;
 	
	nr_of_strings = (int)(type11->nr_of_strings);
	
	for(i=0; i < nr_of_strings; i++)
	{
	    sprintf(help, "%s %d", TYPE11_NR, i);
	    for(j=35-strlen(help); j > 0; j--)
	       strcat(help, " ");
	
	    /* prepare interpreted type 11 strings */
	    sprintf (line, "%s %s\n", SEP1, GetString(smbiosstruct, (unsigned int)(i + 1)) );
 	
	    strcat(file, help);
 	    strcat(file, line);
	}

 	
    /* now line contains a string with fully interpreted type 11 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_12 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 12 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 12 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 12 (System Configuration Options) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_12 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_12 *type12;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];
	
	unsigned char help[128];
	
	int nr_of_strings = 0;
	int i, j;


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE12_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 12 */
    type12 = (smbios_type_12 *)smbiosstruct;
 	
	nr_of_strings = (int)(type12->nr_of_strings);
	
	for(i=0; i < nr_of_strings; i++)
	{
	    sprintf(help, "%s %d", TYPE12_OPTION, i);
	    for(j=35-strlen(help); j > 0; j--)
	       strcat(help, " ");
	
	    /* prepare interpreted type 12 strings */
	    sprintf (line, "%s %s\n", SEP1, GetString(smbiosstruct, (unsigned int)(i + 1)) );
 	
 	    strcat(file, help);
 	    strcat(file, line);
	}

 	
    /* now line contains a string with fully interpreted type 12 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_13 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 13 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 13 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 13 (Bios Language Information) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_13 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_13 *type13;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];
	
	int nr_of_strings = 0;
	int i;


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE13_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 13 */
    type13 = (smbios_type_13 *)smbiosstruct;
 	
	nr_of_strings = (int)(type13->nr_of_strings);
	
	sprintf(line, "%-35s%s \n", TYPE13_SUP_LANG, SEP1);
	strcat(file, line);
	for(i=0; i < nr_of_strings; i++)
	{
	    /* prepare interpreted type 13 strings */
	    sprintf (line, "%-35s  %s %s\n", "", SEP2, GetString(smbiosstruct, (unsigned int)(i + 1)) );
 	    strcat(file, line);
	}

	sprintf (line, "%-35s%s %s\n", TYPE13_CUR_LANG, SEP1, GetString(smbiosstruct, (unsigned int)(type13->current_language)) );
    strcat(file, line);
 	
    /* now line contains a string with fully interpreted type 13 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_16 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 16 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 16 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 16 (Physical Memory Array) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_16 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_16 *type16;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE16_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 16 */
    type16 = (smbios_type_16 *)smbiosstruct;
 	
    switch(type16->location)
    {
 	    case 1:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_OTHER);
 	                break;
 	    case 2:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_UNKNOWN);
 	                break;
 	    case 3:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_MOTHERBOARD);
 	                break;
 	    case 4:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_ISA);
 	                break;
 	    case 5:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_EISA);
 	                break;
 	    case 6:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PCI);
 	                break;
 	    case 7:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_MCA);
 	                break;
 	    case 8:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PCMCIA);
 	                break;
 	    case 9:     sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PRORIETARY);
 	                break;
 	    case 10:    sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_NUBUS);
 	                break;
 	    case 160:   sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PC98C20);
 	                break;
 	    case 161:   sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PC98C24);
 	                break;
 	    case 162:   sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PC98E);
 	                break;
 	    case 163:   sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PC98LOCAL);
 	                break;
 	    case 164:   sprintf(line, "%-35s%s %s\n", TYPE16_LOCATION, SEP1, TYPE16_L_PC98CARD);
 	                break;
 	    default:    sprintf(line, "%-35s%s %d\n", TYPE16_LOCATION, SEP1, type16->location);
 	}
 	strcat(file, line);
 	
 	switch(type16->use)
 	{
 	    case 1:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_OTHER);
 	                break;
 	    case 2:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_UNKNOWN);
 	                break;
 	    case 3:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_SYSTEM);
 	                break;
 	    case 4:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_VIDEO);
 	                break;
 	    case 5:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_FLASH);
 	                break;
 	    case 6:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_NON_VOLATILE);
 	                break;
 	    case 7:     sprintf(line, "%-35s%s %s\n", TYPE16_USE, SEP1, TYPE16_USE_CACHE);
 	                break;
 	    default:    sprintf(line, "%-35s%s %d\n", TYPE16_USE, SEP1, type16->use);
 	}
 	strcat(file, line);
 	
 	switch(type16->ecc)
 	{
 	    case 1:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_OTHER);
 	                break;
 	    case 2:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_UNKNOWN);
 	                break;
 	    case 3:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_NONE);
 	                break;
 	    case 4:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_PARITY);
 	                break;
 	    case 5:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_SINGLE_ECC);
 	                break;
 	    case 6:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_DOUBLE_ECC);
 	                break;
 	    case 7:     sprintf(line, "%-35s%s %s\n", TYPE16_ECC, SEP1, TYPE16_ECC_CRC);
 	                break;
 	    default:    sprintf(line, "%-35s%s %d\n", TYPE16_ECC, SEP1, type16->ecc);
 	}
 	strcat(file, line);
 	
 	if(type16->max_capacity & 0x80000000)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE16_MAX_CAPACITY, SEP1, TYPE16_MC_UNKNOWN);
 	    strcat(file, line);
 	}
 	else
 	{
 	    sprintf(line, "%-35s%s %d %s\n", TYPE16_MAX_CAPACITY, SEP1, type16->max_capacity, KB);
 	    strcat(file, line);
 	}
 	
 	if(type16->error_handle == 0xFFFE)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE16_ERROR_HANDLE, SEP1, TYPE16_EH_NOT_SUPPORTED);
 	    strcat(file, line);
 	}
    if(type16->error_handle == 0xFFFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE16_ERROR_HANDLE, SEP1, TYPE16_EH_NO_ERROR);
 	    strcat(file, line);
 	}
 	if(type16->error_handle < 0xFFFE)
 	{
 	    sprintf(line, "%-35s%s %s %d\n", TYPE16_ERROR_HANDLE, SEP1, TYPE16_EH_HANDLE, type16->error_handle);
 	    strcat(file, line);
 	}
 	
 	sprintf(line, "%-35s%s %d\n", TYPE16_NR_DEVICES, SEP1, type16->nr_of_devices);
 	strcat(file, line);
 	

    /* now line contains a string with fully interpreted type 16 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_17 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 17 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 17 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 17 (Memory Device) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_17 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_17 *type17;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE17_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 17 */
    type17 = (smbios_type_17 *)smbiosstruct;

    sprintf(line, "%-35s%s %d\n", TYPE17_MEM_ARRAY_HANDLE, SEP1, type17->memory_array_handle);
 	strcat(file, line);
 	
 	if(type17->error_handle == 0xFFFE)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_ERROR_HANDLE, SEP1, TYPE17_EH_NOT_SUPPORTED);
 	    strcat(file, line);
 	}
    if(type17->error_handle == 0xFFFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_ERROR_HANDLE, SEP1, TYPE17_EH_NO_ERROR);
 	    strcat(file, line);
 	}
 	if(type17->error_handle < 0xFFFE)
 	{
 	    sprintf(line, "%-35s%s %s %d\n", TYPE17_ERROR_HANDLE, SEP1, TYPE17_EH_HANDLE, type17->error_handle);
 	    strcat(file, line);
 	}
 	
 	if(type17->total_width == 0xFFFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_TOTAL_WIDTH, SEP1, TYPE17_TW_UNKNOWN);
 	    strcat(file, line);
 	}
 	else
 	{
 	    sprintf(line, "%-35s%s %d %s\n", TYPE17_TOTAL_WIDTH, SEP1, type17->total_width, BIT);
 	    strcat(file, line);
 	}
 	
 	if(type17->data_width == 0xFFFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_DATA_WIDTH, SEP1, TYPE17_DW_UNKNOWN);
 	    strcat(file, line);
 	}
 	else
 	{
 	    sprintf(line, "%-35s%s %d %s\n", TYPE17_DATA_WIDTH, SEP1, type17->data_width, BIT);
 	    strcat(file, line);
 	}
 	
 	if(type17->size == 0)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_SIZE, SEP1, TYPE17_SIZE_NOT_INSTALLED);
 	    strcat(file, line);
 	}
 	if(type17->size == 0xFFFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_SIZE, SEP1, TYPE17_SIZE_UNKNOWN);
 	    strcat(file, line);
 	}
 	if((type17->size > 0) && (type17->size < 0xFFFF))
 	{
 	    if(type17->size & 0x8000)
 	    {
 	        sprintf(line, "%-35s%s %d %s\n", TYPE17_SIZE, SEP1, type17->size & 0x7FFF, KB);
 	        strcat(file, line);
 	    }
 	    else
 	    {
 	        sprintf(line, "%-35s%s %d %s\n", TYPE17_SIZE, SEP1, type17->size, MB);
 	        strcat(file, line);
 	    }
 	}
 	 	
    switch(type17->form_factor)
    {
 	    case 1:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_OTHER);
 	                break;
 	    case 2:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_UNKNOWN);
 	                break;
 	    case 3:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_SIMM);
 	                break;
 	    case 4:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_SIP);
 	                break;
 	    case 5:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_CHIP);
 	                break;
 	    case 6:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_DIP);
 	                break;
 	    case 7:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_ZIP);
 	                break;
 	    case 8:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_PROPRIETARY);
 	                break;
 	    case 9:     sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_DIMM);
 	                break;
 	    case 10:    sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_TSOP);
 	                break;
 	    case 11:    sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_ROW);
 	                break;
 	    case 12:    sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_RIMM);
 	                break;
 	    case 13:    sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_SODIMM);
 	                break;
 	    case 14:    sprintf(line, "%-35s%s %s\n", TYPE17_FORM_FACTOR, SEP1, TYPE17_FF_SRIMM);
 	                break;
 	    default:    sprintf(line, "%-35s%s %d\n", TYPE17_FORM_FACTOR, SEP1, type17->form_factor);
 	}
 	strcat(file, line);
 	
 	if(type17->device_set == 0)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_DEVICE_SET, SEP1, TYPE17_DEVICE_SET_NOT_IN_SET);
 	    strcat(file, line);
 	}
 	if(type17->device_set == 0xFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE17_DEVICE_SET, SEP1, TYPE17_DEVICE_SET_UNKONWN);
 	    strcat(file, line);
 	}
 	if((type17->device_set > 0) && (type17->device_set < 0xFF))
 	{
 	    sprintf(line, "%-35s%s %d\n", TYPE17_DEVICE_SET, SEP1, type17->device_set);
 	    strcat(file, line);
 	}
 	
 	sprintf(line, "%-35s%s %s\n", TYPE17_DEVICE_LOCATOR, SEP1, GetString(smbiosstruct, (unsigned int)(type17->device_locator)));
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %s\n", TYPE17_BANK_LOCATOR, SEP1, GetString(smbiosstruct, (unsigned int)(type17->bank_locator)));
 	strcat(file, line);
 	
 	switch(type17->mem_type)
    {
 	    case 1:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_OTHER);
 	                break;
 	    case 2:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_UNKNOWN);
 	                break;
 	    case 3:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_DRAM);
 	                break;
 	    case 4:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_EDRAM);
 	                break;
 	    case 5:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_VRAM);
 	                break;
 	    case 6:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_SRAM);
 	                break;
 	    case 7:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_RAM);
 	                break;
 	    case 8:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_ROM);
 	                break;
 	    case 9:     sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_FLASH);
 	                break;
 	    case 10:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_EEPROM);
 	                break;
 	    case 11:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_FEPROM);
 	                break;
 	    case 12:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_EPROM);
 	                break;
 	    case 13:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_CDRAM);
 	                break;
 	    case 14:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_3DRAM);
 	                break;
 	    case 15:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_SDRAM);
 	                break;
 	    case 16:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_SGRAM);
 	                break;
 	    case 17:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_RDRAM);
 	                break;
 	    case 18:    sprintf(line, "%-35s%s %s\n", TYPE17_MEM_TYPE, SEP1, TYPE17_MT_DDR);
 	                break;
 	    default:    sprintf(line, "%-35s%s %d\n", TYPE17_MEM_TYPE, SEP1, type17->mem_type);
 	}
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s\n", TYPE17_TYPE_DETAIL, SEP1);
    strcat(file, line);
    if( (type17->type_detail) & 0x0001 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_RESERVED);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0002 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_OTHER);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0004 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_UNKNOWN);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0008 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_FAST_PAGE);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0010 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_STATIC_COLUMN);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0020 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_PSEUDO_STATIC);
        strcat(file, line);
    }
 	if( (type17->type_detail) & 0x0040 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_RAMBUS);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0080 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_SYNCHRONOUS);
        strcat(file, line);
    }
 	if( (type17->type_detail) & 0x0100 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_CMOS);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0200 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_EDO);
        strcat(file, line);
    }
 	if( (type17->type_detail) & 0x0400 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_WINDOW_DRAM);
        strcat(file, line);
    }
    if( (type17->type_detail) & 0x0800 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_CACHE_DRAM);
        strcat(file, line);
    }
 	if( (type17->type_detail) & 0x1000 )
    {
        sprintf(line, "%-35s  %s %s \n", "", SEP2, TYPE17_TD_NON_VOLATILE);
        strcat(file, line);
    }

    if(smbiosstruct->length > 0x15)
    {
        if(type17->speed > 0)
        {
            sprintf(line, "%-35s%s %d %s\n", TYPE17_SPEED, SEP1, type17->speed, MHZ);
            strcat(file, line);
        }
    }

    if(smbiosstruct->length > 0x17)
    {
        sprintf(line, "%-35s%s %s\n", TYPE17_MANUFACTURER, SEP1, GetString(smbiosstruct, (unsigned int)(type17->manufacturer)));
 	    strcat(file, line);
 	
 	    sprintf(line, "%-35s%s %s\n", TYPE17_SERIAL_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type17->serial_number)));
 	    strcat(file, line);
 	
 	    sprintf(line, "%-35s%s %s\n", TYPE17_ASSET_TAG, SEP1, GetString(smbiosstruct, (unsigned int)(type17->asset_tag)));
 	    strcat(file, line);
 	
 	    sprintf(line, "%-35s%s %s\n", TYPE17_PART_NUMBER, SEP1, GetString(smbiosstruct, (unsigned int)(type17->part_number)));
 	    strcat(file, line);
    }
 	

    /* now line contains a string with fully interpreted type 17 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_19 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 19 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 19 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 19 (Memory Array Mapped Address) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_19 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_19 *type19;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE19_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 19 */
    type19 = (smbios_type_19 *)smbiosstruct;
 	
 	sprintf(line, "%-35s%s %.8X h\n", TYPE19_STARTING_ADR, SEP1, type19->starting_adr);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %.8X h\n", TYPE19_ENDING_ADR, SEP1, type19->ending_adr);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %d\n", TYPE19_MEMORY_ARRAY_HANDLE, SEP1, type19->mem_array_handle);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %d\n", TYPE19_PARTITION_WIDTH, SEP1, type19->partition_width);
 	strcat(file, line);
 	

    /* now line contains a string with fully interpreted type 19 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_20 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 20 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 20 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 20 (Memory Device Mapped Address) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_20 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_20 *type20;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE20_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 20 */
    type20 = (smbios_type_20 *)smbiosstruct;
 	
 	sprintf(line, "%-35s%s %.8X h\n", TYPE20_START_ADR, SEP1, type20->starting_adr);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %.8X h\n", TYPE20_END_ADR, SEP1, type20->ending_adr);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %d\n", TYPE20_MEM_DEV_HANDLE, SEP1, type20->mem_dev_handle);
 	strcat(file, line);
 	
 	sprintf(line, "%-35s%s %d\n", TYPE20_MEM_ARRAY_MAPPED_HANDLE, SEP1, type20->mem_array_mapped_adr_handle);
 	strcat(file, line);
 	
 	if(type20->partition_row_position == 0xFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE20_PARTITION_ROW_POSITION, SEP1, TYPE20_PRP_UNKNOWN);
 	    strcat(file, line);
 	}
 	if( (type20->partition_row_position > 0) && (type20->partition_row_position < 0xFF))
 	{
 	    sprintf(line, "%-35s%s %d\n", TYPE20_PARTITION_ROW_POSITION, SEP1, type20->partition_row_position);
 	    strcat(file, line);
 	}
 	
 	if(type20->interleave_position == 0)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE20_INTERLEAVE_POSITION, SEP1, TYPE20_IP_NOT_INTERLEAVED);
 	    strcat(file, line);
 	}
 	if(type20->interleave_position == 0xFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE20_INTERLEAVE_POSITION, SEP1, TYPE20_IP_UNKNOWN);
 	    strcat(file, line);
 	}
 	if( (type20->interleave_position > 0) && (type20->interleave_position < 0xFF))
 	{
 	    sprintf(line, "%-35s%s %d\n", TYPE20_INTERLEAVE_POSITION, SEP1, type20->interleave_position);
 	    strcat(file, line);
 	}
 	
 	if(type20->interleaved_data_depth == 0)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE20_INTERLEAVED_DATA_DEPTH, SEP1, TYPE20_IDD_NOT_INTERLEAVED);
 	    strcat(file, line);
 	}
 	if(type20->interleaved_data_depth == 0xFF)
 	{
 	    sprintf(line, "%-35s%s %s\n", TYPE20_INTERLEAVED_DATA_DEPTH, SEP1, TYPE20_IDD_UNKNOWN);
 	    strcat(file, line);
 	}
 	if( (type20->interleaved_data_depth > 0) && (type20->interleaved_data_depth < 0xFF))
 	{
 	    sprintf(line, "%-35s%s %d\n", TYPE20_INTERLEAVED_DATA_DEPTH, SEP1, type20->interleaved_data_depth);
 	    strcat(file, line);
 	}
 	

    /* now line contains a string with fully interpreted type 20 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_32 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 32 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 32 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 32 (System Boot Information) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_32 (smbios_struct * smbiosstruct, unsigned int *plength)
{
	smbios_type_32 *type32;
    unsigned char * scratch;
	
	/* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];


    /* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE32_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    /* cast our data ptr to a structure ptr of SMBIOS type 32 */
    type32 = (smbios_type_32 *)smbiosstruct;
 	
 	switch(type32->status)
 	{
 	    case 0:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_NO_ERROR);
 	                break;
 	    case 1:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_NO_BOOT_MEDIA);
 	                break;
 	    case 2:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_NO_OS);
 	                break;
 	    case 3:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_FIRMWARE_HW);
 	                break;
 	    case 4:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_OS_HW);
 	                break;
 	    case 5:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_USER);
 	                break;
 	    case 6:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_SECURITY);
 	                break;
 	    case 7:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_REQUEST);
 	                break;
 	    case 8:     sprintf(line, "%-35s%s %s\n", TYPE32_STATUS, SEP1, TYPE32_STATUS_WATCHDOG);
 	                break;
 	    default:    sprintf(line, "%-35s%s %d\n", TYPE32_STATUS, SEP1, type32->status); 	
 	}
 	strcat(file, line);
 	
 	
 	/* now line contains a string with fully interpreted type 32 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn unsigned char * bios_cook_type_127 (smbios_struct *smbiosstruct, unsigned int * plength)
  * \brief writes interpreted SMBIOS Type 127 data to a /proc file
  * \param smbiosstruct pointer to SMBIOS Type 127 raw structure
  * \param plength amount of memory allocated by this function
  * \return pointer to string that holds the interpreted data
  *
  * this function gets a raw SMBIOS Type 127 (System Boot Information) structure. it
  * interpretes the raw data and builds a string with the interpreted
  * data. this is the return value of this structure. the caller is
  * responsible to free the allocated memory.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned char *
bios_cook_type_127 (smbios_struct * smbiosstruct, unsigned int *plength)
{
    unsigned char * scratch;

    /* contains the full block of interpreted data */
    /* on some systems the system crashed if the stack (local variables) */
    /* is bigger than one pages (4k). since linux needs some space in this */
    /* page the variables should never exceed 3kB. */
    unsigned char file[2800];

    /* contains one line of the above file */
    unsigned char line[128];


	/* prepare header strings */
    sprintf (line, "%-35s%s %d %s\n", TYPE, SEP1, smbiosstruct->type, TYPE127_NAME);
	strcpy(file, line);
	sprintf (line, "%-35s%s %d %s\n", LENGTH, SEP1, smbiosstruct->length, BYTES);
	strcat(file, line);
	sprintf (line, "%-35s%s %d\n\n", HANDLE, SEP1, smbiosstruct->handle);
	strcat(file, line);
 	
    sprintf (line, "%-35s%s %s\n\n", TYPE127_DESC, SEP1, TYPE127_DESC_TEXT);
 	strcat(file, line);
 	
 	
 	/* now line contains a string with fully interpreted type 32 data */
	*plength = strlen(file);

	/* allocate memory */
	scratch = kmalloc (*plength+1, GFP_KERNEL);
	if (scratch == NULL)
	{
		*plength = 0;
		return NULL;
	}

	/* copy the interpreted data */
	memcpy (scratch, file, *plength);

	/* return a string with all the interpreted data for the given raw structure. */
	/* the caller is responsible to free the memory. */	
	return scratch;
}


/** \fn char * GetString (smbios_struct *struct_ptr, unsigned int stringnr)
  * \brief gets the n-th string from a given SMBIOS structure
  * \param smbiosstruct pointer to SMBIOS raw structure
  * \param stringnr number of the string to be returned (1-based)
  * \return pointer to string that holds the desired string
  *
  * a string number of zero indicates that there is no string to fetch.
  * this function jumps to the end of the structure and walks through the
  * strings behind it. the strings are seperated by 0x00. a double 0x00
  * indicates the end of the strings section.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */


char *
GetString(smbios_struct *struct_ptr, unsigned int stringnr)
{
	unsigned int str_stringnr = 0;
	unsigned char *ptr = NULL;


    /* if we should get string nr 0, we have nothing to do */
	if (stringnr == str_stringnr)
        return NULL;

	str_stringnr = 1;

    /* jump to string list */
    ptr = ((unsigned char *) struct_ptr) + (struct_ptr->length);

    /* double 0x00 indicates we reached the end of the string list */
    if(ptr[0] == 0x00 && ptr[1] == 0x00) return NULL;

    while(str_stringnr < stringnr)		/* as long as we have not found our string */
	{
	    ptr = ptr + strlen(ptr) + 1;
        if(ptr[0] == 0x00)
            return NULL;	/* this means we have a double 0x00 and have reached the end of the string list */
        str_stringnr++;
	}

    return (char *) ptr;

}


/** \fn unsigned int pow2 (unsigned int n)
  * \brief returns the n-th power of 2 (2*2*2...)
  * \param n Indicates how often we should multiply 2 with itself
  * \return n-th power of 2
  *
  * if n=0 the  return value will be 1. Otherwise 2 is multiplied n-times
  * with itself.
  *
  * \author Joachim Braeuer
  * \date March 2001
  */

unsigned int
pow2(unsigned int n)
{
    unsigned int retval;


    if (n == 0)
        return 1;

    retval = 1;

    while (n > 0)
    {
        retval = retval * 2;
        n--;
    }

    return retval;
}
