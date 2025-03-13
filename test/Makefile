CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../engine -Idependencies/glm -Idependencies/freeglut -Idependencies/gtest
CFLAGS = -Wall -std=c++20 -fexceptions -fPIC
RCFLAGS = 
RESINC = 
LIBDIR = 
LIB = -lengine -lGL -lGLU -lglut -lgtest -lgtest_main
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -D_DEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR) -L../engine/bin/Debug
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) -L../engine/bin/Debug -lengine
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/test

SRC_FILES = cameraTest.cpp directionalLightTest.cpp lightTest.cpp listTest.cpp main.cpp materialTest.cpp meshTest.cpp nodeTest.cpp pointLightTest.cpp shadowTest.cpp spotLightTest.cpp textureTest.cpp utils.cpp vertexTest.cpp

OBJ_DEBUG = $(patsubst %.cpp, $(OBJDIR_DEBUG)/%.o, $(SRC_FILES))

all: install_gtest debug run_tests

install_gtest:
	@if [ ! -f /usr/local/lib/libgtest.a ]; then \
		echo "Installing and building GTest..."; \
		apt-get update && apt-get install -y build-essential cmake libgtest-dev; \
		cd /usr/src/gtest && cmake . && make; \
		cp *.a /usr/local/lib; \
		echo "GTest installed successfully."; \
	else \
		echo "GTest already installed."; \
	fi

clean: clean_debug

before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug:

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/%.o: %.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $< -o $@

# Esecuzione dei test
run_tests: out_debug
	LD_LIBRARY_PATH=../engine/bin/Debug ./bin/Debug/test

clean_debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

.PHONY: before_debug after_debug clean_debug install_gtest run_tests

