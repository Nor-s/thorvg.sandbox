#include <tvgCommon.h>
#include "editor/app.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static void MainLoop(void* arg)
{
	App::GetInstance().loop();
}

int main()
{
	TVGLOG("MAIN", "START");
	App::InitInstance(App::AppState());
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(MainLoop, NULL, 0, true);
	TVGLOG("COMMON", "EMSCRIPTEN SET MAIN LOOP");
#else
	App::MainLoop();
#endif
	App::DestroyInstance();
	TVGLOG("MAIN", "END");
}