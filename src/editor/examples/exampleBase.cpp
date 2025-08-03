#include "exampleBase.h"

#include "examples.h"

namespace tvgexam
{
    ExampleCanvas::ExampleCanvas(void* context, tvg::Size size, bool bIsSw)
		: core::CanvasWrapper(context, size, bIsSw)
	{
		if(gExampleList.size() == 0)
		{
			gMakeExample.push_back([](){
				return std::make_unique<EmptyExample>();
			});
			gMakeExample.push_back([](){
				return std::make_unique<LottieExample>();
			});
			gMakeExample.push_back([](){
				return std::make_unique<SvgExample>();
			});
			gMakeExample.push_back([](){
				return std::make_unique<BoundingBoxExample>();
			});
			gMakeExample.push_back([](){
				return std::make_unique<ParticleExample>();
			});
			for(auto& makeFunc: gMakeExample)
			{
				gExampleList.push_back(makeFunc());
			}
		}
		mCurrentExampleIdx = 0;
		rExample = gExampleList[mCurrentExampleIdx].get();
		mExample = gMakeExample[mCurrentExampleIdx]();
	}
}
