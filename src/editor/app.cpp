#include "app.h"
#include "window/glWindow.h"
#include "imgui/imguiCanvas.h"

#include <tvgGl.h>
#include <thorvg.h>
#include <tvgCommon.h>
#include <core/core.h>

App& App::GetInstance()
{
	static App instance;
	return instance;
}

void App::InitInstance(const AppState& state)
{
	tvg::Initializer::init(0);

	GetInstance().mState = state;
	GetInstance().init();

	// tvgGl.h
	glInit();
}

void App::DestroyInstance()
{
}

void App::MainLoop()
{
	App& app = GetInstance();
	while (app.mState.running)
	{
		app.loop();
	}
}

void App::init()
{
	mWindow = std::make_unique<editor::GLWindow>(mState.resolution);
	mWindow->show();
}

void App::loop()
{
	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	(void) io;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				mState.running = false;
				break;
			default:
				break;
		}
	}
	if (SDL_GetWindowFlags(mWindow->mWindow) & SDL_WINDOW_MINIMIZED)
	{
		SDL_Delay(10);
		return;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
				 clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	draw();
	drawgui();
	drawend();
}

void App::draw()
{
	// temp code
	static bool bIsInit = false;
	if (!bIsInit)
	{
		float clearColor[3] = {1.0f, 1.0f, 1.0f};
		mCanvasList.push_back(new core::CanvasWrapper(mWindow->mContext, {500.0f, 500.0f}));
		mCanvasList.back()->clearColor(clearColor);
		auto shape4 = tvg::Shape::gen();
		shape4->appendRect(0, 0, 300, 300, 50, 50);	   // x, y, w, h, rx, ry
		shape4->fill(0, 0, 0);						   // r, g, b
		mCanvasList.back()->getCanvas()->push(shape4);
		mCanvasList.back()->draw();

		mCanvasList.push_back(new core::CanvasWrapper(mWindow->mContext, {500.0f, 500.0f}));
		mCanvasList.back()->clearColor(clearColor);
		mCanvasList.back()->draw();
		auto shape2 = tvg::Shape::gen();
		shape2->appendCircle(150, 150, 150, 150);	 // cx, cy, radiusW, radiusH
		shape2->fill(255, 125, 59);					 // r, g, b
		mCanvasList.back()->getCanvas()->push(shape2);
		bIsInit = true;
	}
	for (auto& canvas : mCanvasList)
	{
		canvas->draw();
	}
}

void App::drawgui()
{
	{
		static bool show_demo_window = true;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
	}
	// temp code
	int i = 0;
	for (auto* canvas : mCanvasList)
	{
		editor::ImGuiCanvasView().onDraw("canvas" + std::to_string(i++), *canvas);
	}
}

void App::drawend()
{
	// ImGUI Rendering
	ImGui::Render();
	SDL_GL_MakeCurrent(mWindow->mWindow, mWindow->mContext);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	mWindow->refresh();
}
