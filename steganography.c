#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *openImageFile();

unsigned char *readImageHeader(FILE *fp);

unsigned char *readImageBytes(FILE *fp, unsigned char *secretMessage);

void setLSB(unsigned char *imageByte);

unsigned char *readSecretMessage();

void confirmSecretMessage(unsigned char *secretMessage);

void embedSecretMessage(unsigned char *secretMessage, unsigned char *imageByte);

void mainArguments(int argc, char *argv[]);

void printBinary(unsigned char byte);

void help();

void prepareUI();

void freeUI();

void confirmUI();

void compressing(int argc, char *argv[]);

void decompressing(int argc, char *argv[]);

void helpCompressing();

void helpDecompressing();

char* UI[4];


int main(int argc, char *argv[])
{
    prepareUI();
    mainArguments(argc, argv);
    confirmUI();

    unsigned char *secretMessage = readSecretMessage();
    confirmSecretMessage(secretMessage);

    FILE* fpImage = openImageFile();

    unsigned char *imageHeader = readImageHeader(fpImage);

    /*
    printf("\nimage header: ");
    for(int i=0; i<(9*strlen((const char*)imageHeader)); i++) printf("%X ", imageHeader[i]);
    printf("\n");
    */

    unsigned char *imageByte = readImageBytes(fpImage, secretMessage);
    setLSB(imageByte);

    embedSecretMessage(secretMessage, imageByte);

    freeUI();
    return 0;   
}


//Functions
void prepareUI()
{
    for(int i=0; i<4; i++) UI[i] = (char*) realloc(UI[i], 1);
}

void freeUI()
{
    for(int i=0; i<4; i++) free(UI[i]);
}

unsigned char* readSecretMessage()
{
    FILE *fp = fopen(UI[3], "r");
    if(fp == NULL) {printf("\nFile %s does not exist or could not be opened.\n", UI[3]); exit(0);}

    int characters=0;
    for(char c=getc(fp);c != EOF; c=getc(fp))characters++;
    unsigned char *secretMessage = (unsigned char*) calloc(characters, sizeof(char));

    rewind(fp);
    for(int i=0;i <characters; i++)
    {
        char c = getc(fp);
        secretMessage[i] = c;
    }
    fclose(fp);
    return secretMessage;
}

void confirmSecretMessage(unsigned char *secretMessage)
{
    char answer;
    printf("\nSecret message:\n\n%s\n\n",secretMessage);
    fflush(stdin);
    printf("Secret message correct? (y/n)");
    scanf("%c",&answer);
    if(answer == 'y' || answer == 'Y') printf("\nNice!!\n");
    else {printf("\nPlease check '%s' file\n", UI[3]); exit(0);}
    return;
}

FILE* openImageFile()
{
    FILE *fp = fopen(UI[1], "rb");
    if(fp == NULL){printf("\nFile %s does not exist or could not be opened.\n", UI[1]); exit(0);}
    return fp;
}

unsigned char* readImageHeader(FILE *fp)
{
    unsigned char *imageHeader = (unsigned char*) calloc(54, 1); //1 of sizeof(char)?
    fread(imageHeader, 1, 54, fp);
    return imageHeader;
}

unsigned char* readImageBytes(FILE *fp, unsigned char *secretMessage)
{
    int length = 8*strlen((const char*)secretMessage);
    printf("\nlength string: %d\nLSBs needed: %d\n\n", length/8, length);
    unsigned char *imageByte = (unsigned char*) calloc(length, 1);
    fread(imageByte, 1, length, fp);
    return imageByte;
}

void setLSB(unsigned char *imageByte)
{
    for(int i=0; i<strlen((const char*)imageByte); i++)
    {
        printf("%3d: ", i+1);
        imageByte[i] &= 0b11111110;
        printBinary(imageByte[i]);
    }
}

void embedSecretMessage(unsigned char *secretMessage, unsigned char *imageByte)
{
    int byteCount=0, characters=strlen((const char*)secretMessage);
    for(int j=0; j<characters; j++)
    {
        printf("\n\ncharacter %d:\n", j+1);
        for(int i=0; i<8; i++)
        {
            if(secretMessage[j] & 0b00000001) imageByte[byteCount]+=1;
            secretMessage[j] >>= 1;
            printBinary(imageByte[byteCount]);
            byteCount++;
        }
    }
}

