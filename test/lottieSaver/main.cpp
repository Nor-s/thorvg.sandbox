#include <iostream>

#include <tvgLottieSaver.h>
#include <tvgPicture.h>

int main()
{
	tvg::Initializer::init(0);
	{
		auto* canvas = tvg::SwCanvas::gen();
		uint32_t buffer[100 * 100];
		canvas->target(buffer, 100, 100, 100, ColorSpace::ABGR8888);
		tvg::LottieSaver saver;
		auto anim = tvg::Animation::gen();
		auto path = EXAMPLE_DIR "/simple/Simple.json";
		anim->picture()->load(path);
		TVGLOG("saverd", "%s", path);
		saver.save(anim, "save.json");
	}
	tvg::Initializer::term();
}