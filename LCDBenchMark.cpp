#include "LCDBenchMark.h"
#include "LCDDriver.h"

LCDBenchMark *LCDBenchMark::instance=0;
LCDDriver *LCDBenchMark::lcd=0;

LCDBenchMark::LCDBenchMark()
{
    lcd = LCDDriver::GetInstance();
}

LCDBenchMark::~LCDBenchMark(){}

LCDBenchMark *LCDBenchMark::GetInstance()
{
    if (!instance)
        instance = new LCDBenchMark();
    return instance;
}

void LCDBenchMark::LineTestPerformance()
{
    struct timespec start, stop;
    double timp;
    uint16_t x1,y1,x2,y2;
    int i;
    int number=5000;
    
    srand(time(NULL));
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    
    for (i=0;i<number;i++)
    {
        x1=rand()%RESX;
        y1=rand()%RESY;
        x2=rand()%RESX;
        y2=rand()%RESY;
        lcd->Line(x1,y1,x2,y2,rand());
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);    
    timp = ((stop.tv_sec * 1000 + stop.tv_nsec/1000000) - (start.tv_sec * 1000 + start.tv_nsec/1000000)); 
    timp/=1000;
    printf("LineTest took %0.3f seconds for [%d] lines\n",timp,number);       
    printf("Line rate = %.3f lps\n",number/timp);      
}

void LCDBenchMark::FillRectTestPerformance()
{
    struct timespec start, stop;
    double timp;
    uint16_t x,y,w,h;
    int i;
    int number=20000;
    
    srand(time(NULL));
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    
    for (i=0;i<number;i++)
    {
        x=rand()%RESX;
        y=rand()%RESY;
        w=rand()%(RESX-x);
        h=rand()%(RESY-y);
        lcd->RectFill(x,y,w,h,rand());
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);    
    timp = ((stop.tv_sec * 1000 + stop.tv_nsec/1000000) - (start.tv_sec * 1000 + start.tv_nsec/1000000)); 
    timp/=1000;
    printf("FillRectTest took %0.3f seconds for [%d] shapes\n",timp,number);       
    printf("Shape rate = %.3f rps\n",number/timp);     
}

void LCDBenchMark::ScreenFillTestPerformance()
{
    struct timespec start, stop;
    double timp;
    int i;
    int number=1000;
    
    srand(time(NULL));
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for (i=0;i<number;i++)
        lcd->LCDFill(rand());
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    timp = ((stop.tv_sec * 1000 + stop.tv_nsec/1000000) - (start.tv_sec * 1000 + start.tv_nsec/1000000)); 
    timp/=1000;
    printf("FillTest took %0.3f seconds for [%d] frames\n",timp,number);       
    printf("Fill rate = %.3f fps\n",number/timp);  
}

void LCDBenchMark::CreateRandomText(char *buffer, int max_chars)
{
    int i,count;
    char dict[100]="abc<def>gh,i!jk.lm@n/op#qrs$t|uv%wxy^zA}B&CD?E F*GHI(JKL)MN{O-PQ_RS+TUV=WXY~Z012`3456[789]";
        
    count = rand()%max_chars;
    for (i=0;i<count;i++)
        buffer[i]=dict[rand()%90];    
    buffer[count]=0;
}

