/*
 * Copyright (c) 2023 - 2025 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _EDITOR_EXAMPLSES_ANIMATION_EXAMPLE_H_
#define _EDITOR_EXAMPLSES_ANIMATION_EXAMPLE_H_

#include "exampleBase.h"

/************************************************************************/
/* ThorVG Drawing Contents                                              */
/************************************************************************/

struct AnimationExample : tvgexam::Example
{
	unique_ptr<tvg::Animation> animation;

	std::string_view toString() override
	{
		static std::string str = "animation example";
		return str;
	}

	bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
	{
		// The default font for fallback in case
		tvg::Text::load(EXAMPLE_DIR "/font/Arial.ttf");

		// Animation Controller
		animation = unique_ptr<tvg::Animation>(tvg::Animation::gen());
		auto picture = animation->picture();

		// Background
		auto shape = tvg::Shape::gen();
		shape->appendRect(0, 0, w, h);
		shape->fill(50, 50, 50);

		canvas->push(shape);

		if (!tvgexam::verify(picture->load(EXAMPLE_DIR "/lottie/sample.json")))
			return false;

		// image scaling preserving its aspect ratio
		float scale;
		float shiftX = 0.0f, shiftY = 0.0f;
		float w2, h2;
		picture->size(&w2, &h2);

		if (w2 > h2)
		{
			scale = w / w2;
			shiftY = (h - h2 * scale) * 0.5f;
		}
		else
		{
			scale = h / h2;
			shiftX = (w - w2 * scale) * 0.5f;
		}

		picture->scale(scale);
		picture->translate(shiftX, shiftY);

		canvas->push(picture);

		return true;
	}

	bool update(tvg::Canvas* canvas, uint32_t elapsed) override
	{
		auto progress = tvgexam::progress(elapsed, animation->duration());

		// Update animation frame only when it's changed
		if (animation->frame(animation->totalFrame() * progress) == tvg::Result(0))
		{
			canvas->update();
			return true;
		}

		return false;
	}
};

#endif