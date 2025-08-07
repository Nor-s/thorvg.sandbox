#include "animationBuilder.h"

namespace core
{

AnimationBuilder::AnimationBuilder()
{
}
AnimationBuilder::~AnimationBuilder()
{
}
// AnimationBuilder::AnimationBuilder(tvg::LottieAnimation* animation)
// : rAnimation(animation)
// {
// 	auto picture = PICTURE(rAnimation->picture());
// 	rLoader = static_cast<LottieLoader*>(picture->loader);
// }

// float AnimationBuilder::totalFrame()
// {
//     return rAnimation->totalFrame();
// }

// float AnimationBuilder::duration()
// {
//     return rAnimation->duration();
// }

// void AnimationBuilder::frameUpdate(float no)
// {
//     rAnimation->frame(no);
// }

// void AnimationBuilder::updateCurrentFrame()
// {
//     frameUpdate(mCurrentFrame);
// }

// void AnimationBuilder::pick(int x, int y)
// {
// 	auto* comp = rLoader->comp;
//     if(!comp || !comp->root || !comp->root->scene)
//     {
//         return;
//     }

//     auto paints = comp->root->scene->paints();
//     for(auto& paint: paints)
//     {
//         float x, y, w, h;
//         paint->bounds(&x, &y, &w, &h);
//     }
// 	for (auto* layer : comp->root->children)
// 	{
// 		auto* llayer = static_cast<LottieLayer*>(layer);
// 		switch (llayer->type)
// 		{
// 			case LottieLayer::Type::Shape:
// 			{
// 				break;
// 			}
// 		};
// 	}}

// void AnimationBuilder::onUpdateKeyframe(KeyframeVar key)
// {

// }

// void AnimationBuilder::onAddShapeLayer()
// {
// }

// void AnimationBuilder::onRemoveShapeLayer()
// {
// }

}	 // namespace core