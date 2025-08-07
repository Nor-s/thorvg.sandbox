#include "tvgLottieSaver.h"
#include <tvgCommon.h>
#include <tvgInlist.h>
#include <tvgShape.h>
#include <tvgLottieExpressions.h>
#include <tvgLottieModifier.h>
#include <tvgPicture.h>
#include <tvgFrameModule.h>
#include <tvgAnimation.h>
#include <tvgLottieLoader.h>
#include <tvgLottieBuilder.h>

namespace tvg
{

MapValue processTransform(LottieTransform* transform)
{
	TVGLOG("saver", "transform start");
	MapValue ret;
	if (transform == nullptr)
		return ret;

	ret["a"] = VALMOV(processLottieProperty(transform->anchor));
	ret["s"] = VALMOV(processLottieProperty(transform->scale));
	ret["r"] = VALMOV(processLottieProperty(transform->rotation));
	ret["p"] = VALMOV(processLottieProperty(transform->position));

	TVGLOG("saver", "transform end");

	return ret;
}

ArrayValue processShapes(LottieLayer* layer)
{
	ArrayValue ret;
	
	auto& shapeLayers= layer->children;
	for (auto& shapeLayer : shapeLayers)
	{
		MapValue val;

		// todo: id -> djb2Encode
		val["nm"] = VAL(std::to_string(shapeLayer->id));
		val["hd"] = VAL(shapeLayer->hidden);

		switch (shapeLayer->type)
		{
			case LottieObject::Rect:
			{
				auto* rect = static_cast<LottieRect*>(shapeLayer);
				// todo: bm
				val["ty"] = VAL("rc");
				val["bm"] = VAL(0);
				val["s"] = VALMOV(processLottieProperty(rect->size));
				val["p"] = VALMOV(processLottieProperty(rect->position));
				val["r"] = VALMOV(processLottieProperty(rect->radius));
				val["d"] = VAL(rect->clockwise?1:0); // Direction the shape is drawn as, mostly relevant when using trim path
				break;
			}
			case LottieObject::SolidFill:
			{
				auto* fill = static_cast<LottieSolidFill*>(shapeLayer);
				// todo: bm
				val["ty"] = VAL("fl");
				val["bm"] = VAL(0);
				val["c"] = VALMOV(processLottieProperty(fill->color));
				val["o"] = VALMOV(processLottieProperty(fill->opacity));
				val["r"] = VAL(fill->rule == FillRule::NonZero? 1:0);

				break;
			}
		};
		ret.push_back(VALMOV(val));
	}

	return ret;
}

MapValue processShapeLayer(LottieLayer* layer)
{
	MapValue ret;
	ret["ty"] = VAL(4);
	ret["nm"] = VAL(layer->name);
	ret["sr"] = VAL(layer->timeStretch);
	ret["st"] = VAL(layer->startFrame);
	ret["ip"] = VAL(layer->inFrame);
	ret["op"] = VAL(layer->outFrame);
	ret["hd"] = VAL(layer->hidden);
	// ret["ddd"] = VAL(0);
	ret["bm"] = VAL((int) layer->blendMethod);
	ret["hasMask"] = VAL(!layer->masks.empty());
	ret["ao"] = VAL((int) layer->autoOrient);

	ret["ks"] = VALMOV(processTransform(layer->transform));
	ret["shapes"] = VALMOV(processShapes(layer));
	ret["ind"] = VAL(layer->ix);

	return ret;
}

ArrayValue processLayers(LottieLoader* lottieLoader)
{
	ArrayValue ret;

	auto* comp = lottieLoader->comp;
	for (auto* layer : comp->root->children)
	{
		auto* llayer = static_cast<LottieLayer*>(layer);
		switch (llayer->type)
		{
			case LottieLayer::Type::Shape:
			{
				ret.push_back(VALMOV(processShapeLayer(llayer)));
				break;
			}
		};
	}
	return ret;
}

void LottieSaver::main()
{
	auto picture = PICTURE(rAnimation->picture());
	auto* loader = picture->loader;

	if (loader->type != FileType::Lot)
		return;

	auto* lottieLoader = static_cast<LottieLoader*>(loader);
	auto* comp = lottieLoader->comp;

	MapValue jsonMap;

	jsonMap["nm"] = VAL(comp->name);
	jsonMap["ddd"] = VAL(0);
	jsonMap["h"] = VAL((int) picture->h);
	jsonMap["w"] = VAL((int) picture->w);
	jsonMap["fr"] = VAL((int) lottieLoader->frameRate);
	jsonMap["ip"] = VAL((int) comp->root->inFrame);
	jsonMap["op"] = VAL((int) comp->root->outFrame);

	{
		MapValue meta;
		meta["g"] = VAL("tvg.saver");
		jsonMap["meta"] = VALMOV(meta);
	}
	jsonMap["layers"] = VALMOV(processLayers(lottieLoader));

	auto lottieJson = VALMOV(jsonMap);
	visitValue(mSstream, *lottieJson.get(), true);
}

void LottieSaver::run(unsigned tid)
{
	main();

	std::ofstream outFile(mFileName.c_str());
	outFile << mSstream.rdbuf();
	outFile.close();
}

LottieSaver::~LottieSaver()
{
}

bool LottieSaver::save(Animation* animation, const char* filename)
{
	rAnimation = animation;
	mFileName = filename;
	TaskScheduler::request(this);
	return false;
}

}	 // namespace tvg
