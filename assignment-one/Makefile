CXX := g++

CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic \
	-Iinclude \
	-Iexternal/imgui \
	-Iexternal/imgui/misc/cpp \
	-Iexternal/imgui-sfml

TARGET := build/sfml-shapes

SRC := \
	src/main.cpp \
	src/Config.cpp \
	src/EditorUI.cpp \
	src/ShapeRenderer.cpp \
	external/imgui/imgui.cpp \
	external/imgui/imgui_draw.cpp \
	external/imgui/imgui_tables.cpp \
	external/imgui/imgui_widgets.cpp \
	external/imgui-sfml/imgui-SFML.cpp \
	external/imgui/misc/cpp/imgui_stdlib.cpp

LIBS := \
	-lsfml-graphics \
	-lsfml-window \
	-lsfml-system \
	-lGL

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build/*

.PHONY: all run clean