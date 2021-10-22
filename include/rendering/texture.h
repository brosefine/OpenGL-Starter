#pragma once

#include <vector>
#include <string>

class Texture {
public:
	// create texture from image file
	Texture(std::string filename, bool srgb = false);
	~Texture();

	void bind() const;
	unsigned int getTexId() const { return texId_; }

private:
	unsigned int texId_;
};

class FBOTexture {
public:
	// create empty fbo texture
	FBOTexture(int width, int height);
	~FBOTexture();

	void resize(int width, int height);

	void bindTex() const;
	void bindImageTex(int binding, unsigned int mode) const;
	unsigned int getTexId() const { return texId_; }
	unsigned int getFboId() const { return fboId_; }

	int getWidth() const { return width_; }
	int getHeight() const { return height_; }

private:
	int width_, height_;
	unsigned int texId_;
	unsigned int fboId_;
};

class CubeMap {
public:

	/*
	  Create a cubemap texture from 6 texture paths
	  Face order is: right, left, top, bottom, front, back
	  = +x, -x, +y, -y, +z, -z
	*/
	CubeMap(std::vector<std::string> faces);

	void bind() const;
	unsigned int getId() const { return ID_; }
private:
	unsigned int ID_;

	void loadImages(std::vector<std::string> faces);
};