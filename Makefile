PEDANTIC_FLAGS =-DDEBUG -Wall -Wextra -pedantic -std=c++17 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector

NORMAL_FLAGS =-std=c++17 -O2

D ?= 0

ifeq ($(D), 1)
	CXXFLAGS = $(PEDANTIC_FLAGS)
else
	CXXFLAGS = $(NORMAL_FLAGS)
endif

clean:
	find . -maxdepth 1 -type f -executable -delete
