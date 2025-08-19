#ifndef _CORE_CANVAS_SCENE_UI_CONTROL_BOX_H_
#define _CORE_CANVAS_SCENE_UI_CONTROL_BOX_H_

#include <memory>
#include "scene/scene.h"

namespace core
{

class IFunction
{
public:
	virtual bool invoke() = 0;
};

template <typename Func>
class LambdaWrapper : public IFunction
{
public:
	LambdaWrapper(Func func) : mFunc(func)
	{
	}
	virtual bool invoke() override
	{
		return mFunc();
	}

private:
	Func mFunc;
};

template <typename F>
auto MakeLambda(F&& f)
{
	using FnType = std::decay_t<F>;
	return std::make_unique<LambdaWrapper<FnType>>(std::forward<F>(f));
}

class ControlBox
{
public:
	// for cursor shape
	enum class Type
	{
		Move,
		Rotate,
		Scale
	};

	enum class ShapeType
	{
		Rect,
		Ellipse
	};

public:
	ControlBox(Scene* scene, Vec2 center, Vec2 wh, Type type, ShapeType shape = ShapeType::Rect);
	ControlBox(Scene* scene, const std::array<Vec2, 4>& obbPoints, Type type = Type::Move);
	~ControlBox();

	void moveTo(Vec2 xy);
	bool onLeftDown(Vec2 xy);

	bool onLeftDrag()
	{
		if (mOnLeftDrag)
		{
			return mOnLeftDrag->invoke();
		}
		return false;
	}

	void setOnLeftDrag(std::unique_ptr<IFunction> onLeftDrag)
	{
		mOnLeftDrag = std::move(onLeftDrag);
	}

	void setVisible(bool visible);
	bool isVisible() const;

private:
	Scene* rScene{nullptr};
	Entity mEntity;
	std::unique_ptr<IFunction> mOnLeftDrag;
	std::array<Vec2, 4> mObbPoints;
};

}	 // namespace core

#endif