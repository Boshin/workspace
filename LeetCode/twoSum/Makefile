.PHONY: all clean

UNAME    	:= $(shell uname)
HOST 		:= $(shell hostname -s)

MKDIR = mkdir
RM = rm
RMFLAGS = -rf

ifneq ($(UNAME), Linux)
CC = g++
else 
CC = g++-4.8
endif

FLAG = -Wall -g -std=c++11

CC := $(CC) $(FLAG)

DIR_OBJS = objs
DIR_EXES = exes
DIR_DEPS = deps
DIRS = $(DIR_OBJS) $(DIR_EXES) $(DIR_DEPS)

EXE = complicated.elf
EXE := $(addprefix $(DIR_EXES)/, $(EXE))

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))
DEPS = $(SRCS:.cpp=.dep)
DEPS := $(addprefix $(DIR_DEPS)/, $(DEPS))

ifeq ("$(wildcard $(DIR_OBJS))", "")
DEP_DIR_OBJS := $(DIR_OBJS)
endif
ifeq ("$(wildcard $(DIR_EXES))", "")
DEP_DIR_EXES := $(DIR_EXES)
endif
ifeq ("$(wildcard $(DIR_DEPS))", "")
DEP_DIR_DEPS := $(DIR_DEPS)
endif

all: $(EXE)

ifneq ($(MAKECMDGOALS), clean)
include $(DEPS)
endif

$(DIRS):
	$(MKDIR) $@
$(EXE): $(DEP_DIR_EXES) $(OBJS)
	$(CC) -o $@ $(filter %.o, $^)
$(DIR_OBJS)/%.o: $(DEP_DIR_OBJS) %.cpp
	$(CC) -o $@ -c $(filter %.cpp, $^)
$(DIR_DEPS)/%.dep: $(DEP_DIR_DEPS) %.cpp
	@echo "Creating $@ ..."
	@set -e; \
	$(RM) $(RMFLAGS) $@.tmp ; \
	$(CC) -E -MM $(filter %.cpp, $^) > $@.tmp ; \
	sed 's,\(.*\)\.o[ :]*,objs/\1.o $@: ,g' < $@.tmp > $@ ; \
	$(RM) $(RMFLAGS) $@.tmp
clean:
	$(RM) $(RMFLAGS) $(DIRS)
