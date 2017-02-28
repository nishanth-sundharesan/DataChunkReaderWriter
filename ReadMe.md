# Data Chunk Reader Writer

#### Description:  

The C program creates a binary file which will contain N(0 < N < 50) sections of data, where N is specified by the user (ie. How many sections of data the user wants?). Each section will consist of three chunks of data.
 The first chunk will be a random (100 < NUM_BYTES < 500) number of bytes, followed by a random (100 < NUM_SHORTS < 500) number of shorts, followed by a random (100 < NUM_LONGS < 500) number of longs.
 The byte values themselves will be unsigned and random (50 < BYTEVAL<150) . The short values will also be random (1000 < SHORTVAL < 2000). The long values will also be random (3000 < LONGVAL < 5000).
 The range of NUM_BYTES, NUM_SHORTS and NUM_LONGS is provided in an input text file that the C program reads.  

 - Size Range input file is present in "AppData\SizeRange.txt"

 - Value Range input file is present in "AppData\ValueRange.txt"

The stats file present in "OutputFiles\OutputStatisticsFile.txt" will show the number of pieces of data in each chunk, along with the average value, rounded to 2 decimal places. An example output might look like:

User chose 35 sections.  
Section 1  
238 bytes, average value 101.73  
401 shorts, average value 1487.25  
104 longs, average value 4345.32  
 …  
Section 35  
123 bytes, average value 101.32  
481 shorts, average value 1581.15  
342 longs, average value 4125.12

The assembly program will then be run and ask the user for a section number to use in its computations. The assembly program will spit out the averages for the three chunks in the section specified by the user. The values should match the text output given by the C program.

This was part of the assignment at FIEA.


#### To run the program:

1. Open the "DataChunkReaderWriter.sln" using Visual Studio.  
2. Run the project and enter the number of sections(0 to 50).  
3. Open the "OutputFiles\OutputStatisticsFile.txt" in a window.  
4. Now open the assembly file "Assembly Files\AverageOfChunks.X68" and run it. Details about how to run the assembly program is mentioned below.  
5. Enter a valid section number, whose data you would like to view.
6. Now you can compare the number of pieces in each data chunk and its average value with the data present in "OutputStatisticsFile.txt" which you had previously opened.


#### To run the assembly program:

1. Please download the Easy68k Assembler/Editor/Simulator from "http://www.easy68k.com" (it is an assembler for 68000).
2. Open "AverageOfChunks.X68" (it must open in Easy 68k).
3. Hit play button, which is on the right side of the Menu bar (shortcut key: F9).
4. Hit Execute button.
5. Hit Play button again, this time it is the second button from left on the Menu bar.
