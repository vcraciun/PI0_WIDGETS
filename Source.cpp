#include "LCDDriver.h"
#include "terminus.h"
#include "LCDBenchMark.h"

void SetupFonts(void)
{
	TerminusFont.Offset=32;
	TerminusFont.Width=8;
	TerminusFont.Height=12;
	TerminusFont.Position=TerminusCharOffsets;
	TerminusFont.Data=TerminusFontBitmaps;
	
	Font8x16.Offset=32;
	Font8x16.Width=8;
	Font8x16.Height=16;
	Font8x16.Position=Font8x16Offsets;
	Font8x16.Data=Font8x16Chars;
	
	TerminusBigFont.Offset=32;
	TerminusBigFont.Width=16;
	TerminusBigFont.Height=28;
	TerminusBigFont.Position=TerminusBigCharOffsets;
	TerminusBigFont.Data=TerminusBigFontBitmaps;	
}

void PrimitiveGrafice()
{
    LCDDriver *lcd = LCDDriver::GetInstance(); 
    
    lcd->LCDFill(WHITE);
    lcd->Rectangle(RESX/2-100,RESY/2-100,200,200,BLUE);
    lcd->Rectangle(RESX/2-101,RESY/2-101,202,202,BLUE);
    lcd->Rectangle(0,0,RESX,RESY,RED);
    lcd->Rectangle(1,1,RESX-2,RESY-2,RED);  
    lcd->HLine(1,RESY/2,RESX-1,DARK_GREEN);
    lcd->VLine(RESX/2,0,RESY-1,DARK_GREEN);
    lcd->Line(0,0,239,319,ORANGE);
    lcd->Line(0,319,239,0,ORANGE);
    lcd->Circle(120,160,80,0,PINK);
    lcd->Circle(120,160,81,0,PINK);
    lcd->RectFill(RESX/2-25,RESY/2-25,50,50,YELLOW);
    delay(5000);
}

void FontTest()
{
    LCDDriver *lcd = LCDDriver::GetInstance(); 
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("What's app duuuude",0,0,BLACK,WHITE);
    lcd->SetFont(&TerminusFont);
    lcd->Print("Raspberry PI Zero",0,40,WHITE,BLACK);
    lcd->SetFont(&TerminusBigFont);
    lcd->Print("Salut Nicu",0,100,YELLOW,BLUE);
    delay(5000);    
}

int main(void)
{
    int i;
    uint16_t color[] = {RED,GREEN,BLUE,CYAN,MAGENTA,YELLOW,GRAY,ORANGE, VIOLET, MAROON, AQUA, LIGHT_RED, DARK_GREEN,LIGHT_BLUE,PINK,LIME,OLIVE,NAVY};
    
    LCDDriver *lcd = LCDDriver::GetInstance(); 
    LCDBenchMark *benchmark = LCDBenchMark::GetInstance();
    SetupFonts();        
        
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("      PRIMITIVE GRAFICE       ",0,160,WHITE,BLUE);
    printf("Primitives TEST\n");
    delay(5000);        
    PrimitiveGrafice();
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("          FONT  TEST          ",0,160,WHITE,BLUE);
    printf("FONT Test\n");
    delay(5000);        
    FontTest();
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("   FULL SCREEN RANDOM COLOR   ",0,160,WHITE,BLUE);    
    printf("*** FILL Screen TEST ***\n");
    delay(5000); 
    benchmark->ScreenFillTestPerformance();
    delay(5000);        
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("     RANDOM  LINES   TEST     ",0,160,WHITE,BLUE);        
    printf("*** Line TEST ***\n");
    delay(5000); 
    lcd->LCDFill(WHITE);
    benchmark->LineTestPerformance(); 
    delay(5000);        

    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("     RANDOM  SQUARE  TEST     ",0,160,WHITE,BLUE);        
    printf("*** Rectangle TEST ***\n");
    delay(5000); 
    lcd->LCDFill(WHITE);
    benchmark->FillRectTestPerformance();    
    delay(5000);            
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("   RANDOM  TEXT-SCROLL TEST   ",0,160,WHITE,BLUE);            
    printf("*** TextScroll TEST ***\n");
    delay(5000); 
    lcd->LCDFill(WHITE);
    benchmark->TextPerformance(&TerminusBigFont); 
    lcd->LCDFill(WHITE);
    benchmark->TextPerformance(&TerminusFont); 
    lcd->LCDFill(WHITE);
    benchmark->TextPerformance(&Font8x16);  
    delay(5000);            
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("        ANIMATION TEST        ",0,160,WHITE,BLUE);                
    printf("*** Animation TEST ***\n");
    delay(5000); 
    lcd->LCDFill(WHITE);
    benchmark->BMPAnimation("anim",RESX/2-23,RESY/2-33);
    delay(5000);        
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("        24BPP BMP TEST        ",0,160,WHITE,BLUE);                    
    printf("*** Bitmaps TEST 24BPP ***\n");
    delay(5000); 
    lcd->LCDFill(WHITE);
    benchmark->BMPTest("poze24",true);    
    delay(5000);        
    
    return 0;
}