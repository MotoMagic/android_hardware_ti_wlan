

##
##
## Directory Locations
##
##

COMMON      := $(DK_ROOT)/stad
COMSRC      := $(COMMON)/src
TWD         := $(DK_ROOT)/TWD
UTILS       := $(DK_ROOT)/utils
TESTSRC     := $(DK_ROOT)/Test
LINUX_PFORM := $(DK_ROOT)/platforms/os/linux
PLATFORMS   := $(DK_ROOT)/platforms
EXT_DRV     := $(DK_ROOT)/external_drivers
EXT_APP     := $(DK_ROOT)/external_apps
TXN         := $(DK_ROOT)/Txn

##
##
## Make Flags
##
##

#
# Compile with FW1273 or FW1251
#
FW ?= 1273

#
# Compile with XCC support or not
#
XCC ?= n


#
# Compile with GEM support or not
#
GEM ?= n

#
# Choose the bus type (n for SDIO, y for WSPI)
#
WSPI ?= n

#
# Use periodic interrupt
#
INTR ?= y


#
# Use hostapd_cli static library
#
TI_HOSTAPD_LIB ?= n


#
# Enable or disable debugging
#
DEBUG ?= y
KERNEL_DEBUGGER ?= n
STRIP ?= y
BMTRACE ?= n
#
# Enable or disable OS and init messages
#
INFO ?= n

#
# TNETW chip version
#
TNETW ?= 1283

#
# Enable trace buffer
#
TI_TRACE_BUFFER ?= n


#
# Enable AP MODE
#
AP_MODE_ENABLED ?= y



##
##
## Driver Compilation Directives
##
##
ifeq ($(TI_HOSTAPD_LIB),y)
    DK_DEFINES += -D TI_HOSTAPD_LIB
endif


ifeq ($(AP_MODE_ENABLED),y)
    DK_DEFINES += -D AP_MODE_ENABLED
    DK_DEFINES += -D AP_DATA_PATH_DEBUG  # Temp debug flag - will be removed after data path integration completion
endif

ifeq ($(XCC),y)
    DK_DEFINES += -D XCC_MODULE_INCLUDED
endif

ifeq ($(GEM),y)
    DK_DEFINES += -D GEM_SUPPORTED
endif

ifeq ($(INTR),n)
    DK_DEFINES += -D PRIODIC_INTERRUPT
endif    

ifeq ($(DEBUG),y)
    DK_DEFINES += -D TI_DBG
    DK_DEFINES += -D REPORT_LOG
    DK_DEFINES += -O2 
else
    DK_DEFINES += -U REPORT_LOG
    DK_DEFINES += -O2
endif
ifeq ($(KERNEL_DEBUGGER),y)
    DK_DEFINES += -g -O2
	STRIP = n
endif
export KERNEL_DEBUGGER

DK_DEFINES += -D __BYTE_ORDER_LITTLE_ENDIAN

ifeq ($(TI_TRACE_BUFFER),y)
   DK_DEFINES += -D TI_TRACE_BUF
endif

ifeq ($(BMTRACE),y)
   DK_DEFINES += -D TIWLAN_BMTRACE
endif

# Flag to support level based interrupt in OMAP
DK_DEFINES += -D OMAP_LEVEL_INT

##
##
## Platform Compilation Directives
##
##

#
# Bus Driver
#
ifeq ($(WSPI),y)
    BUS_DRV = spi
    BUS_DRV_REMOVE = sdio
    BUS_DRIVER_MODULE = spi.ko
    PFORM_DEFINES += -D WSPI
else
    BUS_DRV = sdio
    BUS_DRV_REMOVE = spi
    BUS_DRV = sdio
    BUS_DRV_REMOVE = spi
    BUS_DRIVER_MODULE = sdio.ko
endif

ifeq ($(INFO),y)
    PFORM_DEFINES += -D OS_INFO_MESSAGES
    PFORM_DEFINES += -D INIT_MESSAGES
endif

PFORM_DEFINES += -D HOST_COMPILE
PFORM_DEFINES += -D FW_RUNNING_AS_STA
ifeq ($(TNETW),1273)
    PFORM_DEFINES += -D TNETW1273
endif

ifeq ($(TNETW),1283)
    PFORM_DEFINES += -D TNETW1283
endif

ifeq ($(FULL_ASYNC),y)
    PFORM_DEFINES += -D FULL_ASYNC_MODE
endif

ifeq ($(USE_IRQ_ACTIVE_HIGH),y)
    PFORM_DEFINES += -D USE_IRQ_ACTIVE_HIGH
endif

##
##
## Miscellaneous Compilation Directivcs
##
##

EXTRA_CFLAGS += -fsigned-char -Werror 
EXTRA_CFLAGS += -D __LINUX__
EXTRA_CFLAGS += $(DK_DEFINES)
EXTRA_CFLAGS += $(PFORM_DEFINES)
