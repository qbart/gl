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

	glfw.window.hintContextVersion(4, 3);
	glfw.window.hintCoreProfileForwardCompat();
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
	moveToHalfRight();
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

#ifdef _DEBUG
void DefaultApp::moveToHalfRight()
{
	int w, h;
	int fh;
	int mx, my, mw, mh;
	int l, t, r, b;

	glfwGetWindowFrameSize(wnd, &l, &t, &r, &b);
	glfwGetWindowSize(wnd, &w, &h);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);

	fh = mh - (t + b - 1);

	glfw.window.size(wnd, { mw / 2, fh });
	glfw.window.pos(wnd, v2(mx + mw / 2, my + t));
}
#endif