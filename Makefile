############################### Customise #####################################
CURDIR := $(shell pwd)
src_dir := $(CURDIR)/src
inc_dir := $(CURDIR)/include
build_dir := $(CURDIR)/build
doc_dir := $(CURDIR)/docs
target := out

############################### Variables #####################################

CXX := g++
CXXFLAGS := -I$(inc_dir) -I$(SYSTEMC_HOME)/include
CXXFLAGS += -I$(VCML_HOME)/include
ifdef debug
CXXFLAGS += -g -Wall -DDEBUG
else
CXXFLAGS += -O3
endif
LDLIBS := -lvcml -lmwr
LDLIBS += -lm -lsystemc
LDFLAGS := -L$(VCML_HOME)/lib
LDFLAGS += -L$(SYSTEMC_HOME)/lib-linux64

depdir := $(build_dir)/.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(depdir)/$*.d

COMPILE.c = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

srcs := $(foreach dir, $(src_dir), $(wildcard $(dir)/*.cpp))
objs := $(addprefix $(build_dir)/, $(addsuffix .o, $(notdir $(basename $(srcs)))))

vpath %.cpp $(src_dir)
vpath %.h $(inc_dir)

############################### Rules #########################################

.PHONY: all rebuild clean doc

all: $(build_dir)/$(target)

rebuild: clean all

doc:
	doxygen Doxyfile

$(build_dir)/$(target): $(objs)
	$(CXX) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(build_dir)/%.o: %.cpp $(depdir)/%.d | $(depdir)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(depdir): ; @mkdir -p $@

DEPFILES := $(addprefix $(depdir)/, $(addsuffix .d, $(notdir $(basename $(srcs)))))
$(DEPFILES):
include $(wildcard $(DEPFILES))

clean:
	$(RM) -rf $(build_dir)
	$(RM) -rf $(doc_dir)/*
