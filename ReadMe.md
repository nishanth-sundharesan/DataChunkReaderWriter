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

This was part of the the assignment at FIEA.

