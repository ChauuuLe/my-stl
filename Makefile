# Compiler settings
CXX       := g++
# remove warning flags and add -w to disable all warnings
CXXFLAGS  := -std=c++17 -w -Icontainers

# Build directories & files
OBJDIR    := build
SRCS      := tests/test_vector.cpp
OBJS      := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
TARGET    := test_vector

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
