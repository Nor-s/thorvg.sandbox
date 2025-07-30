#ifndef _EDITOR_APP_H_
#define _EDITOR_APP_H_

#include <core/core.h>

#include <string>
#include <memory>

namespace core
{
class CanvasWrapper;
}
namespace editor
{
class Window;
}

// todo: singleton
class App
{
public:
	struct AppState
	{
		tvg::Size resolution;
		std::string title;
		bool running = true;
		AppState() : resolution(800, 600), title("thorvg.sandbox")
		{
		}
	};

	static App& GetInstance();
	static void InitInstance(const AppState& state = AppState());
	static void DestroyInstance();
	static void MainLoop();
	static const AppState& GetState()
	{
		return GetInstance().mState;
	}

public:
	App() = default;
	~App() = default;

	void init();
	void loop();
	void draw();
	void drawgui();
	void drawend();

private:
	AppState mState;
	std::unique_ptr<editor::Window> mWindow;
	// todo: smart pointer or change container class
	std::vector<core::CanvasWrapper*> mCanvasList;
};

#endif