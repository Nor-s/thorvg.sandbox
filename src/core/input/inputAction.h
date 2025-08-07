#ifndef __CORE_INPUT_INPUT_ACTION_H_
#define __CORE_INPUT_INPUT_ACTION_H_

#include "input.h"

namespace core
{

class InputAction
{
public:
	InputAction(InputType type) : mType(type)
	{
	}
	InputType getType() const 
	{
		return mType;
	}

private:
	InputType mType;
};

}	 // namespace ns

#endif