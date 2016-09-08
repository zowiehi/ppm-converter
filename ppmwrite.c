#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


#define SIZE 256;
int main(int argc, char *argv[])
{
  if(argc < 4 || argc > 4) {
    perror("usage: conversion-number in-file out-file \n");
  }

  char outType[2];
  if(argv[1] == 6) outType = "P3";
  else if(argv[1] == 3) outType = "P6";
  else perror("Please specify the conversion type\n Enter 3 to convert to P3 or 6 to convert to P6\n");

  const int dimx = 800, dimy = 800;
  int i, j;
  FILE *outFile = fopen(argv[3], "wb");
  FILE *inFile = fopen(argv[2], "rb");
  (void) fprintf(outFile, "%s\n%d %d\n255\n", outType, dimx, dimy);

  if(strcmp(outType,"P6")) return writeP6(inFile, outFile, dimx, dimy);
  else return writeP3(inFile, outFile, dimx, dimy);

  (void) fclose(outFile);
  return EXIT_SUCCESS;
}

void writeP6 (FILE *inFile, FILE *outFile, int dimx, int dimy)){

  char buff[SIZE], *fb;

  fb = fgets(buff, SIZE, inFile);
  if(Fb == null) return 

  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
      static unsigned char color[3];
      color[0] = i % 256;  /* red */
      color[1] = j % 256;  /* green */
      color[2] = (i * j) % 256;  /* blue */
      (void) fwrite(color, 1, 3, outFile);
    }
  }
}
