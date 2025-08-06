#ifndef _EDITOR_SAVER_TVG_LOTTIE_SAVER_H_
#define _EDITOR_SAVER_TVG_LOTTIE_SAVER_H_

#include "jsonValue.h"

#include <tvgSaveModule.h>
#include <tvgTaskScheduler.h>
#include <tvgLottieModel.h>

#include <sstream>


namespace tvg
{

template<typename T>
static MapValue processKeyframe(T& frame)
{
	MapValue ret;

	if(frame.interpolator)
	{
		MapValue imap;
		MapValue omap;

		imap["x"] = VAL(frame.interpolator->inTangent.x);
		imap["y"] = VAL(frame.interpolator->inTangent.y);
		ret["i"] = VALMOV(imap);

		omap["x"] = VAL(frame.interpolator->outTangent.x);
		omap["y"] = VAL(frame.interpolator->outTangent.y);
		ret["o"] = VALMOV(omap);
	}
	ret["s"] = VAL(frame.value); // value
	ret["t"] = VAL(frame.no);
	ret["h"] = VAL(frame.hold?1:0);
	return ret;
}

template<typename T>
static ArrayValue processKeyframes(T& frames)
{
	ArrayValue ret;
	for(auto& frame: frames)
	{
		ret.push_back(VALMOV(processKeyframe(frame)));
	}

	return ret;
}

template<typename T>
static MapValue processLottieProperty(T& prop)
{
	MapValue ret;

	ret["a"] = VAL((prop.frames && prop.frames->count > 1)? 1 : 0);
	if(prop.frames == nullptr || prop.frames->count == 1) 
	{
		ret["k"] = VAL(prop.value);
		return ret;
	}

	ret["k"] = VALMOV(processKeyframes(*prop.frames));

	return ret;
}

class LottieSaver : public Task
{
private:
	std::stringstream mSstream;
	tvg::Animation* rAnimation = nullptr;
	std::string mFileName;
    std::string name;

private:
	void run(unsigned tid) override;
    void main();

public:
	~LottieSaver();

	bool save(tvg::Animation* canvas, const char* filename);
};

}	 // namespace tvg

#endif