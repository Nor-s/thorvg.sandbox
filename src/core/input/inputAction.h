#ifndef __CORE_INPUT_INPUT_ACTION_H_
#define __CORE_INPUT_INPUT_ACTION_H_

#include "input.h"

namespace core
{

class InputAction
{
public:
	InputAction(InputType type, int priority = 0, bool useCapture=true) : mType(type), mPriority(priority), mUseCapture(useCapture)
	{
	}
	InputType getType() const
	{
		return mType;
	}

private:
	friend class InputActionBinding;
	friend class InputController;
	InputType mType;
	int mPriority{0};
	bool mUseCapture{false};
};

}	 // namespace core

#endif