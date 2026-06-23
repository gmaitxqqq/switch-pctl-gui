DEVKITPRO  ?= /opt/devkitpro
DEVKITA64  := $(DEVKITPRO)/devkitA64
PORTLIBS   := $(DEVKITPRO)/portlibs/switch
LIBNX      := $(DEVKITPRO)/libnx

CC       = aarch64-none-elf-gcc
CXX      = aarch64-none-elf-g++
CFLAGS  := -g -Wall -O2 \
           -march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIE \
           -I$(LIBNX)/include -I$(PORTLIBS)/include \
           -I$(DEVKITA64)/include -Iinclude \
           -Iexternals/borealis/include
CXXFLAGS := $(CFLAGS) -std=c++17 -fno-exceptions -fno-rtti
LDFLAGS := -g -specs=$(DEVKITA64)/switch.specs -march=armv8-a \
           -mtune=cortex-a57 -mtp=soft -fPIE \
           -L$(LIBNX)/lib -L$(PORTLIBS)/lib -L$(DEVKITA64)/lib \
           -lnx -lborealis -lEGL -lglapi -ldrm_nouveau -lnvapi_lite \
           -lnvbuf -lpthread

# Source files
C_SRCS   := $(wildcard source/*.c)
CXX_SRCS := $(wildcard source/*.cpp)
OBJS     := $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)

# Output
TARGET    := switch-pctl-gui.nro

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "  LINK  $@"
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	@echo "  CXX   $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	@echo "  CC    $<"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(DEVKITPRO)/switch/
