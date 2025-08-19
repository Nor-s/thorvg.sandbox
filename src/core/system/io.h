#ifndef _CORE_SYSTEM_IO_H_
#define _CORE_SYSTEM_IO_H_

#include "common/common.h"

namespace core
{

struct io
{
	inline static float currentFrame{0.0f};
	inline static bool isStopFrame{false};
	inline static float deltaTime{0.0f};
	inline static Vec2 mouseOffset{0.0f, 0.0f};
};

}	 // namespace core

#endif