#pragma once
#include "precompile_header.h"
#include "graphics/gui_element.h"
using Ptr = std::unique_ptr;

template<class Args>
class GuiEvent
{
private:
    Ptr<GuiElement> ;
    /* data */
public:
    GuiEvent(const std::string& name, );
    ~GuiEvent();
};


