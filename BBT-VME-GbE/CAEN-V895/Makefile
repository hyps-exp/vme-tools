CXXFLAGS	+= -Wall -O2 -MMD -I../lib/
LIBS		+= -L../lib -lBBT-VME-GbE -Wl,-rpath,../lib

target		= v895
object		= v895.o
deps		= $(object:.o=.d)

all: $(target)

$(target): $(object)
	@echo === Linking $@ ...
	$(CXX) -o $@ $^ $(LIBS)

-include $(deps)

.cc.o:
	@echo === Compiling $< ...
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(target) $(object) $(deps) *~
