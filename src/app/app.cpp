#include <app/app.h>

GLApp::GLApp(int width, int height, std::string const& name)
	: window_(width, height, name)
	, gui_(window_.getPtr())
	, showGui_(false)
{
}

void GLApp::renderLoop()
{
	while (!window_.shouldClose())
	{
		processKeyboardInput();
		if (showGui_) {

			gui_.newFrame();
			renderGui();
			gui_.render();
		}

		renderContent();

		if (showGui_) gui_.renderEnd();
		window_.endFrame();
	}
}
