#include "events.h"
#include "../app.h"

namespace editor
{

void CanvasResizeEvent::execute()
{
    App::CanvasResize(mCanvasIndex, mSize);
}

void CanvasFocusEvent::execute()
{
    App::CavasFocus(mCanvasIndex, mIsFocus);
}

}	 // namespace ns::editor