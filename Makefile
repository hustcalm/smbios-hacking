# Comment/uncomment the following line to disable/enable debugging
DEBUG = n 

CC = gcc
INCLUDEDIR = /usr/includei/linux

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -D_DEBUG_ # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

CFLAGS = -D__KERNEL__ -DMODULE -Wall -Wstrict-prototypes -fomit-frame-pointer \
         -fno-strict-aliasing -pipe -mpreferred-stack-boundary=2 $(DEBFLAGS)
CFLAGS += -I$(INCLUDEDIR)

# Extract version number from headers.
VER = `awk -F\" '/REL/ {print $$2}' $(INCLUDEDIR)/linux/version.h`

TARGET = smbios
OBJS = $(TARGET).o
SRC = bios.c main.c cooking.c

all: .depend $(TARGET).o


$(TARGET).o: $(SRC:.c=.o)
	$(LD) -r $^ -o $@

install:
	mkdir -p /lib/modules/$(VER)/misc
	install -c -m 644 $(TARGET).o /lib/modules/$(VER)/misc

clean:
	rm -f *.o *~ core .depend

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > $@


ifeq (.depend,$(wildcard .depend))
include .depend
endif
