CXX = g++

CXXFLAGS = -std=c++11 -Wall

SRC = RealEstateInflation.cc

DEBUG_BIN = RealEstateInvestment_debug
RELEASE_BIN = RealEstateInvestment_release

debug: CXXFLAGS += -g
debug: $(DEBUG_BIN)

release: CXXFLAGS += -O3
release: $(RELEASE_BIN)

$(DEBUG_BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(RELEASE_BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean: rm -f $(DEBUG_BIN) $(RELEASE_BIN)

.PHONY: debug release clean