# Project Name
TARGET = FeedbackSynth

C_DEFS = \
	-DUSE_ARM_DSP

C_INCLUDES = \
	-ISource/

C_SOURCES = \
	lib/libDaisy/Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.c \
	lib/libDaisy/Drivers/CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.c

CPP_SOURCES = \
	FeedbackSynth_main.cpp \
	Source/BiquadFilters.cpp \
	Source/FeedbackSynthEngine.cpp \
	Source/KarplusString.cpp

LIBDAISY_DIR = lib/libDaisy
DAISYSP_DIR = lib/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
