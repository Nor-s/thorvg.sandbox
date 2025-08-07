#ifndef __CORE_INPUT_INPUT_CONTROLLER_H_
#define __CORE_INPUT_INPUT_CONTROLLER_H_

#include <memory>
#include <vector>
#include <unordered_map>

#include "inputAction.h"

namespace core
{

class InputActionBinding
{
public:
	InputActionBinding(const InputAction& action, InputTrigger triggerState) : mAction(action), mTrigger(triggerState)
	{
	}
	virtual void execute(const InputValue& inputValue) = 0;

	InputTrigger getInputTrigger() const
	{
		return mTrigger;
	}
	const InputAction& getAction() const
	{
		return mAction;
	}

private:
	const InputAction& mAction;
	InputTrigger mTrigger;
};

template <typename Func>
class InputActionFunctionBinding : public InputActionBinding
{
public:
	InputActionFunctionBinding(Func func, const InputAction& action, InputTrigger triggerState)
		: InputActionBinding(action, triggerState), mFunc(func)
	{
	}

	virtual void execute(const InputValue& inputValue) override
	{
		mFunc(inputValue);
	}

private:
	Func mFunc;
};

template <typename TClass, typename Func>
class InputActionMemberFunctionBinding : public InputActionBinding
{
public:
	InputActionMemberFunctionBinding(TClass* classPointer,
									 Func func,
									 const InputAction& action,
									 InputTrigger triggerState)
		: InputActionBinding(action, triggerState), rClassPointer(classPointer), mFunc(func)
	{
	}

	virtual void execute(const InputValue& inputValue) override
	{
		(rClassPointer->*mFunc)(inputValue);
	}

private:
	TClass* rClassPointer;
	Func mFunc;
};

class InputController
{
public:
	template <typename TClass>
	void bindAction(const InputAction& action,
					InputTrigger triggerState,
					TClass* object,
					void (TClass::*func)(const InputValue& inputValue))
	{
		auto* actionEvent =
			new InputActionMemberFunctionBinding<TClass, decltype(func)>(object, func, action, triggerState);
		inputActions_[action.getType()].emplace_back(actionEvent);
	}
	void bindAction(const InputAction& action, InputTrigger triggerState, void (*func)(const InputValue& inputValue))
	{
		auto* actionEvent = new InputActionFunctionBinding<decltype(func)>(func, action, triggerState);
		inputActions_[action.getType()].emplace_back(actionEvent);
	}
	void broadcast(InputType inputType, InputTrigger triggerState, const InputValue& value)
	{
		for (auto& inputActionBinding : inputActions_[inputType])
		{
			if (HasFlag(inputActionBinding->getInputTrigger(), triggerState))
			{
				inputActionBinding->execute(value);
			}
		}
	}
	size_t size() const
	{
		return inputActions_.size();
	}

private:
	// TODO: delete input action binding
	std::unordered_map<InputType, std::vector<InputActionBinding*>> inputActions_;
};

}	 // namespace core

#endif