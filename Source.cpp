#include "LCDDriver.h"
#include "terminus.h"

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

int main(void)
{
    int i;
    uint16_t color[] = {RED,GREEN,BLUE,CYAN,MAGENTA,YELLOW,GRAY,ORANGE, VIOLET, MAROON, AQUA, LIGHT_RED, DARK_GREEN,LIGHT_BLUE,PINK,LIME,OLIVE,NAVY};
    
    LCDDriver *lcd = LCDDriver::GetInstance(); 
    SetupFonts();    

    lcd->LCDFill(BLACK);
    lcd->Rectangle(RESX/2-100,RESY/2-100,200,200,AQUA);
    lcd->Rectangle(RESX/2-101,RESY/2-101,202,202,AQUA);
    lcd->Rectangle(0,0,RESX,RESY,LIGHT_RED1);
    lcd->Rectangle(1,1,RESX-2,RESY-2,LIGHT_RED1);  
    lcd->HLine(1,RESY/2,RESX-1,RGB(0xFF,0xFF,0xFF));
    lcd->VLine(RESX/2,0,RESY-1,RGB(0xFF,0xFF,0xFF));
    lcd->Line(0,0,239,319,0xFFE0);
    lcd->Line(0,319,239,0,0xFFE0);
    lcd->Circle(120,160,80,0,0x07E0);
    lcd->Circle(120,160,81,0,0x07E0);
    lcd->RectFill(RESX/2-25,RESY/2-25,50,50,ORANGE);
    delay(5000);
    
    lcd->LCDFill(WHITE);
    lcd->SetFont(&Font8x16);
    lcd->Print("Salut DAN",0,0,BLACK);
    lcd->SetFont(&TerminusFont);
    lcd->Print("Salut DAN",0,30,BLACK);
    lcd->SetFont(&TerminusBigFont);
    lcd->Print("Salut DAN",0,100,BLACK);
    delay(5000);
    
	FILE *input_fd = fopen ("poza.bmp", "r");	
	unsigned char *buffer;
    if (input_fd == NULL) {
		printf("open failed\n");
        return 0;
    }
	fseek(input_fd,0,SEEK_END);
	int size=ftell(input_fd);
	printf("Size = %d\n",size);	
	rewind(input_fd);
	buffer = (unsigned char*)malloc(size);
	memset(buffer,0,size);
	printf("Alloc: %08X\n",buffer);
    int ret_in = fread (buffer, 1, size, input_fd);
	printf("read: %d\n",ret_in);
	fclose(input_fd);
	printf("display BMP\n");
	lcd->DisplayBMP(0,0,buffer);
	printf("free buffer\n");
	free(buffer);
    /*for (i=0;i<sizeof(color)/2;i++)
    {
        lcd->RectFill(50,50,250,140,color[i]);
        delay(500);
    }*/
    
    return 0;
}