TARGET = DynarecTest

OBJS = \
Dynarec.o \
main.o 

CC=psp-g++
CXX=psp-g++


PSPSDK = $(shell psp-config --pspsdk-path)
PSPDEV = $(shell psp-config -d)

CFLAGS = -O2 -finline-functions -fno-rtti -ffast-math -fweb -fomit-frame-pointer -G0 -Wall

CXXFLAGS = $(CFLAGS) -fno-exceptions

ASFLAGS = $(CFLAGS) 

LIBS  =  -lpthread-psp -lpsppower -lfreetype -lpng -lz -lm -lpsphprm -lpspaudio -lstdc++ -lpsprtc -lvorbisidec -lpspsdk

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = DynarecTest

include $(PSPSDK)/lib/build.mak

#PSP_EBOOT_ICON = ICON0.png
#PSP_EBOOT_PIC1 = PIC1.png
#PSP_EBOOT_SND0 = SND0.at3
