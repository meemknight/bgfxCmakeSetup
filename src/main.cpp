#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

static void error_callback(int error, const char *description)
{
	std::cerr << "Error: " << description << std::endl;
}

int main(void)
{
	// Set GLFW error callback
	glfwSetErrorCallback(error_callback);

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// No OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create a GLFW window
	GLFWwindow *window = glfwCreateWindow(800, 600, "BGFX Example", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Initialize BGFX
	bgfx::Init bgfxInit;

	// Platform data for BGFX
	bgfx::PlatformData pd;
	pd.nwh = glfwGetWin32Window(window); // Windows
	pd.ndt = nullptr;
	pd.context = nullptr;
	pd.backBuffer = nullptr;
	pd.backBufferDS = nullptr;

	bgfxInit.platformData = pd;
	bgfxInit.resolution.width = 800;
	bgfxInit.resolution.height = 600;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	bgfxInit.callback = nullptr;
	bgfxInit.allocator = nullptr;

	if (!bgfx::init(bgfxInit))
	{
		std::cerr << "Failed to initialize BGFX" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	// Enable debug text to get more information
	bgfx::setDebug(BGFX_DEBUG_TEXT);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Set view 0 clear state to red
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, 800, 600); // Set view rectangle to match window size

		// Clear the screen
		bgfx::touch(0);

		// Debug text to help with troubleshooting
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 0, 0x4f, "BGFX is running");
		bgfx::dbgTextPrintf(0, 1, 0x4f, "Screen should be red");

		// Advance to the next frame
		bgfx::frame();
	}

	// Cleanup
	bgfx::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
