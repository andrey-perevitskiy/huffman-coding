CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lncurses
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
BIN = $(BINDIR)/prog

SRCS = $(shell find $(SRCDIR) -name '*.c')
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DIRS = $(sort $(dir $(OBJS)))

.PHONY: all
all: $(DIRS) $(BINDIR) $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINDIR):
	@[ -d $@ ] || mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIRS):
	@[ -d $@ ] || mkdir $@

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)
