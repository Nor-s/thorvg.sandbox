#ifndef _WINDOW_GLWINDOW_H_
#define _WINDOW_GLWINDOW_H_

#include "window.h"

#include <string>

namespace editor
{

class GLWindow : public Window
{
public:
	GLWindow(const tvg::Size& res);
	~GLWindow();

	void refresh() override;

private:
	std::string mGlslVersion;
};

}	 // namespace editor

#endif
