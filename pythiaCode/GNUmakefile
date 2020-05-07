# GNUmakefile for pythiaHBT
# -------------------------

PYTHIAHOME = path/to/pythia

# Check distribution (use local version first, then installed version).
ifneq ("$(wildcard $(PYTHIAHOME)/lib/libpythia8.*)","")
  PREFIX_LIB=$(PYTHIAHOME)/lib
  PREFIX_INCLUDE=$(PYTHIAHOME)/include
endif
CXX_COMMON:=-I$(PREFIX_INCLUDE) $(CXX_COMMON)
CXX_COMMON+= -L$(PREFIX_LIB) -Wl,-rpath,$(PREFIX_LIB) -lpythia8 -ldl 

CXX := g++

RM				=	rm -f
O               =   .o
SRCDIR			=	src
INCDIR			=	include
OBJDIR			=	obj

# --------------- Files involved ------------------

ifeq "$(MAIN)" ""
MAIN		=	main_Pythia_for_PCA.e
endif

MAINSRC		=	main_Pythia_for_PCA.cc

# -------------------------------------------------

OBJECTS				=	$(addprefix $(OBJDIR)/, $(addsuffix $O, \
						$(notdir $(basename $(SRC)))))

TARGET			=	$(MAIN)
INSTPATH		=	..

# --------------- Pattern rules -------------------

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

# -------------------------------------------------

.PHONY:		all mkobjdir clean distclean install target

all:		mkobjdir $(TARGET)

target:		mkobjdir $(TARGET)

help:
		@grep '^##' GNUmakefile

mkobjdir:	
		-@mkdir -p $(OBJDIR)

$(TARGET): $(MAINSRC) $(OBJECTS) $(PREFIX_LIB)/libpythia8.a
	$(CXX) $< $(OBJECTS) -o $@ -std=c++11 -g $(CXX_COMMON)

clean:		
		-rm -f $(OBJECTS)

distclean:	
		-rm -f $(TARGET)
		-rm -f $(OBJECTS)

install:	$(TARGET)
		cp $(TARGET) $(INSTPATH)

# --------------- Dependencies -------------------

