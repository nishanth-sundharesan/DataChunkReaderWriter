/*	Format of the Output Binary File:
*	1. Number Of Sections (32 bits)
*	2. First Section Size (32 bits)
*	3. Char Data's Size (32 bits)
*	4. Char Data (8 bits for each char)
*	5. Short Data's Size (32 bits)
*	6. Short Data (16 bits for each short)
*	7. Int Data's Size (32 bits)
*	8. Int Data (32 bits for each Int)
*	9. Second Section Size (32 bits)
*	10. Repeat 3-8 for second section
*	11. Third Section Size (32 bits)
*	12. Repeat 3-8 for third section
*	13. ...
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Defining boolean
typedef char bool;
#define true 1
#define false 0

#define RANGE_HEADING_SIZE  20
#define NumberOfChunks 3

//Defining constants based on the given problem
const char RangeNames[NumberOfChunks][RANGE_HEADING_SIZE] = { { "ByteRange" },{ "ShortRange" },{ "LongRange" } };
const int MinSections = 0;
const int MaxSections = 50;

enum TYPE { CHAR, SHORT, INT } types;

//Function declarations/prototyping
bool readConfigFiles(FILE *file, int startRange[], int endRange[]);
void insertRandomValues(unsigned int randomSize, int startValueRange, int maxValue, float *average, enum TYPE t, FILE *outputBinaryFileToWrite);
unsigned short *convertEndianFormat16Bits(unsigned short *number);
unsigned int *convertEndianFormat32BitsRef(unsigned int *number);
unsigned int convertEndianFormat32BitsValue(unsigned int number);
int generateRandomNumbers(int startSizeRange, int endSizeRange);
void clearInputBuffer();
int readSectionNumbers();

//File names
#define FILENAME_SIZE_RANGE_TO_READ "AppData/SizeRange.txt"
#define FILENAME_VALUE_RANGE_TO_READ "AppData/ValueRange.txt"
#define FILENAME_OUTPUT_STAT_TO_WRITE "OutputFiles/OutputStatisticsFile.txt"
#define FILENAME_OUTPUT_BINARY_TO_WRITE "OutputFiles/OutputBinaryFile.dat"

//Config file range names
#define BYTE_RANGE "ByteRange"
#define SHORT_RANGE "ShortRange"
#define LONG_RANGE "LongRange"

int main(int argc, char* argv[])
{		
	//Declaring file pointers
	FILE *sizeRangeFileToRead = NULL;
	FILE *valueRangeFileToRead = NULL;
	FILE *outputStatFileToWrite = NULL;
	FILE *outputBinaryFileToWrite = NULL;

	//Declaring the size range for each chunk
	int startSizeRange[NumberOfChunks], endSizeRange[NumberOfChunks];
	//Declaring the value range for each types
	int startValueRange[NumberOfChunks], endValueRange[NumberOfChunks];

	unsigned int numberOfSections = 0;
	float average = 0.0f;

	//Declaring random size variables for each chunk
	unsigned int randomSizeBytes = 0;
	unsigned int randomSizeShorts = 0;
	unsigned int randomSizeLongs = 0;

	//Open the files to read & write	
	if (fopen_s(&sizeRangeFileToRead, FILENAME_SIZE_RANGE_TO_READ, "r") != 0)
	{
		printf("Unable to open the file %s", FILENAME_SIZE_RANGE_TO_READ);
		return 0;
	}
	if (fopen_s(&valueRangeFileToRead, FILENAME_VALUE_RANGE_TO_READ, "r") != 0)
	{
		printf("Unable to open the file %s", FILENAME_VALUE_RANGE_TO_READ);
		return 0;
	}
	if (fopen_s(&outputStatFileToWrite, FILENAME_OUTPUT_STAT_TO_WRITE, "w") != 0)
	{
		printf("Unable to open the file %s", FILENAME_OUTPUT_STAT_TO_WRITE);
		return 0;
	}
	if (fopen_s(&outputBinaryFileToWrite, FILENAME_OUTPUT_BINARY_TO_WRITE, "wb") != 0)
	{
		printf("Unable to open the file %s", FILENAME_OUTPUT_BINARY_TO_WRITE);
		return 0;
	}

	//Read the config files
	bool isReadConfigFiles = true;
	if (!readConfigFiles(sizeRangeFileToRead, startSizeRange, endSizeRange))
	{
		printf("Unable to read the file %s \n", FILENAME_SIZE_RANGE_TO_READ);
		return 0;
	}
	if (!readConfigFiles(valueRangeFileToRead, startValueRange, endValueRange))
	{
		printf("Unable to read the file %s \n", FILENAME_VALUE_RANGE_TO_READ);
		return 0;
	}

	//Read the user input
	numberOfSections = readSectionNumbers();

	//Seeding the random number with time.
	srand((unsigned)time(NULL));

	//Output the number of sections to both text and binary file
	fprintf_s(outputStatFileToWrite, "User chose %d sections", numberOfSections);
	fwrite(convertEndianFormat32BitsRef(&numberOfSections), sizeof(int), 1, outputBinaryFileToWrite);   //Convert the numberOfSections to BigEndian format so that it's easier to read from the assembly language
	numberOfSections = convertEndianFormat32BitsValue(numberOfSections);								//Converting the numberOfSections back to the littleEndian format
	for (unsigned int i = 0; i < numberOfSections; i++)
	{
		fprintf_s(outputStatFileToWrite, "\nSection %d", i + 1);

		//Getting the random size for each chunk
		randomSizeBytes = generateRandomNumbers(startSizeRange[0], endSizeRange[0]);
		randomSizeShorts = generateRandomNumbers(startSizeRange[1], endSizeRange[1]);
		randomSizeLongs = generateRandomNumbers(startSizeRange[2], endSizeRange[2]);

		//Output the size of each section to the binary file.
		unsigned int sizeOfSection = convertEndianFormat32BitsValue((randomSizeBytes % 2 == 1 ? randomSizeBytes + 1 : randomSizeBytes) +
			(randomSizeShorts * sizeof(short)) + (randomSizeLongs*(sizeof(int)) + (sizeof(int) * NumberOfChunks)));
		fwrite(&sizeOfSection, sizeof(int), 1, outputBinaryFileToWrite);

		insertRandomValues(randomSizeBytes, startValueRange[0], (endValueRange[0] - startValueRange[0]), &average, CHAR, outputBinaryFileToWrite);
		fprintf_s(outputStatFileToWrite, "\n%d bytes, average value %.2f", randomSizeBytes, average);
		insertRandomValues(randomSizeShorts, startValueRange[1], (endValueRange[1] - startValueRange[1]), &average, SHORT, outputBinaryFileToWrite);
		fprintf_s(outputStatFileToWrite, "\n%d shorts, average value %.2f", randomSizeShorts, average);
		insertRandomValues(randomSizeLongs, startValueRange[2], (endValueRange[2] - startValueRange[2]), &average, INT, outputBinaryFileToWrite);
		fprintf_s(outputStatFileToWrite, "\n%d longs, average value %.2f", randomSizeLongs, average);
	}

	//Closing file pointers
	fclose(sizeRangeFileToRead);
	fclose(valueRangeFileToRead);
	fclose(outputStatFileToWrite);
	fclose(outputBinaryFileToWrite);
}

/*
*	Function:readSectionNumbers
*	-----------------------------------
*	Description: Reads the section number between MinSections & MaxSections only. Doesn't allow to enter other characters.
*	Returns: Returns the section number.
*/
int readSectionNumbers()
{	
	int inputValue;
	while (true)
	{
		printf("Enter the number of sections(0 to %d): ", MaxSections);
		scanf_s("%d", &inputValue);
		if (inputValue > MinSections && inputValue <= MaxSections)
		{
			break;
		}
		clearInputBuffer();
		printf("\n");
	}
	return inputValue;
}

