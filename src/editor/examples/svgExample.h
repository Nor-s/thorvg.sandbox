/*
 * Copyright (c) 2020 - 2025 the ThorVG project. All rights reserved.

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
#ifndef _EDITOR_EXAMPLES_SVG_EXAMPLE_H_
#define _EDITOR_EXAMPLES_SVG_EXAMPLE_H_

#include "exampleBase.h"

/************************************************************************/
/* ThorVG Drawing Contents                                              */
/************************************************************************/

#define NUM_PER_ROW 3
#define NUM_PER_COL 3

struct SvgExample : tvgexam::Example
{
	std::vector<tvg::Picture*> pictures;
	uint32_t w, h;
	uint32_t size;

	int counter = 0;

	std::string_view toString() override
	{
		static std::string str = "svg example";
		return str;
	}

	void populate(const char* path) override
	{
		if (counter >= NUM_PER_ROW * NUM_PER_COL)
			return;

		// ignore if not svg.
		const char* ext = path + strlen(path) - 3;
		if (strcmp(ext, "svg"))
			return;

		auto picture = tvg::Picture::gen();

		if (!tvgexam::verify(picture->load(path)))
			return;

		// image scaling preserving its aspect ratio
		float scale;
		float shiftX = 0.0f, shiftY = 0.0f;
		float w, h;
		picture->size(&w, &h);

		if (w > h)
		{
			scale = size / w;
			shiftY = (size - h * scale) * 0.5f;
		}
		else
		{
			scale = size / h;
			shiftX = (size - w * scale) * 0.5f;
		}

		picture->scale(scale);
		picture->translate((counter % NUM_PER_ROW) * size + shiftX,
						   (counter / NUM_PER_ROW) * (this->h / NUM_PER_COL) + shiftY);

		pictures.push_back(picture);

		cout << "SVG: " << path << endl;

		counter++;
	}

	bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
	{
		// The default font for fallback in case
		pictures.clear();
		counter = 0;

		// Background
		auto shape = tvg::Shape::gen();
		shape->appendRect(0, 0, w, h);
		shape->fill(255, 255, 255);

		canvas->push(shape);

		this->w = w;
		this->h = h;
		this->size = w / NUM_PER_ROW;

		this->scandir(EXAMPLE_DIR "/svg");

		/* This showcase demonstrates the asynchronous loading of tvg.
		   For this, pictures are pushed at a certain sync time.
		   This allows time for the tvg resources to finish loading;
		   otherwise, you can push pictures immediately. */
		for (auto& paint : pictures)
		{
			canvas->push(paint);
		}

		pictures.clear();

		return true;
	}
};

#endif