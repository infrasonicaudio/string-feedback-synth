# Project Name
TARGET = FeedbackSynth

# Includes
C_INCLUDES = \
	-ISource/

# Sources
CPP_SOURCES = \
	FeedbackSynth_main.cpp \
	Source/BiquadFilters.cpp \
	Source/FeedbackSynthEngine.cpp

# Library Locations
LIBDAISY_DIR = lib/libDaisy
DAISYSP_DIR = lib/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