void LCDBenchMark::TextPerformance(font_t* font)
{    
    char **lines;
    int number=500;
    struct timespec start, stop;
    double timp;
    int i,j;
    int cline=0;
    char *clearline;
    
    lcd->SetFont(font);
    srand(time(NULL));
    lines=(char**)malloc((RESY/font->Height)*sizeof(char*));
    clearline=(char*)malloc(RESX/font->Width+1);
    memset(clearline,' ',RESX/font->Width);
    clearline[RESX/font->Width]=0;
    for (i=0;i<RESY/font->Height;i++)
    {
        lines[i]=(char*)malloc(RESX/font->Width+1);
        memset(lines[i],0,RESX/font->Width+1);
    }
    lcd->LCDFill(WHITE);    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for (i=0;i<number;i++)
    {
        if (cline==RESY/font->Height)
        {
            for (j=1;j<cline;j++)
                strcpy(lines[j-1],lines[j]);                
            for (j=0;j<cline-1;j++)        
            {
                lcd->RectFill(0,j*font->Height,RESX,font->Height,WHITE);
                lcd->Print(lines[j],0,j*font->Height,BLACK,WHITE);            
            }
            lcd->RectFill(0,j*font->Height,RESX,font->Height,WHITE);
            cline--;
        }        
        CreateRandomText(lines[cline],RESX/font->Width);
        lcd->Print(lines[cline],0,cline*font->Height,BLACK,WHITE);
        cline++;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    for (i=0;i<RESY/font->Height;i++)
        free(lines[i]);
    free(lines);
    free(clearline);
    timp = ((stop.tv_sec * 1000 + stop.tv_nsec/1000000) - (start.tv_sec * 1000 + start.tv_nsec/1000000)); 
    timp/=1000;
    printf("TextScrollTest took %0.3f seconds for [%d] lines\n",timp,number);       
    printf("Text Scroll rate = %.3f lps\n",number/timp);
}

void LCDBenchMark::BMPTest(const char *foldername, bool cache)
{    
    DIR *dir;
    struct dirent *ent;
    uint8_t *buffer;
    FILE *input_fd;
    int size,ret_in;
    struct timespec start, stop;
    double timp;
    int poze;
    char full_path[256];
    char **fnames;
    uint8_t **buffers;
    int *files_size;
    int i,name_index;
    
    poze=0;    
    
    if (cache)
    {
        if ((dir = opendir (foldername)) != NULL) 
        {
            while ((ent = readdir (dir)) != NULL) 
            {
                if (ent->d_type != DT_DIR && (strstr(ent->d_name,".bmp") || strstr(ent->d_name,".BMP")))
                    poze++;
            }
            closedir (dir);
        } 
        if (!poze)
        {
            printf("Nu am gasit poze\n");
            return;
        }
        printf("Found %d pictures\n",poze);
        
        fnames = (char**)malloc(poze * sizeof(char*));
        buffers = (uint8_t**)malloc(poze * sizeof(uint8_t*));
        files_size = (int*)malloc(poze * sizeof(int));
        for (i=0;i<poze;i++)
            fnames[i] = (char*)malloc(50);
        if ((dir = opendir (foldername)) != NULL) 
        {
            while ((ent = readdir (dir)) != NULL) 
            {
                if (ent->d_type != DT_DIR && (strstr(ent->d_name,".bmp") || strstr(ent->d_name,".BMP")))
                {                
                    name_index = atoi(ent->d_name)-1;
                    sprintf(fnames[name_index],"%s/%s",foldername,ent->d_name);
                    input_fd = fopen (fnames[name_index], "r");
                    if (input_fd == NULL) {
                        printf("open failed\n");
                        return;
                    }        
                    fseek(input_fd,0,SEEK_END);
                    files_size[name_index]=ftell(input_fd);
                    rewind(input_fd);
                    buffers[name_index] = (uint8_t*)malloc(files_size[name_index]);
                    ret_in = fread (buffers[name_index], 1, files_size[name_index], input_fd);
                    if (!ret_in || ret_in != files_size[name_index])
                        printf("Unable to read file : %s!\n",strerror(errno));
                    fclose(input_fd); 
                }
            }
            closedir (dir);
        }
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        for (i=0;i<poze;i++)
            lcd->DisplayBMP(0,0,buffers[i]); 
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        
        free(files_size);
        for (i=0;i<poze;i++)
        {
            free(fnames[i]);    
            free(buffers[i]);
        }
        free(fnames);
        free(buffers);
    }
    else
    {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    
        if ((dir = opendir (foldername)) != NULL) 
        {
            while ((ent = readdir (dir)) != NULL) 
            {
                if (ent->d_type != DT_DIR)
                {                                
                    sprintf(full_path,"%s/%s",foldername,ent->d_name);
                    input_fd = fopen (full_path, "r");
                    if (input_fd == NULL) {
                        printf("open failed\n");
                        return;
                    }         
                    fseek(input_fd,0,SEEK_END);
                    size=ftell(input_fd);
                    rewind(input_fd);
                    buffer = (uint8_t*)malloc(size);
                    memset(buffer,0,size);
                    ret_in = fread (buffer, 1, size, input_fd);
                    fclose(input_fd);
                    lcd->DisplayBMP(0,0,buffer);
                    free(buffer);  
                    poze++;
                }
            }
            closedir (dir);
        } 
        else
            printf("Cannot open folder [%s]\n",foldername);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    }
    timp = ((stop.tv_sec * 1000 + stop.tv_nsec/1000000) - (start.tv_sec * 1000 + start.tv_nsec/1000000)); 
    timp/=1000;
    printf("BMPTest took %0.3f seconds for [%d] pictures\n",timp,poze);    
    printf("Picture rate = %.3f pfs\n",poze/timp);  
}

void LCDBenchMark::BMPAnimation(const char *foldername,uint16_t x,uint16_t y)
{
    DIR *dir;
    struct dirent *ent;
    unsigned char *buffer;
    FILE *input_fd;
    int size,ret_in;
    struct timespec start, stop;
    double timp;
    int poze;
    char full_path[256];
    char **fnames;
    int *files_size;
    uint8_t **buffers;
    int name_index;
    int i,j;
    int number=1000;
        
    poze=0;
    if ((dir = opendir (foldername)) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL) 
        {
            if (ent->d_type != DT_DIR && (strstr(ent->d_name,".bmp") || strstr(ent->d_name,".BMP")))
                poze++;
        }
        closedir (dir);
    } 
    if (!poze)
    {
        printf("Nu am gasit poze\n");
        return;
    }
    printf("Found %d pictures\n",poze);
    
    fnames = (char**)malloc(poze * sizeof(char*));
    buffers = (uint8_t**)malloc(poze * sizeof(uint8_t*));
    files_size = (int*)malloc(poze * sizeof(int));
    for (i=0;i<poze;i++)
        fnames[i] = (char*)malloc(50);
    if ((dir = opendir (foldername)) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL) 
        {
            if (ent->d_type != DT_DIR && (strstr(ent->d_name,".bmp") || strstr(ent->d_name,".BMP")))
            {                
                name_index = atoi(ent->d_name)-1;
                sprintf(fnames[name_index],"%s/%s",foldername,ent->d_name);
                input_fd = fopen (fnames[name_index], "r");
                if (input_fd == NULL) {
                    printf("open failed\n");
                    return;
                }        
                fseek(input_fd,0,SEEK_END);
                files_size[name_index]=ftell(input_fd);
                rewind(input_fd);
                buffers[name_index] = (uint8_t*)malloc(files_size[name_index]);
                ret_in = fread (buffers[name_index], 1, files_size[name_index], input_fd);
                if (!ret_in || ret_in != files_size[name_index])
                    printf("Unable to read file : %s!\n",strerror(errno));
                fclose(input_fd); 
            }
        }
        closedir (dir);
    }
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    lcd->DisplayBMP(x,y,buffers[0]);  
    /*for (i=0;i<number;i++)
    {          
        lcd->RectFill(x,y,*((uint32_t*)(buffers[i%poze]+0x12)),*((uint32_t*)(buffers[i%poze]+0x16)),WHITE);
        if (x+*((uint32_t*)(buffers[i%poze]+0x12))<RESX)
            x+=1;
        else
        {
            x=0;
            y+=*((uint32_t*)(buffers[i%poze]+0x16));
            if (y+*((uint32_t*)(buffers[i%poze]+0x16))>RESY)
                y=0;
        }
        lcd->DisplayBMP(x,y,buffers[i%poze]);      
    }*/
    for (i=0;i<number;i++)
        lcd->DisplayBMP(x,y,buffers[i%poze]); 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    
    free(files_size);
    for (i=0;i<poze;i++)
    {
        free(fnames[i]);    
        free(buffers[i]);
    }
    free(fnames);
    free(buffers);
    
    timp = ((stop.tv_sec * 1000 + stop.tv_nsec/1000000) - (start.tv_sec * 1000 + start.tv_nsec/1000000)); 
    timp/=1000;
    printf("BMPAnimation took %0.3f seconds for [%d] frames\n",timp,number);    
    printf("Frame rate = %.3f fps\n",number/timp);      
}
