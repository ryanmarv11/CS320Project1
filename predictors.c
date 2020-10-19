#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct BranchPair
{
	unsigned long pc;
	int taken;
};
unsigned createMask(unsigned start_bit, unsigned end_bit);
int alwaysTakenPredictor(char* actual);
int alwaysNotTakenPredictor(char* actual);
char* alwaysPredictorParser(char* argv[]);
struct BranchPair** notAlwaysPredictorParser(char* argv[]);

int main(int argc, char *argv[])
{
/*char* actual = alwaysPredictorParser(argv);
int result = alwaysTakenPredictor(actual);
int i = 0;
printf("Always Taken Result: %d\n", result);
result = alwaysNotTakenPredictor(actual);
printf("Always Not Taken Result: %d\n", result);
unsigned long tester = 0x23;
unsigned test_mask = createMask(0,1);
tester = tester & test_mask;
printf("Tester now is: %#x\n", tester);
*/
struct BranchPair** tester = notAlwaysPredictorParser(argv);
for(int i = 0; i < 2; i++)
{
  printf("Tester[%d] PC: %#lx\tTaken: %d\n", i, tester[i]->pc, tester[i]->taken);
}
return 0;
}

unsigned createMask(unsigned start_bit, unsigned end_bit)
{
	unsigned mask = 0;
	for(unsigned int i = start_bit; i <= end_bit; i++)
		mask |= 1 << i;
	return mask;
}
int alwaysTakenPredictor(char* actual)
{
	int i = 0, correct_predictions = 0;
	while(actual[i] != '\0')
	{
		if(actual[i] == 'N')
		{
			i+=2;
		}
		else if(actual[i] == 'T')
		{
			correct_predictions++;
			i++;
		}
	}
	return correct_predictions;
}

int alwaysNotTakenPredictor(char* actual)
{
	int i = 0, correct_predictions = 0;
	while(actual[i] != '\0')
	{
		if(actual[i] == 'N')
		{
			correct_predictions++;
			i++;
		}
		else
		{
			i++;
		}
	}
	return correct_predictions;
}

char* alwaysPredictorParser(char*argv[])
{
	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		printf("ERROR: Cannot open file.\n");
		exit(1);
	}
	char *ans = malloc(2000);
	int incrementer = 0;
	char c;
	while((c = fgetc(fp)) != EOF)
	{
		if(c == 'T' || c == 'N')
		{
		ans[incrementer++] = (char) c;
		}
	}
	ans[incrementer++] = '\0';
	fclose(fp);
	return ans;
}
struct BranchPair** notAlwaysPredictorParser(char*argv[])
{
	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		printf("ERROR: Cannot open file.\n");
		exit(1);
	}

	struct BranchPair** ans = (struct BranchPair**)malloc(1000000*sizeof(struct BranchPair*));
	for(int i = 0; i < 1000000; i++)
	{
		ans[i] = (struct BranchPair*)malloc(sizeof(struct BranchPair));
	}
	char c, *eptr;
	char temp_address[12];
 	int address_input = 0, parsed_address = 0, parsed_taken = 0, end_of_file = 0;; //"boolean" switches

	unsigned long temp_pc; //temp variables
	int temp_taken;

	int array_iterator = 0, temp_address_i = 0;
	while(end_of_file == 0)
	{
    c = fgetc(fp);
    if(c == EOF)
    {
      printf("I made it here.\n");
			address_input = 0;
			parsed_address = 0;
			parsed_taken = 0;
			ans[array_iterator]-> pc = strtoul(temp_address, &eptr, 16);
			ans[array_iterator]->taken = temp_taken;
			for(int i = 0; i < 12; i++)
        temp_address[i] = ' ';
      temp_address_i = 0;
			temp_taken = -1;
			array_iterator++;
      end_of_file = 1;
    }
		else if(address_input == 0 && parsed_address == 0)
		{
			c = fgetc(fp);
			parsed_address = 1;
		}
		else if(address_input == 0 && parsed_address == 1)
		{
			if(c != ' ')
			{
				temp_address[temp_address_i] = (char) c;
        temp_address_i++;
			}
			else
			{
				temp_pc = strtoul(temp_address, &eptr, 16);
				address_input = 1;
			}
		}
		else if(address_input == 1 && parsed_taken == 0)
		{
			if(c == 'N')
			{
				temp_taken = 0;
				parsed_taken = 1;
			}
			else if(c == 'T')
			{
				temp_taken = 1;
				parsed_taken = 1;
			}
		}
		if(c == '\n')
		{
      printf("I made it here.\n");
			address_input = 0;
			parsed_address = 0;
			parsed_taken = 0;
			ans[array_iterator]-> pc = strtoul(temp_address, &eptr, 16);
			ans[array_iterator]->taken = temp_taken;
			for(int i = 0; i < 12; i++)
        temp_address[i] = ' ';
      temp_address_i = 0;
			temp_taken = -1;
			array_iterator++;
		}

	}
	return ans;
}
