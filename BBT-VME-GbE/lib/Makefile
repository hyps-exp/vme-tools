CXXFLAGS 	+= -Wall -O2 -MMD

target		= libBBT-VME-GbE.a
objects		= bbt-vme-gbe.o
deps		= $(objects:.o=.d)

all: $(target)

$(target): $(objects)
	@echo === Archiving $@ ...
	ar cr $@ $^
	ranlib $@

-include $(deps)

.cc.o:
	@echo === Compiling $< ...
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(target) $(objects) $(deps) *~
