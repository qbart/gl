#include "default_app.hh"

void DefaultApp::KeyCounter::reset()
{
	value = 0;
}

void DefaultApp::KeyCounter::inc()
{
	++value;
	if (value > 2)
		value = 2;
}

bool DefaultApp::KeyCounter::one()
{
	return value == 1;
}

DefaultApp::~DefaultApp()
{
	if (wnd)
		glfw.window.destroy(wnd);
	glfw.terminate();
}

bool DefaultApp::init(uint w, uint h, const string& title)
{
	if (!glfw.init())
	{
		std::cerr << "Failed to init GLFW";
		return false;
	}

	glfw.window.hint430();
	glfw.window.hintResizable(true);

	wnd = glfw.window.create(w, h, title.c_str());
	if (!wnd)
	{
		std::cerr << "Failed to create Window";
		return false;
	}
	glfw.window.makeContextCurrent(wnd);

	if (!glew.init())
	{
		std::cerr << "Failed to init GLEW";
		return false;
	}

	gl_printInfo();
	gl_bindDebugCallback();
#ifdef _DEBUG
	glfw.window.moveToHalfRight(wnd);
#endif
	return true;
}

bool DefaultApp::isOpen() const
{
	return !glfw.window.shouldClose(wnd);
}

void DefaultApp::close()
{
	glfw.window.shouldClose(wnd, true);
}

Dimension DefaultApp::framebufferSize() const
{
	return glfw.window.framebufferSize(wnd);
}

void DefaultApp::process()
{
	glfw.window.swapBuffers(wnd);
	glfw.pollEvents();
}

bool DefaultApp::keyPress(int key) const
{
	return glfw.window.keyPress(wnd, key);
}

bool DefaultApp::keyReleasedOnce(int key)
{
	if (glfw.window.keyPress(wnd, key))
		keyReleaseCount[key].reset();

	if (glfw.window.keyRelease(wnd, key))
	{
		keyReleaseCount[key].inc();
		return keyReleaseCount[key].one();
	}

	return false;
}
