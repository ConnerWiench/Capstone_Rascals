# Include the nanopb provided Makefile rules
include ../../extra/nanopb.mk

# Compiler flags to enable all warnings & debug info
CFLAGS = -Wall -Werror -g -O0
CFLAGS += "-I$(NANOPB_DIR)"
CC = g++

# C source code files that are required
CSRC  = radiation.cpp                   # The main program
CSRC += radQueue.h					#The compiled queue library
CSRC += rad.pb.c                	# The compiled protocol definition
CSRC += $(NANOPB_DIR)/pb_encode.c  # The nanopb encoder
CSRC += $(NANOPB_DIR)/pb_decode.c  # The nanopb decoder
CSRC += $(NANOPB_DIR)/pb_common.c  # The nanopb common parts

# Build rule for the main program
rad: $(CSRC)
	$(CC) $(CFLAGS) -orad $(CSRC)

# Build rule for the protocol
rad.pb.c: rad.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. rad.proto
