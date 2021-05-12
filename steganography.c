#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* openFile();

void mainArguments(int argc, char* argv[]);

void printBinary(unsigned char byte);

void help();

void prepareUI();

void freeUI();

void printUI();

void compressing(int argc, char *argv[]);

void decompressing(int argc, char *argv[]);

void helpCompressing();

void helpDecompressing();

char* UI[4];


int main(int argc, char* argv[])
{
    prepareUI();

    mainArguments(argc, argv);

    printUI();
    
    unsigned char header[55];

    FILE* fp = openFile();

    fread(header, 1, 54, fp);
    for(int i=0; i<55; i++)
    {
        printf("%X ", header[i]);
    }
    printf("\n");

    int i=0;
    for(unsigned char c=getc(fp); i<2; c=getc(fp))
    {
        printBinary(c);
        c &= 0b11111110;
        printBinary(c);
        printf("\n");
        i++;
    }
    printf("\n");

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
    for(int i=0; i<4; i++)
    {
        free(UI[i]);
    }
}

FILE* openFile()
{

    FILE *fp = fopen(UI[1], "rb");
    if(fp == NULL)
    {
        printf("\nFile %s does not exist or could not be opened.\n", UI[1]);
        exit(EXIT_FAILURE);
    }
    return fp;
}

void printBinary(unsigned char byte)
{
    printf("\ndecimal: %d",byte);
    printf("\nhexdecimal: %02X",byte);
    printf("\nBinary: ");
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
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-i")==0) || (strcmp(argv[i+1],"-o")==0) )
            {
                printf("\n-s: No filename/path.\n");
                helpCompressing();
            }
            UI[3] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[3], argv[i+1]);
            i++;
        }

        else if(strcmp(argv[i], "-i")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-s")==0) || (strcmp(argv[i+1],"-o")==0) )
            {
                printf("\n-i: No filename/path.\n");
                helpCompressing();
            }

            UI[1] = (char*) calloc(strlen(argv[i+1]), 1);
            strcpy(UI[1], argv[i+1]);
            i++;
        }

        else if(strcmp(argv[i], "-o")==0)
        {
            if( (argv[i+1]==NULL) || (strcmp(argv[i+1],"-s")==0) || (strcmp(argv[i+1],"-i")==0))
            {
                printf("\n-o: No filename/path.\n");
                helpCompressing();
            }

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
            printf("\n-i: %s",argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "-o")==0)
        {
            printf("\n-o: %s",argv[i+1]);
            i++;
        }
        else continue;
    } 
}

void printUI()
{
    char answer;
    if(strcmp(UI[0], "-c")==0)
    {
        printf("\ncompressing inputs:");
        printf("\n-s: %s", UI[3]);
        printf("\n-i; %s", UI[1]);
        printf("\n-o: %s\n\n", UI[2]);
        printf("Are this the filenames/paths you want? (y/n)");
        scanf(" %c", &answer);
        if(answer== 'y' || answer == 'Y')
        {
            printf("\nstarting encoding...\n\n");
            return;
        } 
        else
        {
            printf("\nexiting program, re-run program with correct filenames/paths\n");
            helpCompressing();
        }
    }

    else if(strcmp(UI[0], "-d")==0)
    {
        printf("\ndecompressing inputs:");
        printf("\n-i; %s", UI[1]);
        printf("\n-o: %s\n\n", UI[2]);
        printf("Are this the filenames/paths you want? (y/n)");
        scanf(" %c", &answer);
        if(answer== 'y' || answer == 'Y')
        {
            printf("\nstarting decoding...\n");
            return;
        } 
        else
        {
            printf("\nexiting program, re-run program with correct filenames/paths\n");
            helpDecompressing();
        }
        return;
    }
}

void help()
{
    printf("\ncompressing: ./program -c -s <secretMessageIn.txt> -i <inputImage> -o <outputImage>");
    printf("\n  -c                 --> compress");
    printf("\n  -s <filename/path> --> .txt message to compress in image");
    printf("\n  -i <filename/path> --> input .bmp file to compress message in");
    printf("\n  -o <filename/path> --> output .bmp file with compressed message\n");

    printf("\ndecompressing: ./program -d -i <inputImage> -o <secretMessageOut.txt>");
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