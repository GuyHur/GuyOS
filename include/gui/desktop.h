#ifndef __GUYOS__GUI__DESKTOP_H
#define __GUYOS__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace guyos
{
    namespace gui
    {
        
        class Desktop : public CompositeWidget, public guyos::drivers::MouseEventHandler
        {
        protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;
            
        public:
            Desktop(common::int32_t w, common::int32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();
            
            void Draw(common::GraphicsCon* gc);
            
            void OnMouseDown(guyos::common::uint8_t button);
            void OnMouseUp(guyos::common::uint8_t button);
            void OnMouseMove(int x, int y);
        };
        
    }
}


#endif