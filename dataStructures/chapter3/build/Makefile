.PHONY: all clean

ROOT = $(realpath ..)

DIRS = $(ROOT)/code/linearStack/src \
	$(ROOT)/code/linkStack/src \
	$(ROOT)/code/digitConv/src \
	$(ROOT)/code/verifyBrackets/src \
	$(ROOT)/code/lineEditor/src \
	$(ROOT)/code/mazePath/src \
	$(ROOT)/code/expressEval/src \
	$(ROOT)/code/hanoi/src \
	$(ROOT)/code/linkQueue/src \
	$(ROOT)/code/loopQueue/src \
	$(ROOT)/code/bankSimulation/src \
	$(ROOT)/code/chapter3/src

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
	@echo ";-) Completed"
	@echo ""

#@echo "make[1]: Entering directory '$$dir'"
#@echo "make[1]: Leaving directory '$$dir'"
