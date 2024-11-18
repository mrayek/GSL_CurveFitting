TARGET = program
BUILD  = build
SRC    = src
INC    = include

CXX      = g++
CXXFLAGS = -std=c++20 -I$(INC)
LDLIBS   = -lgsl -lgslcblas -lm

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(SOURCES))

output: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(BUILD)/*.o $(BUILD)/$(TARGET)





