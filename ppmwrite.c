#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


#define SIZE 256

void writeP3 (FILE *inFile, FILE *outFile);
void writeP6 (FILE *inFile, FILE *outFile);
int main(int argc, char *argv[])
{
  if(argc < 4 || argc > 4) {
    perror("usage: conversion-number in-file.ppm out-file.ppm \n");
  }

  char *outType;
  if(strcmp(argv[1], "6")) outType = "P3";
  else if(strcmp(argv[1], "3")) outType = "P6";
  else perror("Please specify the conversion type\n Enter 3 to convert to P3 or 6 to convert to P6\n");

  FILE *outFile = fopen(argv[3], "wb");
  FILE *inFile = fopen(argv[2], "rb");

  if(strcmp(outType,"P3")) writeP3(inFile, outFile);
  else return NULL; //writeP3(inFile, outFile, dimx, dimy);


  (void) fclose(outFile);
  (void) fclose(inFile);
  return EXIT_SUCCESS;
}

void writeP3 (FILE *inFile, FILE *outFile){

  char buff[SIZE], *fb;
  int read, i, j, counter = 1;
  unsigned int width, height, colors;
  unsigned char red, green, blue;

  fb = fgets(buff, SIZE, inFile);
  if ( (fb == NULL) || ( strncmp(buff, "P6\n", 3) != 0 ) ) perror("Please provide a P6 .ppm file for conversion\n");
  (void) fprintf(outFile, "P3\n");

  // do
  //       {
  fb = fgets(buff, SIZE, inFile);
  //          fprintf(outFile, "%s\n", buff);
  //          if ( fb == NULL ) return;
  //       } while ( strncmp(buff, "#", 1) == 0 );
  //
  read = sscanf(buff, "%u %u", &width, &height);

  if(read < 2) perror("File Unreadable. Please check the file format\n");
  read = fscanf(inFile, "%u", &colors);

  fprintf(outFile, "%u %u\n%u\n",width, height, colors);
  for (i = 0; i < width*height; i++)
  {
      fread(&red, 1, 1, inFile);
      fread(&green, 1, 1, inFile);
      fread(&blue, 1, 1, inFile);
      fprintf(outFile, "%d %d %d ", red, green, blue);

      if (counter == width)
        {
            fprintf(outFile, "\n");
            counter = 1;
        }

        else
            counter++;
    }
}
