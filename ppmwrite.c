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

  char *outType;
  char fh[256];                        //Decide which type of conversion to do
  if(strcmp(argv[1], "6")) outType = "P6";
  else if(strcmp(argv[1], "3")) outType = "P3";
  else {
    perror("Please specify the conversion type\n Enter 3 to convert to P3 or 6 to convert to P6\n");
    return 0;
  }

  if(strstr(argv[2], ".ppm") == NULL) {
    perror("Please provide a .ppm file for conversion");
    return 0;
  }
  if(strstr(argv[3], ".ppm") == NULL) {
    perror("Please provide a valid .ppm file name to write to");
    return 0;
  }
  FILE *outFile = fopen(argv[3], "wb");
  FILE *inFile = fopen(argv[2], "rb");

  if(strcmp(outType,"P6")) writeP6(inFile, outFile);
  else writeP3(inFile, outFile);

  //clean up
  (void) fclose(outFile);
  (void) fclose(inFile);
  return EXIT_SUCCESS;
}

//write a new P6 file
void writeP6 (FILE *inFile, FILE *outFile){

  char buff[SIZE], *fh;

  PPMImage image;


  int read, i, counter = 1;
  unsigned int width, height, maxColors;
  char red[8], green[8], blue[8];

  fh = fgets(buff, SIZE, inFile);           //Get the magic number first
  printf("%s\n",buff );
  if ( (fh == NULL) || ( strncmp(buff, "P3\n", 3) != 0 ) ) perror("Please provide a P3 .ppm file for conversion\n");
  (void) fprintf(outFile, "P6\n");

  //handle any and all comments
  do
        {
           fh = fgets(buff, SIZE, inFile);
           if( strncmp(buff, "#", 1) == 0) fprintf(outFile, "%s", buff);
           printf("%s",buff);
           if ( fh == NULL ) return;
        } while ( strncmp(buff, "#", 1) == 0 );

  //read in the width and height
  read = sscanf(buff, "%u %u", &image.width, &image.height);

  if(read < 2) {
    perror("File Unreadable. Please check the file format\n");
    return;
  }

  image.data = (RGBPix *)malloc(sizeof(RGBPix) * image.width * image.height);


  //read in the max colors
  read = fscanf(inFile, "%u", &maxColors);

  if(maxColors != 255 || read != 1) {
    perror("Please provide an 24-bit color file");
    return;
  }

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

void writeP3 (FILE *inFile, FILE *outFile){

  char buff[SIZE], *fh;

  PPMImage image;

  int read, i, j, counter = 1;
  unsigned int width, height, maxColors;
  unsigned char red, green, blue;

  fh = (char *)malloc(sizeof(char) * SIZE);
  fh = fgets(buff, SIZE, inFile);
  if ( (fh == NULL) || ( strncmp(buff, "P6\n", 3) != 0 ) ) perror("Please provide a P6 .ppm file for conversion\n");
  (void) fprintf(outFile, "P3\n");

  do
        {
           fh = fgets(buff, SIZE, inFile);
           if( strncmp(buff, "#", 1) == 0) fprintf(outFile, "%s", buff);
           if ( fh == NULL ) return;
        } while ( strncmp(buff, "#", 1) == 0 );

  //read in the width and height
  read = sscanf(buff, "%u %u", &image.width, &image.height);


  if(read < 2) {
    perror("File Unreadable. Please check the file format\n");
    return;
  }
  image.data = (RGBPix *)malloc(sizeof(RGBPix) * image.width * image.height);

  read = fscanf(inFile, "%u", &maxColors);
  if(maxColors != 255 || read != 1) {
    perror("Please provide an 24-bit color file");
    return;
  }


  //put all that good stuff into the outFile
  fprintf(outFile, "%u %u\n%u\n",image.width, image.height, maxColors);

  fread(image.data, sizeof(RGBPix), image.width * image.height, inFile);

  for (i = 0; i < image.width * image.height; i++)
  {
      fprintf(outFile, " %d %d %d ", image.data[i].g, image.data[i].b, image.data[i].r);

      if(counter == image.width) {
        fprintf(outFile, "\n");
        counter = 1;
      }
      else counter += 1;
    }

}
