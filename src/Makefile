CFLAGS = -lmenuw \
	-lformw \
	-lncursesw
OBJDIR = obj
BIN = ckana

SRC := $(wildcard *.c **/*.c)
OBJ := $(SRC:.c=.o)

.PHONY: all install

all: prebuild target

prebuild:
	mkdir -p $(OBJDIR)

target: $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) $(addprefix $(OBJDIR)/,$(notdir $(OBJ)))

%.o: %.c
	$(CC) -c -o $(OBJDIR)/$(notdir $@) $^

install:
	install -D -t $(PREFIX)/bin $(BIN)
