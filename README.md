# TRS-HARD disk emulator, aka 'FreHD'

This is a fork of the original FreHD project by Frederic Vecoven.  The original FreHD project can be found on Frederic's site [https://www.vecoven.com/trs80/trs80.html](https://www.vecoven.com/trs80/trs80.html).  The project has since been moved to github at [https://github.com/veco/FreHDv1](https://github.com/veco/FreHDv1) where the latest versions of everything can be found.

The FreHD emulates both the harddisk controller and the disk storage, plus adds many feature which enhance usability.  The FreHD emulates the original TRS-80 harddisk systems such that the original DOS's that supported hardisks should also work with the FreHD using their original harddisk drivers.

The FreHD consists of several components and the scope of the project is really quite amazing.  First there is hardware itself, which uses a PIC microcontroller for a majority of the function, an SD card for the actual storage, a PAL (programmable logic) for miscellaneous logic, and an RTC (realtime clock).  Software (firmware) embedded in the PIC emulates the harddrive and the additional features and provides access to the RTC.  The main purpose of this fork is to provide compatible alternative firmwares, although it goes a bit beyond that.  Although the intent isn't to improve the original project, some improvements and bugfixes have been provided back to the original project.

The equivalent of everything in the original project is included here but might be modified or improved.     My goal is to maintain as much similarity as possible to the original project including the directory structure.  The directory structure is as follows:

+ hw : Hardware files.
  - gal : Equations of the GAL and the compiled jedec file.
  - protel99 : PCB source (schematics and pcb), in Protel99 format for the original PCB design.  Since it is unlikely that anyone is using Protel99 nowadays this is unlikely to change.
  - kicad : PCB source (schematics and pcb), in KiCad format.  This was converted from the original Protel99 design using a conversion script.  It has since been slightly modified and is subject to change.

+ sw : Software files.
  - pic : PIC MPLAB/MPLABX project files, source code, and compiled hex code.
    - trs_hard : 
    - FatFS  : elm FatFS library, ported to PIC18

+ sw/z80
  - fredhd_rom : source code of the boot menu
  - fredhd_rom/rom-patches : patches for all TRS80 models
  - utils : FreHD utilities


To build the PIC software, you will need Microchip C18 compiler (free)
and MPLab (I used version 8.36). You will also need perl, which is
used to compute the CRC16 used by the bootloader. (If you modify the
source, you need to compile twice, the second time to include the
correct CRC in your hex file).

To build the Z80 code, you need zmac. We are using version 1.3, this
assembler is public domain (by Bruce Norskog and others).

To build the rom patches, you need 'asl'. It is available at:
http://john.ccac.rwth-aachen.de:8000/as/

This project started during the long evenings of December 2012, and I
am very proud to release it to the community today, May 12th 2013. 
Many thanks to Andrew Quinn, who beta-tested and motivated me to
finish this project, or at least make it usable :)

One last thing : you may modify, duplicate, enhance or do whatever you
want with the code. All I ask is to keep this README file. You must
make your modifications available (GPL). If you decide to build and
sell (modified) emulators, all I ask is an emulator for my TRS80, so I
can test your changes.


Enjoy !

-Frederic Vecoven (frederic@vecoven.com)


Last update: Thu Jan 23 15:19:26 2014


!!! github !!!
The code is now available on github. I don't have time to
update/polish the code, so contributions are very welcome.
