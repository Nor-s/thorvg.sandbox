#ifndef _CORE_CANVAS_PAINT_WRAPPER_H_
#define _CORE_CANVAS_PAINT_WRAPPER_H_

#include <thorvg.h>
#include <tvgCommon.h>
namespace core
{

class PaintWrapper
{
public:
    //todo: add dtor
    virtual ~PaintWrapper()
    {
        if(mHandle)
        mHandle->unref();
    }

    virtual void scale(const tvg::Size& size) {
        return;
    }

	tvg::Paint* mHandle;
};

class PictureWrapper : public PaintWrapper
{
public:
    static std::unique_ptr<PictureWrapper> Gen(std::string_view path) 
    {
        std::unique_ptr<PictureWrapper> pictureWrapper = std::make_unique<PictureWrapper>();
        auto* picture = tvg::Picture::gen();
        pictureWrapper->mHandle = picture;
        pictureWrapper->mHandle->ref();
        picture->load(path.data());
        return pictureWrapper;
    }
public:
    ~PictureWrapper() = default;

    void scale(const tvg::Size& size) override
    {
        auto* picture = static_cast<tvg::Picture*>(mHandle);
        float scale = 0.0f, w2 = 0.0f, h2 = 0.0f;
        picture->size(&w2, &h2);

        if (w2 > h2) 
            scale = size.x / w2;
        else
            scale = size.y / h2;

        picture->scale(scale);
        return;
    }

private:
};

}	 // namespace core

#endif