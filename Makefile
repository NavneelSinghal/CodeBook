PEDANTIC_FLAGS = -Iincludes/debug/includes -std=c++17 -O2 -g -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wno-variadic-macros -DDEBUG -DLOCAL -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector

NORMAL_FLAGS = -Iincludes -std=c++17 -O2 -DTIMING -DLOCAL -ftree-vectorize -fopt-info-vec

D ?= 0

ifeq ($(D), 1)
	CXXFLAGS = $(PEDANTIC_FLAGS)
else
	CXXFLAGS = $(NORMAL_FLAGS)
endif

clean:
	find . -maxdepth 1 -type f -executable -delete
