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

template<typename Func>
class LambdaWrapper : public IFunction 
{
public:
    LambdaWrapper(Func func)
    : mFunc(func)
    {
    }
    virtual bool invoke() override 
    {
        return mFunc();
    }

private:
    Func mFunc;
};

template<typename F>
auto MakeLambda(F&& f) {
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

public:
    ControlBox(Scene* scene, Vec2 xy, Vec2 wh ,Type type);
    ~ControlBox();

    void moveTo(Vec2 xy);

    bool onLeftDrag() 
    {
        if (mOnLeftDrag) 
        {
            return mOnLeftDrag->invoke();
        }
        return false;
    }
    bool onLeftUp()
    {
        if (mOnLeftUp) 
        {
            return mOnLeftUp->invoke();
        }
        return false;
    }

    void setOnLeftDrag(std::unique_ptr<IFunction> onLeftDrag) 
    {
        mOnLeftDrag = std::move(onLeftDrag);
    }
    void setOnLeftUp(std::unique_ptr<IFunction> onLeftUp)
    {
        mOnLeftUp = std::move(onLeftUp);
    }

    void setVisible(bool visible);
    bool isVisible() const;

private:
    Scene* rScene{nullptr};
    Entity mEntity;
    std::unique_ptr<IFunction> mOnLeftDrag;
    std::unique_ptr<IFunction> mOnLeftUp;

};

}	 // namespace core

#endif