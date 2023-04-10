CC = gcc
TARGET_NAME = librconf

SRC_DIR = ./src/
OBJ_DIR = ./obj/

SRC = $(wildcard $(SRC_DIR)*.c)
OBJ = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

CSTAND = gnu99
CFLAGS = -std=$(CSTAND) -Wall
FPIC = -fpic

ifdef $(static)
	$(FPIC) =  
endif
ifdef $(shared)
	$(FPIC) = -fpic
endif

# Use only this flags order : gcc <flags> <objs> < -l lib> -o <out>
# $(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

static : $(OBJ)
	rm -rf ./build/static
	mkdir ./build/static
	ar r ./build/static/$(TARGET_NAME).a $(OBJ)

shared : $(OBJ)
	rm -rf ./build/shared
	mkdir ./build/shared
	$(CC) -shared -o ./build/shared/$(TARGET_NAME).so $(OBJ)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	echo "Compiling objs..."
	$(CC) $(CFLAGS) $(FPIC) -c $< -o $@

install_shared:
ifeq (,$(wildcard ./build/shared/$(TARGET_NAME).so))
	# [-] Make build before install
else
	# [+] Builded
	cp ./build/shared/$(TARGET_NAME).so /usr/lib/
	chmod 0755 /usr/lib/$(TARGET_NAME).so
	cp ./include/librconf.h /usr/include/
endif

clean :
	rm -rf ./obj/*
	rm -rf ./build/*
