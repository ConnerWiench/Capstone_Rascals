# Include the nanopb provided Makefile rules
#include ../../extra/nanopb.mk

# Compiler flags to enable all warnings & debug info
CFLAGS = -Wall -Werror -g -O0
#CFLAGS += "-I$(NANOPB_DIR)"
CC = g++

# C source code files that are required
CSRC  = radiation2Q.cpp                # The main program
CSRC += rad.pb.c                	# The compiled protocol definition
CSRC += repeated_serialization.c    # The custom encode / decode
CSRC += pb_encode.c   				# The nanopb encoder
CSRC += pb_decode.c   				# The nanopb decoder
CSRC += pb_common.c   				# The nanopb common parts

# Build rule for the main program
rad: $(CSRC)
	$(CC) $(CFLAGS) -orad2 $(CSRC)

# Clean
clean:
	rm -f rad $(GENERATED_SRC)
	rm -rf $(OUT_DIR)
