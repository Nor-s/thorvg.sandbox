#include "app.h"
#include "window/window.h"
#include "event/inputEventHandler.h"


#include <core/core.h>

using namespace core; 

void App::CanvasResize(int canvasIndex, core::Size size)
{
	App::GetInstance().mCanvasList[canvasIndex]->mSize = size;
}
void App::CavasFocus(int canvasIndex, bool isFocus)
{
	auto& app = App::GetInstance();
    bool isCurrent = canvasIndex == app.mCurrentFocusCanvas;
	if ((!isCurrent || app.rInputController == nullptr) && isFocus)
	{
		app.focusCanvas(canvasIndex);
	}
    else if(isCurrent && !isFocus && app.rInputController != nullptr)
    {
        app.setInputController(nullptr);
    }
}
void App::focusCanvas(int canvasIndex)
{
	mCurrentFocusCanvas = canvasIndex;
	FocusCurrentCanvas(static_cast<CANVAS_ptr>(mCanvasList[mCurrentFocusCanvas]));
    setInputController(mCanvasList[mCurrentFocusCanvas]->getInputController());
}
void App::setInputController(core::InputController* inputController)
{
    rInputController = inputController;
    mInputEventHandler.setInputContoller(rInputController);
}

bool App::processEvent()
{
	mEventController->processEvent();

	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	core::io::deltaTime = io.DeltaTime;

	if(rInputController)
		rInputController->sync();

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
        mInputEventHandler.processEvent(event);
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
		return false;
	}
    mInputEventHandler.processTick();
	return true;
}