/*
*	Function:readConfigFiles
*	-----------------------------------
*	Description: Reads the size ranges and value ranges from the file specified. It also checks if the config files have proper heading.*				 
*	Parameters: 
*				*file = file pointer from where the data should be read
*				startRange[] = stores the start range of size/value
*				endRange[] = stores the end range of size/value
*	Returns: Returns true if the read is successful or else it returns false
*			 It also returns false if there is an invalid heading in the config files or if the ranges are unsound.
*/
bool readConfigFiles(FILE *file, int startRange[], int endRange[])
{
	bool isReadConfigFiles = true;
	char rangeHeading[RANGE_HEADING_SIZE];

	for (int i = 0; i < NumberOfChunks; i++)
	{
		fscanf_s(file, "%s", rangeHeading, sizeof(rangeHeading));
		if (strcmp(rangeHeading, RangeNames[i]) == 0)
		{
			fscanf_s(file, "%d %d", &startRange[i], &endRange[i]);
			if (startRange[i] < 0 || endRange[i] < 0 || (startRange[i] > endRange[i]))
			{
				isReadConfigFiles = false;
				break;
			}
		}
		else
		{
			isReadConfigFiles = false;
			break;
		}
	}
	return isReadConfigFiles;
}

/*
*	Function: insertRandomValues
*	-----------------------------------
*	Description: This function takes in a 'randomSize' parameter and generates a 'randomSize' number of random values between startValueRange & endValueRange based on the data type.
*				 It then puts the 'randomSize' and the random values generated to the binary file. It also calculates the average for the random values which is put to the Output Statistics file.
*	Parameters:
*				randomSize = the number of random values to be generated
*				startValueRange = the value from where the generated random number should start.
*				maxValue = endValueRange - startValueRange
*				*average = the average of the generated random numbers
*				*outputBinaryFileToWrite = a pointer to the binary file where the random values and it's sizes are outputted
*/
void insertRandomValues(unsigned int randomSize, int startValueRange, int maxValue, float *average, enum TYPE dataType, FILE *outputBinaryFileToWrite)
{	
	unsigned char charValue = 0;
	unsigned short shortValue = 0;
	unsigned int intValue = 0;

	unsigned long long total = 0;
	unsigned int randomSizeBigEndian = (convertEndianFormat32BitsValue(randomSize));

	if (dataType == CHAR)
	{
		//Output the size of char/byte chunk
		fwrite(&randomSizeBigEndian, sizeof(int), 1, outputBinaryFileToWrite);
		if (randomSize % 2 == 1)
		{
			//If the Random Size is ODD then add an additional padding of 1 byte to make the address as even number
			fwrite(&charValue, sizeof(char), 1, outputBinaryFileToWrite);
		}
		//Print the random char/byte values
		for (unsigned int j = 0; j < randomSize; j++)
		{
			charValue = (rand() % maxValue) + startValueRange;
			total += charValue;
			fwrite(&charValue, sizeof(char), 1, outputBinaryFileToWrite);
		}
	}
	else if (dataType == SHORT)
	{
		//Output the size of short/word chunk
		fwrite(&randomSizeBigEndian, sizeof(int), 1, outputBinaryFileToWrite);
		//Print the random short/word values
		for (unsigned int j = 0; j < randomSize; j++)
		{
			shortValue = (rand() % maxValue) + startValueRange;
			total += shortValue;
			fwrite(convertEndianFormat16Bits(&shortValue), sizeof(short), 1, outputBinaryFileToWrite);
		}
	}
	else if (dataType == INT)
	{
		//Output the size of int/long chunk
		fwrite(&randomSizeBigEndian, sizeof(int), 1, outputBinaryFileToWrite);
		//Print the random int/long values
		for (unsigned int j = 0; j < randomSize; j++)
		{
			intValue = (rand() % maxValue) + startValueRange;
			total += intValue;
			fwrite(convertEndianFormat32BitsRef(&intValue), sizeof(int), 1, outputBinaryFileToWrite);
		}
	}
	*average = (randomSize == 0 ? 0.0f : (float)total / randomSize);
}

