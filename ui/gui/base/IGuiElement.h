#pragma once
#include "ui/gui/type.h"
#include <memory>

namespace ui
{  
    class IRenderer;

    class IGuiElement
    {
        public:


        protected:
            unit id_;   //
            unit parentId_;
            bool enabled_; 
            bool visible_;
            bool active_;
            RectWHF rectWHF_;
            float alpha_;

            std::shared_ptr<IRenderer> renderer_;

    };
}`