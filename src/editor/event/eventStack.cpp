#include "eventStack.h"

namespace editor
{

void EventStack::push(std::unique_ptr<Event> event)
{
	events_.push_back(std::move(event));
}

void EventStack::processEvent()
{
	for (auto& event : events_)
	{
		event->execute();
		event.reset();
	}
	events_.clear();
}

}	 // namespace editor