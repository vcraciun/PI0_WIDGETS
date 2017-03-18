#ifndef __LCDBENCHMARK__
#define __LCDBENCHMARK__

#include "Common.h"
#include "LCDDriver.h"

class LCDBenchMark{
    private:
        LCDBenchMark();
        ~LCDBenchMark();
        
        static LCDDriver *lcd;        
        static LCDBenchMark *instance;
        
        void CreateRandomText(char *buffer, int count);
        
    public:
        static LCDBenchMark *GetInstance();
        
        void LineTestPerformance();
        void FillRectTestPerformance();
        void ScreenFillTestPerformance();
        void TextPerformance(font_t* font);
        void BMPTest(const char *foldername,bool cache);
        void BMPAnimation(const char *folder,uint16_t x,uint16_t y);
};

#endif
