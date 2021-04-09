#pragma once
#include "precompile_header.h"

namespace ui
{
    class GuiImageData
    {
    protected:
        std::unique_ptr<INativeImage> image;
        int frameIndex;

    public:
        GuiImageData();
        GuiImageData(std::unique_ptr<INativeImage> _image, int _frameIndex);
        ~GuiImageData();
        std::unique_ptr<INativeImage> GetImage();

        int GetFrameIndex();
    };
}  // namespace ui