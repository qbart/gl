#pragma once
#include "glew.hh"
#include "glfw.hh"
#include "types.hh"
#include "log.hh"
#include "ui.hh"

class DefaultApp
{
	struct KeyCounter
	{
		void reset();
		void inc();
		bool one();
	private:
		uint value{ 2 };
	};

public:
	GLFWwindow *wnd{ nullptr };

	bool init(uint w, uint h, const string& title);
	~DefaultApp();

	bool isOpen() const;
	void close();
	Dimension framebufferSize() const;
	void process();
	bool keyPress(int key) const;
	bool keyReleasedOnce(int key);

private:
	unordered_map<int, KeyCounter> keyReleaseCount;
	GLEW glew;
	GLFW glfw;
};

