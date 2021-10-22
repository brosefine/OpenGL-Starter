#include <ex_app.h>
#include <rendering/triag.h>
#include <iostream>

ExampleApp::ExampleApp(int width, int height) 
	: GLApp(width, height, "Hello Triangle")
	, triag_(std::make_shared<Mesh>(triagPositions, triagUVs, triagIndices))
	, shader_(std::make_shared<Shader>("simple.vert", "simple.frag"))
	, tex_(std::make_shared<Texture>("exampleTexture.png"))
	, useTexture_(false)
{
	// turn on vSync
	glfwSwapInterval(1);
	
	showGui_ = true;
}

void ExampleApp::renderContent()
{
	// bind default framebuffer (= render to screen)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// set size of viewport
	glViewport(0, 0, window_.getWidth(), window_.getHeight());
	// background color
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader_->use();
	// upload uniform value to shader
	// uniform name has to match name in shader
	shader_->setUniform("useTexture", useTexture_);

	// active texture id has to match layout binding in shader
	glActiveTexture(0);
	tex_->bind();
	triag_->draw(GL_TRIANGLES);
}

void ExampleApp::processKeyboardInput() {
	
	auto win = window_.getPtr();
	
	// enable / disable gui with G and H button
	if (glfwGetKey(win, GLFW_KEY_G) == GLFW_PRESS) {
		showGui_ = true;
	}
	else if (glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS) {
		showGui_ = false;
	}
}

void ExampleApp::renderGui() {
	ImGui::ShowDemoWindow();

	ImGui::Begin("Texture Settings");
	ImGui::Checkbox("use texture", &useTexture_);
	ImGui::End();
	
}
