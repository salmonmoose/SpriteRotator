CC = gcc
CXX = g++
RM = rm -f


LDLIBS = \
	-lGL -lXxf86vm -lXext -lX11 \
	-lSDL \
	-lSDL_image \
	-lSDL_ttf \
	-lSDL_mixer

LDFLAGS = $(shell)

SRCS = $(wildcard src/*.c)

OBJS = $(subst .c,.o,$(SRCS)) \

STATIC = \

.PHONY: default all release debug

default all: release

release:	export EXTRA_CPPFLAGS := -O3 -fexpensive-optimizations
debug:		export EXTRA_CPPFLAGS := -DDEBUG -g

CPPFLAGS = \
	$(shell) \
	-Wall \
	-ffast-math \
	-I./include \
	$(EXTRA_CPPFLAGS)

release debug: rotate

rotate: $(OBJS)
	g++ $(LDFLAGS) -o ./bin/Linux/rotate $(OBJS) $(STATIC) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependcanton

include .depend