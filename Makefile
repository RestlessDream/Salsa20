.PHONY: all clean build test run_tests

CC = gcc
SRC = src
TARGET := ./build
DIRS := . $(SRC)/ $(wildcard $(SRC)/*/)
TEST_DIRS := test/
CFLAGS := -Wall -O3 $(addprefix -I, $(DIRS) $(TEST_DIRS)) -g
MAIN := main
NAME := salsa20
TEST_DEP_CPP := $(wildcard $(addsuffix *.c, $(TEST_DIRS)))
TESTS := $(filter-out unity, $(notdir $(basename $(TEST_DEP_CPP))))
DEP_CPP := $(wildcard $(addsuffix *.c, $(DIRS)))
DEP_OBJ := $(addsuffix .o, $(basename $(notdir $(DEP_CPP))))
TEST_DEP_OBJ := $(addsuffix .o, $(basename $(notdir $(TEST_DEP_CPP))))
HEADERS := $(wildcard $(addsuffix *.h, $(DIRS)))

run_tests: $(TESTS)
	eval $(addprefix ; ./, $(TESTS))

all : build test

build : $(NAME) $(HEADERS)

test : $(TESTS) $(HEADERS)

$(NAME): $(DEP_OBJ)
	$(CC) $(CFLAGS) -o $@ $(addprefix $(TARGET)/, $^)

$(TESTS): $(filter-out $(MAIN).o, $(DEP_OBJ)) $(TEST_DEP_OBJ)
	$(CC) $(CFLAGS) -o $@ $(addprefix $(TARGET)/, $(filter-out $(filter-out unity.o, $(filter-out $@.o, $(TEST_DEP_OBJ))), $^))

%.o: 
	$(CC) $(CFLAGS) -c -o $(TARGET)/$@ $(filter %/$(basename $@).c, $(DEP_CPP) $(TEST_DEP_CPP))

%.h: ;

clean:
	rm -f $(addprefix $(TARGET)/, $(DEP_OBJ)) $(addprefix $(TARGET)/, $(TEST_DEP_OBJ)) $(NAME) $(TESTS)