/*
*	Function: convertEndianFormat16Bits
*	-----------------------------------
*	Description: Converts the 16-bit number from BigEndian format to LittleEndian format and vice-versa
*	Parameters:
*				*number = the number whose format should be converted
*	Returns: Returns the address of the converted number
*/
unsigned short *convertEndianFormat16Bits(unsigned short *number)
{
	*number = (*number >> 8) | (*number << 8);
	return number;
}

/*
*	Function: convertEndianFormat32BitsRef
*	-----------------------------------
*	Description: Converts the 32-bit number from BigEndian format to LittleEndian format and vice-versa
*	Parameters:
*				*number = the number whose format should be converted
*	Returns: Returns the address of the converted number
*/
unsigned int *convertEndianFormat32BitsRef(unsigned int *number)
{
	*number = ((*number >> 24) & 0xff) | // Move byte 3 to byte 0
		((*number << 8) & 0xff0000) |	 // Move byte 1 to byte 2
		((*number >> 8) & 0xff00) |		 // Move byte 2 to byte 1
		((*number << 24) & 0xff000000);  // Move byte 0 to byte 3

	return number;
}

/*
*	Function: convertEndianFormat32BitsValue
*	-----------------------------------
*	Description: Converts the 32-bit number from BigEndian format to LittleEndian format and vice-versa
*	Parameters:
*				number = the number whose format should be converted
*	Returns: Returns the converted number
*/
unsigned int convertEndianFormat32BitsValue(unsigned int number)
{
	return ((number >> 24) & 0xff) |	// Move byte 3 to byte 0
		((number << 8) & 0xff0000) |	// Move byte 1 to byte 2
		((number >> 8) & 0xff00) |		// Move byte 2 to byte 1
		((number << 24) & 0xff000000);  // Move byte 0 to byte 3
}

/*
*	Function: clearInputBuffer
*	-----------------------------------
*	Description: Pulls out all the characters from the input buffer until it reaches a new line.	
*/
void clearInputBuffer()
{
	while (getchar() != '\n');	
}

/*
*	Function: generateRandomNumbers
*	-----------------------------------
*	Description: Generates a random number between startSizeRange and endSizeRange
*	Returns: Returns the generated Random number.
*/
int generateRandomNumbers(int startSizeRange, int endSizeRange)
{
	if (endSizeRange - startSizeRange == 0)
	{
		return startSizeRange;
	}
	else
	{
		return (rand() % (endSizeRange - startSizeRange)) + startSizeRange;
	}

}
