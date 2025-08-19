#ifndef _NS_CORE_COMMON_H_
#define _NS_CORE_COMMON_H_

#include <string_view>
#include <memory>
#include <numeric>

#include "logger.h"
#include "timer.h"
#include "vec.h"

namespace core
{

using Size = Vec2;

struct Style
{
    inline static const Vec3 HoverOutlineColor{200.0f, 100.0f, 50.0f};
    inline static const Vec3 ControlBoxOutlineColor{255.0f, 127.0f, 63.0f};
    inline static const Vec3 ControlBoxInnerColor{250.0f, 250.0f, 255.0f};
    inline static const Vec3 DefaultStrokeColor{4.0f, 5.0f, 5.0f};
    inline static const Vec3 DefaultFillColor{127.0f, 127.0f, 127.0f};

    inline static const float BBoxControlBoxWidth{10.0f};
    inline static const float BBoxRotationControlBoxWidth{50.0f};
};

}	 // namespace core

#endif