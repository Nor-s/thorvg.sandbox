#ifndef _EDITOR_EVENT_EVENTS_H_
#define _EDITOR_EVENT_EVENTS_H_

#include <core/core.h>

namespace editor
{

class Event
{
public:
	Event() = default;
	virtual ~Event() = default;
	virtual void execute() = 0;
};

class CanvasResizeEvent : public Event
{
public:
	CanvasResizeEvent(int canvasIndex, const core::Size& size) : mCanvasIndex(canvasIndex), mSize(size)
	{
	}
	~CanvasResizeEvent() = default;
	void execute() override;

private:
	int mCanvasIndex{};
	core::Size mSize{};
};

class CanvasFocusEvent : public Event
{
public:
	CanvasFocusEvent(int canvasIndex, bool isFocus) : mCanvasIndex(canvasIndex), mIsFocus(isFocus)
	{
	}
	~CanvasFocusEvent() = default;
	void execute() override;

private:
	int mCanvasIndex{};
	bool mIsFocus{false};
};

}	 // namespace editor

#endif