CXX := clang++

SRCDIR := .
INCDIR := .
BUILDDIR := build
DOCDIR := docs
TARGET := chebyshov

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(INCDIR)
LDFLAGS := 
LIBS := 

ifeq ($(DEBUG),1)
    BUILDDIR := build/debug
    CXXFLAGS := -std=c++17 -Wall -Wextra -g -O0 -I$(INCDIR)
    TARGET := chebyshov_debug
endif

OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

all: $(TARGET)

debug:
	$(MAKE) DEBUG=1

release: $(TARGET)

clean:
	rm -rf build $(TARGET) chebyshov_debug
	find $(DOCDIR) -maxdepth 1 -type f ! -name "*.tex" -delete

distclean: clean
	rm -f *.txt *.dat *.out

rebuild: clean all

test: $(TARGET)
	./$(TARGET)

info:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "Target: $(TARGET)"

.PHONY: all debug release clean distclean rebuild test info
