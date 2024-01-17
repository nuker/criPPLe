CC := x86_64-w64-mingw32-gcc

CFLAGS := -Wall -D_WIN64 -I /usr/share/mingw-w64/include/ddk/

LFLAGS := -lntoskrnl -lhal -s
LFLAGS := $(LFLAGS) -nostdlib -nostartfiles 
LFLAGS := $(LFLAGS) -Wl,--subsystem,native
LFLAGS := $(LFLAGS) -Wl,--entry,DriverEntry

SRC := src
INC := inc
OBJ := obj

SRC_FILES := $(wildcard $(SRC)/*.c)
OBJ_FILES := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRC_FILES))

DONE := H00.sys

$(DONE): $(OBJ_FILES)
	$(CC) $^ $(LFLAGS) -o $@

$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	$(CC) -c $< $(CFLAGS) -I inc -o $@

$(OBJ):
	mkdir $@

clean:
	rm -rf $(OBJ) H0*