void printBinary(unsigned char byte)
{
    //printf("\nhexdecimal: %02X",byte);
    printf("Binary: ");
    for(int i=7; i>=0; i--)
    {
        unsigned char bit = ((byte >> i) & 1);
        printf("%d", bit);
    }
    printf("\n");
}

void mainArguments(int argc, char* argv[])
{
   if(argc<2) help();
   if(strcmp(argv[1], "--help")==0) help();

   for(int i=1; i<argc; i++)
   {
       if(strcmp(argv[i], "-c")==0)
       {
           if(argv[i+1]==NULL) helpCompressing();
           UI[0] = (char*) calloc(strlen(argv[i]), 1);
           strcpy(UI[0], "-c");
           compressing(argc, argv);
           return;
       }

       else if(strcmp(argv[i], "-d")==0)
       {
           if(argv[i+1]==NULL)helpDecompressing();
           UI[0] = (char*) calloc(strlen(argv[i]), 1);
           strcpy(UI[0], "-d");
           decompressing(argc, argv);
           return;
       }
       else continue;
   }
   help();
}

void compressing(int argc, char *argv[])
{
    for(int i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "-s")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-i")==0) || (strcmp(argv[i+1],"-o")==0) ) {printf("\n-s: No filename/path.\n"); helpCompressing();}
            UI[3] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[3], argv[i+1]);
            i++;
        }

        else if(strcmp(argv[i], "-i")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-s")==0) || (strcmp(argv[i+1],"-o")==0) ) {printf("\n-i: No filename/path.\n"); helpCompressing();}
            UI[1] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[1], argv[i+1]);
            i++;
        }

        else if(strcmp(argv[i], "-o")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-s")==0) || (strcmp(argv[i+1],"-i")==0)) {printf("\n-o: No filename/path.\n"); helpCompressing();}
            UI[2] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[2], argv[i+1]);
            i++;
        }
        else continue;
    }
}

void decompressing(int argc, char *argv[])
{
    for(int i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "-i")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-s")==0) || (strcmp(argv[i+1],"-o")==0) ) {printf("\n-i: No filename/path.\n"); helpDecompressing();}
            UI[1] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[1], argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "-o")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-s")==0) || (strcmp(argv[i+1],"-i")==0)) {printf("\n-o: No filename/path.\n"); helpDecompressing();}
            UI[2] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[2], argv[i+1]);
            i++;
        }
        else continue;
    } 
}

void confirmUI()
{
    char answer;
    if(strcmp(UI[0], "-c")==0)
    {
        printf("\ncompressing inputs:");
        printf("\n-s: %s", UI[3]);
        printf("\n-i; %s", UI[1]);
        printf("\n-o: %s\n\n", UI[2]);
        printf("Are the filenames/paths correct? (y/n)");
        scanf(" %c", &answer);
        if(answer== 'y' || answer == 'Y') return;
        else{printf("\nexiting program.\n"); helpCompressing();}
    }

    else if(strcmp(UI[0], "-d")==0)
    {
        printf("\ndecompressing inputs:");
        printf("\n-i; %s", UI[1]);
        printf("\n-o: %s\n\n", UI[2]);
        printf("Are the filenames/paths correct? (y/n)");
        scanf(" %c", &answer);
        if(answer== 'y' || answer == 'Y') return;
        else{printf("\nexiting program.\n"); helpDecompressing();}
    }
}

void help()
{
    printf("\ncompressing: ./program -c -s <secretMessageIn.txt> -i <inputImage> -o <outputImage>\n");
    printf("\n  -c                 --> compress");
    printf("\n  -s <filename/path> --> .txt message to compress in image");
    printf("\n  -i <filename/path> --> input .bmp file to compress message in");
    printf("\n  -o <filename/path> --> output .bmp file with compressed message\n\n");

    printf("\ndecompressing: ./program -d -i <inputImage> -o <secretMessageOut.txt>\n");
    printf("\n  -d                 --> decompress");
    printf("\n  -i <filename/path> --> input bmp file with compressed message");
    printf("\n  -o <filename/path> --> ouput .txt file with compressed message");
    printf("\n\n"); 

    exit(0);
}

void helpCompressing()
{
    printf("\nto compress, enter: ./program -c -s <secretMessageIn.txt> -i <inputImage> -o <outputImage>    enter ./program --help for more info\n\n");
    exit(0);
}

void helpDecompressing()
{
    printf("\nto decompress, enter: ./program -d -i <inputImage> -o <secretMessageOut.txt>    enter ./program --help for more info\n\n");
    exit(0);
}