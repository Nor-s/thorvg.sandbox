#ifndef _EDITOR_EVENT_INPUT_EVENT_HANDLER_H_
#define _EDITOR_EVENT_INPUT_EVENT_HANDLER_H_

#include <core/core.h>

class InputEventHandler
{
	struct State
	{
		bool leftMouseDown{false};
		core::Vec2 mousePos{};
	};

public:
	void setInputContoller(core::InputController* inputController);
	void processEvent(const SDL_Event& event);
	void processTick();

private:
	State mState;
	core::InputController* rInputController = nullptr;
};

#endif