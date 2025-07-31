#include "exampleBase.h"

#include "examples.h"

namespace tvgexam
{
    ExampleCanvas::ExampleCanvas(void* context, tvg::Size size)
		: core::CanvasWrapper(context, size)
	{
		if(gExampleList.size() == 0)
		{
			gExampleList.push_back(std::make_unique<EmptyExample>());
			gExampleList.push_back(std::make_unique<LottieExample>());
			gExampleList.push_back(std::make_unique<SvgExample>());
			gExampleList.push_back(std::make_unique<BoundingBoxExample>());
			gExampleList.push_back(std::make_unique<ParticleExample>());
		}
		mCurrentExampleIdx = 0;
		rExample = gExampleList[mCurrentExampleIdx].get();
	}
}
