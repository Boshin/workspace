.PHONY: all clean

ROOT = $(realpath ..)

DIRS = $(ROOT)/code/XString/src \
	   $(ROOT)/code/chapter4/src

RM = rm
RMFLAGS = -rf
RMS = $(ROOT)/build/exes $(ROOT)/build/libs

all clean:
	@set -e; \
	for dir in $(DIRS); \
	do \
		cd $$dir && $(MAKE) -r ROOT=$(ROOT) $@; \
	done
	@set -e; \
	if [ "$(MAKECMDGOALS)" = "clean" ]; then $(RM) $(RMFLAGS) $(RMS); fi
	@echo ""
	@echo ":-) Completed"
	@echo ""
