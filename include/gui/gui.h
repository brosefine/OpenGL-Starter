#pragma once

#include <memory>
#include <string>
#include <queue>

#include <imgui.h>
#include <implot.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>


// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_front(ImVec2(x, y));
        else {
            Data.pop_back();
            Data.push_front(ImVec2(x, y));
            //Data[Offset] = ImVec2(x, y);
            //Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

class Gui {
public:
	Gui(GLFWwindow *win);

	void newFrame();
	void render();
	void renderEnd();

	~Gui();

private:

	void initImGui(GLFWwindow* win);
};