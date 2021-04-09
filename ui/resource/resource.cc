#include "resource.h"

namespace ui
{
/***********************************************************************
GuiImageData
***********************************************************************/

		GuiImageData::GuiImageData()
			:frameIndex(-1)
		{
		}

		GuiImageData::GuiImageData(std::unique_ptr<INativeImage> _image, int _frameIndex)
			: image(_image)
			, frameIndex(_frameIndex)
		{
		}

		GuiImageData::~GuiImageData()
		{
		}

		std::unique_ptr<INativeImage> GuiImageData::GetImage()
		{
			return image;
		}

		vint GuiImageData::GetFrameIndex()
		{
			return frameIndex;
		}
}  // namespace ui