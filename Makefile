# compiler and flags
CC = gcc
CFLAGS = -Wall $(shell sdl2-config --cflags) 
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image

# directories
SRCDIR = src
BUILDDIR = build
INCDIR = include

# source files
SRCFILES = $(wildcard $(SRCDIR)/*.c)

# output binary name
TARGET = $(BUILDDIR)/pacman

# object files (compiled .o files)
OBJFILES = $(SRCFILES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# default rule
all: $(TARGET)

# link object files to create the final executable
$(TARGET): $(OBJFILES)
	$(CC) $(OBJFILES) $(LDFLAGS) -o $@  

# compile each source file to an object file
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

# clean 
clean:
	rm -rf $(BUILDDIR)

# phony targets
.PHONY: all clean
