# flowery — SDL3 spirograph renderer
CXX      ?= c++
OPT      ?= -O3
CXXFLAGS ?= $(OPT) -std=c++20 -Wall -Wextra

# -march=native enables the AVX2 + FMA paths in the sampling kernel, the
# bounding box and the screen mapping. The same sources build without it --
# every vector path is behind __AVX2__ with a scalar fallback -- so a portable
# binary is one override away:
#
#     make CXXFLAGS='-O2 -std=c++20 -Wall -Wextra'
#
# -fno-math-errno and -fno-trapping-math only drop the obligation to set
# errno and raise traps on domain errors, which nothing here reads; unlike
# -ffast-math they do not let the compiler reassociate the arithmetic.
CXXFLAGS += -march=native -fno-math-errno -fno-trapping-math
CXXFLAGS += $(shell pkg-config --cflags sdl3)
LDLIBS   += $(shell pkg-config --libs sdl3) -lm

SRC = src/main.cpp src/flowery.cpp
HDR = src/flowery.h

flowery: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LDLIBS)

# The tests cover the curve math, which does not touch SDL.
TEST = .build/test_flowery

$(TEST): tests/test_flowery.cpp src/flowery.cpp $(HDR)
	@mkdir -p .build
	$(CXX) $(CXXFLAGS) -Isrc -o $@ tests/test_flowery.cpp src/flowery.cpp -lm

check: $(TEST)
	./test.sh

clean:
	rm -f flowery
	rm -rf .build

.PHONY: check clean
