CC:=gcc
SRC_DIR ?=sources
HDR_DIR ?=headers
OBJ_DIR ?=bin
DEBUG ?=1

ifeq '$(DEBUG) ' '1 '
CFLAGS ?= -Wall -MMD -g
else
CFLAGS ?= -Wall -MMD -O3 -DNDEBUG
endif


LDFLAGS =
OBJS := $(shell find $(SRC_DIR) -name "*.c"|sed 's/.c$$/.o/g'|sed 's/$(SRC_DIR)/$(OBJ_DIR)/g')
DEPS := $(OBJS : .o = .d)
TARGET ?= exec

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

.PHONY: move

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDR_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

move:
	mv *.o bin
clean:
	rm -rf $(OBJ_DIR)

mrproper: clean
	rm -f $(TARGET)

-include $(DEPS)