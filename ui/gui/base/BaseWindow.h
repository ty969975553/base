#pragma once
#include "ui/gui/base/IGuiElement.h"

namespace ui
{
    class BaseWindow : public IGuiElement
    {
        public:
            bool alwaysTop_;
            bool modalWindow_;
        protected:
            bool initialized_;
            bool destructStart_;

            mutable bool updateRender_;

    };
}  // namespace gui