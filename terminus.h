#ifndef TFTLCD_TERMINUS_H
#define TFTLCD_TERMINUS_H

#include <stdio.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FontMetrics_struct
{
    uint8_t Offset;
    uint8_t Width;
    uint8_t Height;
    const int16_t* Position;
    const char* Data;
}font_t;

extern font_t TerminusFont;
extern font_t TerminusBigFont;
extern font_t Font8x16;
extern const char TerminusFontBitmaps[];
extern const int16_t TerminusCharOffsets[];
extern const char Font8x16Chars[];
extern const int16_t Font8x16Offsets[];
extern const char TerminusBigFontBitmaps[];
extern const int16_t TerminusBigCharOffsets[];

#ifdef __cplusplus
}
#endif

#endif
