CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -Iexternal -static-libgcc -static-libstdc++
LDFLAGS = -Lexternal/DcisML/lib -lDcisML -lgdi32 -luser32 -lkernel32 -static -static-libgcc -static-libstdc++

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/softrender.exe

SRC_FILES = \
    $(SRC_DIR)/math/vec3.cpp \
    $(SRC_DIR)/math/vec4.cpp \
    $(SRC_DIR)/math/mat4.cpp \
    $(SRC_DIR)/gfx/rasterizer.cpp \
    $(SRC_DIR)/app/main.cpp \
    $(SRC_DIR)/app/demo_scene.cpp \
    $(SRC_DIR)/render/renderer.cpp \

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES = $(OBJ_FILES:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

-include $(DEP_FILES)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run