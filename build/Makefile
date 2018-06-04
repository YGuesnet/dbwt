INCDIR = -I$(SRCDIR) $(MY_INCDIR)
# -Wconversion non supporté par sais.hxx ?
CXXFLAGS = -std=c++11 -Wpedantic -Wall -Wextra -Werror $(INCDIR) -g $(MY_CXXFLAGS) -O3
LDLIBS = -lsdsl -lboost_program_options -O3 
SRCDIR = ../src
BUILDDIR = .

OBJS = dsbwt.o generator.o \
       range_tree.o range.o \
       acgt_multiletter.o \
       datatools.o

all: ../generator ../dsbwt

-include $(OBJS:.o=.d)

../dsbwt: dsbwt.o datatools.o acgt_multiletter.o range_tree.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

../generator: generator.o datatools.o acgt_multiletter.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

dsbwt.o: $(SRCDIR)/dsbwt.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(COMPILE.cpp) -MM $< > $*.d

generator.o: $(SRCDIR)/generator.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(COMPILE.cpp) -MM $< > $*.d

acgt_multiletter.o: $(SRCDIR)/multiletter/acgt_multiletter.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(COMPILE.cpp) -MM $< > $*.d

range_tree.o: $(SRCDIR)/trees/range_tree.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(COMPILE.cpp) -MM $< > $*.d

datatools.o: $(SRCDIR)/datatools.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(COMPILE.cpp) -MM $< > $*.d

clean:
	$(RM) *.o *.d

