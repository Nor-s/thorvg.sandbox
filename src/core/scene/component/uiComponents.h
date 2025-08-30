#ifndef _CORE_SCENE_COMPONENT_UI_COMPONENTS_H_
#define _CORE_SCENE_COMPONENT_UI_COMPONENTS_H_

#include <memory>
#include "scene/ui/bbox.h"

namespace core
{

struct BBoxControlComponent
{
	std::unique_ptr<BBox> bbox{nullptr};
};

}	 // namespace core

#endif