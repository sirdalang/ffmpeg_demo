
# customize begin

TARGET=ffmpegdemo

CC = gcc
CPP = g++

SRC_DIRS := .
RELEASE_DIR := release
DEBUG_DIR := debug

CFLAGS = -Wall
LFLAGS = -L. -lpthread -l

RFLAGS = -O2
DFLAGS = -g

# customize end

RFLAGS += $(CFLAGS)
DFLAGS += $(CFLAGS)

RELEASE_TARGET=$(RELEASE_DIR)/$(TARGET)
DEBUG_TARGET=$(DEBUG_DIR)/$(TARGET)

CFILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS_NOTSORT = $(patsubst %.c,%.o, $(CFILES))
CPPFILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
CPPOBJS_NOTSORT = $(patsubst %.cpp,%.o, $(CPPFILES))

OBJS = $(sort $(OBJS_NOTSORT))
CPPOBJS = $(sort $(CPPOBJS_NOTSORT))

%/.:
	mkdir -m 777 -p $*

all: release
release: $(RELEASE_DIR)/. $(RELEASE_DIR)/src/. $(RELEASE_TARGET)
debug: $(DEBUG_DIR)/. $(DEBUG_DIR)/src/. $(DEBUG_TARGET)
.PHONY : clean all release debug

$(RELEASE_TARGET):$(addprefix $(RELEASE_DIR)/,$(OBJS)) $(addprefix $(RELEASE_DIR)/,$(CPPOBJS))
	$(CPP) -o $@  $^  $(RFLAGS) $(LFLAGS)

$(DEBUG_TARGET):$(addprefix $(DEBUG_DIR)/,$(OBJS)) $(addprefix $(DEBUG_DIR)/,$(CPPOBJS))
	$(CPP) -o $@  $^  $(DFLAGS) $(LFLAGS)

$(DEBUG_DIR)/%.o:%.c
	$(CC) $(DFLAGS) -c $^ -o $@

$(DEBUG_DIR)/%.o: %.cpp
	$(CPP) $(DFLAGS) -c $< -o $@

$(RELEASE_DIR)/%.o:%.c
	$(CC) $(RFLAGS) -c $^ -o $@

$(RELEASE_DIR)/%.o: %.cpp
	$(CPP) $(RFLAGS) -c $< -o $@

clean:
	@rm -rvf $(RELEASE_DIR) $(DEBUG_DIR)