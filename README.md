smbios-hacking
==============

A Hacking About Linux Kernel Module - SMBIOS

## Overview
Honestly speaking, this package is old, dated back to 2000:)

But useful, it's a good entry to pry into the world of BIOS.

Many thanks to the authors, Markus Lyra, Thomas Bretthauer and Joachim Braeuer, who were working at Fujitsu Siemsens.

## Main Files
### strgdef.h
**String definitions for cooked text.**

This file contains the string definitions for human readable output
(directory names, file names, SMBios structure names, attribute names
and attribute values and enums).

### bios.h
**declarations and prototypes of DMI-BIOS and SM-BIOS stuff.**

### bios.c
**DMI-BIOS and SM-BIOS access functions.**

The functions defined in this file provide low level access to the BIOS
area that is comform to the DMI BIOS Specification or the SM-BIOS
specification, respectively.

### cooking.h
**declarations and prototypes for interpreting DMI-BIOS and SM-BIOS types.**

### cooking.c
**DMI-BIOS and SM-BIOS interpreting (cooking) functions.**

The functions defined in this file generate a human readable output of the
raw binary DMI- and SM-BIOS types. However, every time the specification of
DMI- and SM-BIOS (types) change, these changes have to be adopted in these
routines, too.

### main.c
**kernel interface functions for smbios kernel module.**

## Prerequirements
* Knowledge about BIOS
* Knowing about ioremap/iounmap
* Basic C programming skills


