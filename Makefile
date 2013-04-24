
APP	 = usuSat

SRCEXT   = cpp
SRCDIR   = .
OBJDIR   = obj
BINDIR   = build

SRCS    := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCDIRS := $(shell find . -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))

DEBUG    = -g
# INCLUDES = -I./nmealib/include -I./include
INCLUDES = 

# CFLAGS   = -Wall -ped6antic -c $(DEBUG) $(INCLUDES)
CFLAGS   = $(INCLUDES) -Wall -c $(DEBUG)

#Its a static library so maybe different l-option? <-- check
LDFLAGS  = -lpthread

ifeq ($(SRCEXT), cpp)
CC       = $(CXX)
CFLAGS  += --std=c++0x
# else
# CFLAGS  += -std=gnu99
endif

.PHONY: all clean distclean

all: $(BINDIR)/$(APP)

$(BINDIR)/$(APP): buildrepo $(OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@echo $(CC) $(OBJS) $(LDFLAGS) -o $@
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.$(SRCEXT)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@echo $(CC) $(CFLAGS) $< -o $@
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning ..."
	@$(RM) -r $(OBJDIR)
	@$(RM) -r $(BINDIR)

#    distclean: clean
# 	@echo "Cleaning NMEA-lib..."
# 	@cd nmealib; make clean;
	

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef


# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFLAGS) \
        $1
endef
