CXX=g++
CXXFLAGS=-std=c++11
SRCDIR=src
OBJDIR=obj
TESTDIR=testes
ANALYSEDDIR=analysed

SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

main: $(OBJ)
	$(CXX) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) -c -o $@ $<

analyse: $(ANALYSEDDIR)
	for file in $(TESTDIR)/*; do \
		base=$$(basename $$file); \
		base_no_ext=$${base%.*}; \
		./analyse $$file $(ANALYSEDDIR)/$${base_no_ext}.txt; \
	done

$(ANALYSEDDIR):
	@mkdir -p $(ANALYSEDDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) main $(ANALYSEDDIR)
