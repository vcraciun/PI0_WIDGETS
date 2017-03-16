#ifndef __LCDDRIVER__
#define __LCDDRIVER__

#include "Common.h"
#include "terminus.h"

#define PORTRAIT 0
#define LANDSCAPE 1
#define R_PORTRAIT 2
#define R_LANDSCAPE 3

#define RESX_PORTRAIT 240
#define RESY_PORTRAIT 320
#define RESX_LANDSCAPE 320
#define RESY_LANDSCAPE 240

#define RESX RESX_PORTRAIT
#define RESY RESY_PORTRAIT

#define RGB(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

#define BLACK             RGB( 0x00, 0x00, 0x00 )
#define WHITE             RGB( 0xFF, 0xFF, 0xFF )
#define RED               RGB( 0xFF, 0x00, 0x00 )
#define GREEN             RGB( 0x00, 0xFF, 0x00 )
#define BLUE              RGB( 0x00, 0x00, 0xFF )
#define CYAN              RGB( 0x00, 0xFF, 0xFF )
#define MAGENTA           RGB( 0xFF, 0x00, 0xFF )
#define YELLOW            RGB( 0xFF, 0xFF, 0x00 )
#define GRAY              RGB( 0x80, 0x80, 0x80 )
#define LIGHT_GRAY        RGB( 0xC0, 0xC0, 0xC0 )
#define DARK_ORANGE       RGB( 0xFF, 0x80, 0x80 )
#define VIOLET            RGB( 0x80, 0x80, 0xFF )
#define MAROON            RGB( 0xFF, 0x80, 0x80 )
#define WORM_GREEN        RGB( 0x80, 0xFF, 0x80 )
#define LIGHT_RED1        RGB( 0xFF, 0xC0, 0xC0 )
#define LIGHT_BLUE        RGB( 0xC0, 0xC0, 0xFF )
#define AQUA              RGB( 0x80, 0xFF, 0xFF )
#define PINK              RGB( 0xFF, 0x80, 0xFF )
#define LIGHT_YELLOW      RGB( 0xFF, 0xFF, 0x80 )
#define LIME              RGB( 0x10, 0xFF, 0x80 )
#define LIGHT_RED         RGB( 0xFF, 0x80, 0x80 )
#define DARK_RED          RGB( 0x80, 0x00, 0x00 )
#define DARK_BLUE         RGB( 0x00, 0x00, 0x80 )
#define DARK_GREEN        RGB( 0x00, 0x80, 0x00 )
#define OLIVE             RGB( 0x80, 0x80, 0x00 )
#define NAVY              RGB( 0x00, 0x80, 0x80 )
#define ORANGE            RGB( 0xFF, 0x80, 0x40 )

typedef struct _DIB{
    uint32_t header_sz;
    uint32_t Width;
    uint32_t Height;
    uint16_t color_planes;
    uint16_t bpp;
    uint32_t compresion_method;
    uint32_t raw_bmp_size;
    uint32_t horiz_res;
    uint32_t vert_res;
    uint32_t nr_colors;
    uint32_t important_colors;  
}__attribute__((packed))DIB;

typedef struct _BMP{
    uint16_t BMP_sig;
    uint32_t BMP_size;
    uint32_t reserved;
    uint32_t bmpoffset; 
    DIB dib_data;
}__attribute__((packed))BMP;

//extern  BMP *splashscreen;

//definitii pentru RPI-0 (40 pin header)
enum LCDPins {
    CS = 20,
    RS = 16,
    WR = 12,
    RD = 1,
    RESET = 7,
    D0 = 2,
    D1 = 3,
    D2 = 4,
    D3 = 17,
    D4 = 27,
    D5 = 22,
    D6 = 10,
    D7 = 9,
    D8 = 11,
    D9 = 0,
    D10 = 5,
    D11 = 6,
    D12 = 13,
    D13 = 19,
    D14 = 26,
    D15 = 21
};

class LCDDriver{
    private:
        LCDDriver(uint32_t orientation);
        ~LCDDriver();

        static LCDDriver *instance;
        
        void PortSetup();
        void WriteCmd(uint16_t cmd);
        void WriteData(uint16_t data);        
        void LCDInit(); 

        uint16_t orientation;
        uint16_t resx;
        uint16_t resy;
        font_t*  _font;

    public:
        static LCDDriver *GetInstance(uint32_t orientation = PORTRAIT);

        void LCDSetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
        void WriteCmdData(uint16_t cmd, uint16_t data);  

        void LCDFill(uint16_t color);
        void RectFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
        void Circle(uint16_t x,uint16_t y,uint16_t radius,uint16_t fill,uint16_t color);
        void Rectangle(uint16_t x1,uint16_t y1,uint16_t w,uint16_t h,uint16_t color);
        void Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
        void HLine(uint16_t x1,uint16_t y1,uint16_t w,uint16_t color);
        void VLine(uint16_t x1,uint16_t y1,uint16_t h,uint16_t color);
        void Pixel(uint16_t x,uint16_t y,uint16_t color);
        
        void SetFont( font_t* font );
        font_t* GetFont();
		uint8_t GetFontWidth();
        uint8_t GetFontHeight();
        
        void PrintChar( char c, uint16_t x, uint16_t y, uint16_t fgColor = -1);
        void Print( const char *str, uint16_t x, uint16_t y, uint16_t fgColor = -1);
		void DisplayBMP(uint16_t x,uint16_t y,unsigned char *buffer);
};

#endif
