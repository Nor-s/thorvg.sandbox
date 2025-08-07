#ifndef _EDITOR_EVENT_EVENT_STACK_H_
#define _EDITOR_EVENT_EVENT_STACK_H_

#include <vector>
#include "eventController.h"

namespace editor
{

class EventStack : public EventController
{
public:
	EventStack() = default;
	~EventStack() = default;
	virtual void push(std::unique_ptr<Event> event) override;
	virtual void processEvent() override;

private:
	std::vector<std::unique_ptr<Event>> events_;
};

}	 // namespace editor

#endif