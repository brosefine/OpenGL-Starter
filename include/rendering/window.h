#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class GLWindow {
public:
	GLWindow() {};
	GLWindow(int width, int height, std::string name);
	~GLWindow();

	void setWidth(int w);

	void setHeight(int h);

	int getWidth() { return width_; }
	int getHeight() { return height_; }
	GLFWwindow* getPtr() { return windowPtr_; }

	bool hasChanged() { 
		if (changed_) {
			changed_ = false;
			return true;
		}
		return false; 
	}
	bool shouldClose();
	void endFrame();

private:

	GLFWwindow* windowPtr_;
	int width_;
	int height_;
	std::string name_;

	bool changed_;

	void init();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void processKeyboardInputs();
};