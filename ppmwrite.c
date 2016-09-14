#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define SIZE 256

void writeP3 (FILE *inFile, FILE *outFile);
void writeP6 (FILE *inFile, FILE *outFile);

typedef struct RGB {
  unsigned char r, g, b;
}RGBPix;

typedef struct Image {
  int width, height;
  RGBPix *data;
}PPMImage;

int main(int argc, char *argv[])
{
  //Make sure the right number of arguments was supplied
  if(argc < 4 || argc > 4) {
    perror("usage: conversion-number in-file.ppm out-file.ppm \n");
  }

  char *outType;                          //Decide which type of conversion to do
  if(strcmp(argv[1], "6")) outType = "P6";
  else if(strcmp(argv[1], "3")) outType = "P3";
  else perror("Please specify the conversion type\n Enter 3 to convert to P3 or 6 to convert to P6\n");

  FILE *outFile = fopen(argv[3], "wb");
  FILE *inFile = fopen(argv[2], "rb");

  if(strcmp(outType,"P6")) writeP6(inFile, outFile);
  else return 0;//writeP3(inFile, outFile);

  //clean up
  (void) fclose(outFile);
  (void) fclose(inFile);
  return EXIT_SUCCESS;
}

//write a new P6 file
void writeP6 (FILE *inFile, FILE *outFile){

  char buff[SIZE], *fb;

  PPMImage image;


  int read, i, j, counter = 1;
  unsigned int width, height, maxColors;
  char red[8], green[8], blue[8];

  fb = fgets(buff, SIZE, inFile);           //Get the magic number first
  printf("%s\n",buff );
  if ( (fb == NULL) || ( strncmp(buff, "P3\n", 3) != 0 ) ) perror("Please provide a P3 .ppm file for conversion\n");
  (void) fprintf(outFile, "P6\n");

  //handle any and all comments
  do
        {
           fb = fgets(buff, SIZE, inFile);
           if( strncmp(buff, "#", 1) == 0) fprintf(outFile, "%s", buff);
           printf("%s",buff);
           if ( fb == NULL ) return;
        } while ( strncmp(buff, "#", 1) == 0 );

  //read in the width and height
  read = sscanf(buff, "%u %u", &image.width, &image.height);

  image.data = (RGBPix *)malloc(sizeof(RGBPix) * image.width * image.height);

  printf("%s\n", buff );

  if(read < 2) perror("File Unreadable. Please check the file format\n");
  //read in the max colors
  read = fscanf(inFile, "%u", &maxColors);

  //put all that good stuff into the outFile
  fprintf(outFile, "%u %u\n%u\n",image.width, image.height, maxColors);

  for (i = 0; i < image.width * image.height; i++)
  {
      int curVal;
      fscanf(inFile, "%d", &curVal);
      image.data[i].r = curVal;
      fscanf(inFile, "%d", &curVal);
      image.data[i].g = curVal;
      fscanf(inFile, "%d", &curVal);
      image.data[i].b = curVal;

    }

      fwrite(image.data, 3 * image.width, image.height, outFile);
}

// void writeP3 (FILE *inFile, FILE *outFile){
//
//   char buff[SIZE], *fb;
//
//   Pix *image;
//   Pix curPix;
//
//   int read, i, j, counter = 1;
//   unsigned int width, height, colors;
//   unsigned char red, green, blue;
//
//   fb = (char *)malloc(sizeof(char) * SIZE);
//   fb = fgets(buff, SIZE, inFile);
//   if ( (fb == NULL) || ( strncmp(buff, "P6\n", 3) != 0 ) ) perror("Please provide a P6 .ppm file for conversion\n");
//   (void) fprintf(outFile, "P3\n");
//
//   do
//         {
//            fb = fgets(buff, SIZE, inFile);
//            if( strncmp(buff, "#", 1) == 0) fprintf(outFile, "%s", buff);
//            if ( fb == NULL ) return;
//         } while ( strncmp(buff, "#", 1) == 0 );
//
//   read = sscanf(buff, "%u %u", &width, &height);
//
//
//     image = (Pix *)malloc(sizeof(Pix) * width * height);
//
//   if(read < 2) perror("File Unreadable. Please check the file format\n");
//   read = fscanf(inFile, "%u", &colors);
//
//   fprintf(outFile, "%u %u\n%u\n",width, height, colors);
//   for (i = 0; i < width*height; i++)
//   {
//
//       fread(curPix, 3, 1, inFile);
//
//       image[i] = curPix;
//   }
//
//   for (i = 0; i < width*height; i++)
//   {
//     curPix = image[i];
//     fprintf(outFile, " %d %d %d ", curPix[1], curPix[2], curPix[0]);
//
//     if (counter == width)
//       {
//           fprintf(outFile, "\n");
//           counter = 1;
//       }
//
//       else
//           counter++;
//   }
// }
