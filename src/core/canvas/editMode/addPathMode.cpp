#include "addPathMode.h"
#include "scene/ui/controlBox.h"
#include "canvas/animationCreatorCanvas.h"
namespace core
{

AddPathMode::PathPointWithControl::PathPointWithControl(const PathPoint& p, std::unique_ptr<ControlBox> control)
	: point(p), controlBox(std::move(control))
{
}

AddPathMode::AddPathMode(AnimationCreatorCanvas* canvas)
{
	rCanvas = canvas;
	onInputAttach({1.0f, 1.0f});
}

AddPathMode::~AddPathMode()
{
}

bool AddPathMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	mContext.beforePos = mContext.currentPos = mContext.startPos = inputValue.get<Vec2>();
	auto& path = mContext.pathList;

    // todo:: pick control?
    mContext.clicked = nullptr;
    for(auto& p: mContext.pathList)
    {
        if (p.controlBox->onLeftDown(mContext.currentPos))
        {
            mContext.clicked = &p;
            return true;
        }
    }

    // if no pick
    {
        const float width =  CommonSetting::Width_DefaultPathPointControlBox;
        auto control = std::make_unique<ControlBox>(rCanvas->mOverlayScene.get(), mContext.currentPos, Vec2{width, width}, ControlBox::Type::Move, ControlBox::ShapeType::FillStrokeEllipse);
        auto pathPoint = PathPoint{.point=mContext.currentPos};
	    path.emplace_back(pathPoint, std::move(control));
        path.back().controlBox->setOnLeftDrag(MakeLambda([ctx=&mContext, p=&path.back()](){
            auto delta = (ctx->currentPos - ctx->beforePos);
            p->controlBox->moveByDelta(delta);
            p->point.point = p->point.point + delta;
            return true;
        }));
    }

	return false;
}

bool AddPathMode::onDragLeftMouse(const InputValue& inputValue)
{
	mContext.beforePos = mContext.currentPos;
    mContext.currentPos = inputValue.get<Vec2>();

    if (mContext.clicked)
    {
        return mContext.clicked->controlBox->onLeftDrag();
    }
    auto len = length2(mContext.currentPos - mContext.startPos);
	auto& path = mContext.pathList;
    if (len > CommonSetting::Threshold_AddPathModeChangeCurve)
    {
        path.back().point.type = PathPoint::Type::Curve;
        mContext.currentEditType = PathPoint::Type::Curve; 
    }
    else 
    {
        path.back().point.type = PathPoint::Type::Line;
        mContext.currentEditType = PathPoint::Type::Line; 
    }

	return false;
}

bool AddPathMode::onMoveMouse(const InputValue& inputValue)
{
	return false;
}

bool AddPathMode::onEndLeftMouse(const InputValue& inputValue)
{
    mContext.clicked = nullptr;
	return false;
}

bool AddPathMode::onInputAttach(const InputValue& inputValue)
{
	return false;
}

bool AddPathMode::onInputDetach(const InputValue& inputValue)
{
	return false;
}

void AddPathMode::onUpdate()
{
}

}	 // namespace core