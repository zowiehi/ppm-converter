# ppm-converter

This is a simple PPM conversion application. 
The user enters supplies either a ascii (p3) .ppm file or a binary (p6) .ppm file and the applications converts it to the other format. 

# Usage
To build the executable, simply enter make into the terminal, and the application will be compiled.

To call the application, type ./ppmwrite followed by a 3 to convert from P6 to P3 or a 6 to convert from P3 to P6, the file you wish to convert, and the name of the file you would like to send the output to. an example is below

./ppmwrite 6 mountains.ppm mountainsOut.ppm