CXXFLAGS	+= -Wall -O2 -MMD -I../lib/
CXXFLAGS	+= -std=c++11
LIBS		+= -L../lib -lBBT-VME-GbE -Wl,-rpath,../lib -lcurl

src_dir         = src
bin_dir         = bin
target          = $(bin_dir)/v895
object          = $(src_dir)/v895.o
deps		= $(object:.o=.d)

all: $(target)

$(bin_dir):
	@mkdir -p $(bin_dir)

$(target): $(object) | $(bin_dir)
	@echo === Linking $@ ...
	$(CXX) -o $@ $^ $(LIBS)

-include $(deps)

$(src_dir)/%.o: $(src_dir)/%.cc
	@echo === Compiling $< ...
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(target) $(object) $(deps) *~
