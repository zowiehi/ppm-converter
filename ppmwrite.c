/*
Created by Zowie Haugaard
9/16/16
PPM Converter
  This program is used to convert between the ascii and binary versions of the
  .ppm image file format.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define SIZE 256

void writeP3 (FILE *inFile, FILE *outFile);
void writeP6 (FILE *inFile, FILE *outFile);

//represents a single pixel object
typedef struct RGB {
  unsigned char r, g, b;
}RGBPix;

//this struct is used to store the entire image data, along with the width and height
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
  char fh[256];
                          //Decide which type of conversion to do
  if(strcmp(argv[1], "6")) outType = "P6";
  else if(strcmp(argv[1], "3")) outType = "P3";     //error handling
  else {
    perror("Please specify the conversion type\n Enter 3 to convert to P3 or 6 to convert to P6\n");
    return 0;
  }

  //make sure they are providing a .ppm file
  if(strstr(argv[2], ".ppm") == NULL) {
    perror("Please provide a .ppm file for conversion");
    return 0;
  }
  if(strstr(argv[3], ".ppm") == NULL) {
    perror("Please provide a valid .ppm file name to write to");
    return 0;
  }
  //open the files
  FILE *outFile = fopen(argv[3], "wb");
  FILE *inFile = fopen(argv[2], "rb");

  //call the appropriate method to read and write the data
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
  unsigned int maxColors;


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

  //allocate memory for the image buffer
  image.data = (RGBPix *)malloc(sizeof(RGBPix) * image.width * image.height);


  //read in the max colors
  read = fscanf(inFile, "%u", &maxColors);

  //make sure we are using 8 bit color representation
  if(maxColors != 255 || read != 1) {
    perror("Please provide an 24-bit color file");
    return;
  }

  //put all that good stuff into the outFile
  fprintf(outFile, "%u %u\n%u\n",image.width, image.height, maxColors);

  //read the ascii file, convert the characters to int, and store it in our buffer
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
      //write the binary out
      fwrite(image.data, 3 * image.width, image.height, outFile);
}

//write a p3 file
void writeP3 (FILE *inFile, FILE *outFile){

  //buffer used for the comments mainly
  char buff[SIZE], *fh;

  PPMImage image;

  int read, i, j, counter = 1;
  unsigned int maxColors;


  fh = (char *)malloc(sizeof(char) * SIZE);
  fh = fgets(buff, SIZE, inFile);             //Make sure we are reading the right type of file
  if ( (fh == NULL) || ( strncmp(buff, "P6\n", 3) != 0 ) ) perror("Please provide a P6 .ppm file for conversion\n");
  (void) fprintf(outFile, "P3\n");

  //get rid of comments
  do
        {
           fh = fgets(buff, SIZE, inFile);      //write the comments into the out file
           if( strncmp(buff, "#", 1) == 0) fprintf(outFile, "%s", buff);
           if ( fh == NULL ) return;
        } while ( strncmp(buff, "#", 1) == 0 );

  //read in the width and height
  read = sscanf(buff, "%u %u", &image.width, &image.height);


  //throw error if the width and height aren't in the file
  if(read < 2) {
    perror("File Unreadable. Please check the file format\n");
    return;
  }
  image.data = (RGBPix *)malloc(sizeof(RGBPix) * image.width * image.height);

  read = fscanf(inFile, "%u", &maxColors);
  //check that the right color format is used
  if(maxColors != 255 || read != 1) {
    perror("Please provide an 24-bit color file");
    return;
  }


  //put all that good stuff into the outFile
  fprintf(outFile, "%u %u\n%u\n",image.width, image.height, maxColors);

  //read the image into the buffer
  fread(image.data, sizeof(RGBPix), image.width * image.height, inFile);

  //write out to the ascii file
  for (i = 0; i < image.width * image.height; i++)
  {
      fprintf(outFile, " %d %d %d ", image.data[i].g, image.data[i].b, image.data[i].r);

      //format handling
      if(counter == image.width) {
        fprintf(outFile, "\n");
        counter = 1;
      }
      else counter += 1;
    }

}
