CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -Iexternal/include -static-libgcc -static-libstdc++
LDFLAGS = -Lexternal/lib -lWindowManager -lgdi32 -luser32 -lkernel32 -static -static-libgcc -static-libstdc++

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/softrender.exe

# Список файлов вручную
SRC_FILES = \
    $(SRC_DIR)/math/vec3.cpp \
    $(SRC_DIR)/math/vec4.cpp \
    $(SRC_DIR)/math/mat4.cpp \
    $(SRC_DIR)/image.cpp \
    $(SRC_DIR)/main.cpp

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Создаём build директории
$(shell if not exist $(BUILD_DIR) mkdir $(BUILD_DIR))
$(shell if not exist $(BUILD_DIR)\math mkdir $(BUILD_DIR)\math)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES) $(LDFLAGS)

# Правило для компиляции .cpp в .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rmdir /s /q $(BUILD_DIR) 2>nul || exit 0

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run