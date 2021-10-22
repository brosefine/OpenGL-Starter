#include <app/app.h>
#include <rendering/mesh.h>
#include <rendering/shader.h>
#include <rendering/texture.h>

class ExampleApp : public GLApp {
public:

	ExampleApp(int width, int height);

private:

	std::shared_ptr<Mesh> triag_;
	std::shared_ptr<Shader> shader_;
	std::shared_ptr<Texture> tex_;

	bool useTexture_;

	void renderContent() override;
	virtual void processKeyboardInput() override;
	virtual void renderGui() override;
};