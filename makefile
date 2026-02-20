VULKAN_SDK ?= G:/LowLevel/Vulkan-1.4.328.1

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -Iexternal -I$(VULKAN_SDK)/Include -static-libgcc -static-libstdc++
LDFLAGS = -Lexternal/DcisML/lib -lDcisML -L$(VULKAN_SDK)/Lib -lgdi32 -luser32 -lkernel32 -static -static-libgcc -static-libstdc++
LDLIBS = -lvulkan-1

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/softrender.exe

SRC_FILES = \
    $(SRC_DIR)/math/vec2.cpp \
    $(SRC_DIR)/math/vec3.cpp \
    $(SRC_DIR)/math/vec4.cpp \
    $(SRC_DIR)/math/mat4.cpp \
    $(SRC_DIR)/app/main.cpp \
    $(SRC_DIR)/app/demo_scene.cpp \
    $(SRC_DIR)/renderer/CPURenderer/rasterizer.cpp \
    $(SRC_DIR)/renderer/CPURenderer/renderer.cpp \

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP_FILES = $(OBJ_FILES:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

-include $(DEP_FILES)

vk_smoke: build/vk_smoke.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

build/vk_smoke.o: src/vk_smoke.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run