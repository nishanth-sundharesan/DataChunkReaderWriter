//#include <stdio.h>
//
////Defining boolean
//typedef char bool;
//#define true 1
//#define false 0
//
////Function Declarations/Prototyping
//bool AllocateMemory(char **randomBytes, short **randomShorts, int **randomLongs, int *startRange, int *endRange);
//void DeAllocateMemory(char *randomBytes, short *randomShorts, int *randomLongs);
//void AddRandomValue(char *randomBytes, short *randomShorts, int *randomLongs);
//void DisplayTemp(char *randomBytes);
//
//
//
////File names
//#define FILENAME_CONFIG_TO_READ	"AppData/Config.txt"
//
////Config file range names
//#define BYTE_RANGE "ByteRange"
//#define SHORT_RANGE "ShortRange"
//#define LONG_RANGE "LongRange"
//
//#define RANGE_HEADING_SIZE  20
//
//const int NumberOfChunks = 3;
//const char RangeNames[3][RANGE_HEADING_SIZE] = { { "ByteRange" },{ "ShortRange" },{ "LongRange" } };
//
//int main(int argc, char* argv[])
//{
//	//Declaring file pointers
//	FILE *configFileToRead = NULL;
//
//	//Declaring range for each chunk
//	int startRange[3], endRange[3];
//
//	int numberOfSections;
//
//	bool isReadConfigFile = true;
//	char rangeHeading[RANGE_HEADING_SIZE];
//
//	char *randomBytes = NULL;
//	short *randomShorts = NULL;
//	int *randomLongs = NULL;
//
//	//Open the config file to read
//	configFileToRead = fopen(FILENAME_CONFIG_TO_READ, "r");
//
//	for (int i = 0; i < NumberOfChunks; i++)
//	{
//		fscanf(configFileToRead, "%s", rangeHeading);
//		if (strcmp(rangeHeading, RangeNames[i]) == 0)
//		{
//			fscanf(configFileToRead, "%d %d", &startRange[i], &endRange[i]);
//			if (startRange[i] >= endRange[i])
//			{
//				isReadConfigFile = false;
//				break;
//			}
//		}
//		else
//		{
//			isReadConfigFile = false;
//			break;
//		}
//	}
//
//	if (!isReadConfigFile)
//	{
//		printf("Unable to read config file.");
//		return 0;
//	}
//
//	if (!AllocateMemory(&randomBytes, &randomShorts, &randomLongs, startRange, endRange))
//	{
//		printf("Unable to Allocate memory.");
//		DeAllocateMemory(randomBytes, randomShorts, randomLongs);
//		return 0;
//	}
//
//	//printf("Enter the number of sections: ");
//	//scanf_s("%d", &numberOfSections);
//
//
//	AddRandomValue(randomBytes, randomShorts, randomLongs);
//	DisplayTemp(randomBytes);
//
//	/*printf("\n Char: %d", sizeof(char));
//	printf("\n Char: %d", sizeof(short));
//	printf("\n Char: %d", sizeof(int));*/
//
//	DeAllocateMemory(randomBytes, randomShorts, randomLongs);
//	getchar();
//}
//
//bool AllocateMemory(char **randomBytes, short **randomShorts, int **randomLongs, int *startRange, int *endRange)
//{
//	if ((*randomBytes = (char *)malloc(endRange[0] - startRange[0])) != NULL)
//	{
//		if ((*randomShorts = (short *)malloc(endRange[1] - startRange[1])) != NULL)
//		{
//			if ((*randomLongs = (int *)malloc(endRange[2] - startRange[2])) != NULL)
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//void DeAllocateMemory(char *randomBytes, short *randomShorts, int *randomLongs)
//{
//	free(randomBytes);
//	free(randomShorts);
//	free(randomLongs);
//}
//
//void AddRandomValue(char *randomBytes, short *randomShorts, int *randomLongs)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		*(randomBytes) = i;
//		randomBytes++;
//	}
//
//	randomBytes = randomBytes - (10 * sizeof(char));
//}
//
//void DisplayTemp(char *randomBytes)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		printf("\n%d", *randomBytes);
//		randomBytes++;
//	}
//
//}
