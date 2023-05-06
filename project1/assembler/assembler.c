/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int opcode2Number(char *);
int boundCheck(int, int, int);
int addLabel(char *, int);
int findLabel(char *);

struct {
	char label[7];
	int index;
	int is_used;
} label_data[65536];

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	int pc;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* here is an example for how to use readAndParse to read a line from
		 inFilePtr */
	pc = -1;
	int a0, a1, a2, offset;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		pc++;

		// TODO 1 : check existence of label 
		if (!strlen(label))
		{
			continue;
		}
		// TODO 1-1 : label name check
		if (isNumber(label)) 
		{
			printf("ERROR : label is not number\n");
			goto ERROR;
		}
		
		// TODO 1-2 : label name check
		if (label[0] >= '0' && label[0] <= '9') 
		{
			printf("ERROR : label is not started number\n");
			goto ERROR;
		}

		// TODO 2 : maximum label length check
		if (strlen(label) > 6) 
		{
			printf("ERROR : Maximum label length is 6\n");
			goto ERROR;
		}

		// TODO 3 : label duplication check
		if (addLabel(label, pc))
		{
			printf("ERROR : Duplicate label!\n");
			goto ERROR;
		}
	}
	/* reached end of file */

	/* TODO: Phase-1 label calculation */

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */

	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */
	pc = -1;
	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		pc++;

		int record;

		int opcodeNum = opcode2Number(opcode);

		switch (opcodeNum)
		{
		case 0:
		case 1:
			if (!(isNumber(arg0) && isNumber(arg1) && isNumber(arg2)))
			{
				printf("ERROR : non-integer register arguments\n");
				goto ERROR;
			}
			a0 = atoi(arg0), a1 = atoi(arg1), a2 = atoi(arg2);
			if (!(boundCheck(a0, 0, 7) && boundCheck(a1, 0, 7) && boundCheck(a2, 0, 7)))
			{
				printf("ERROR : registers outside the range\n");
				goto ERROR;
			}
			record = opcodeNum << 22 | a0 << 19 | a1 << 16 | 0 << 3 | a2;
			break;
		case 2:
		case 3:
			if (!(isNumber(arg0) && isNumber(arg1)))
			{
				printf("ERROR : non-integer register arguments\n");
				goto ERROR;
			}
			if (isNumber(arg2))
			{
				offset = atoi(arg2);
				if (!boundCheck(offset, -32768, 32767))
				{
					printf("ERROR : offsetField doesn't fit in 16bits\n");
					goto ERROR;
				}
			}
			else
			{
				offset = findLabel(arg2);
				if (offset == -1) 
				{
					printf("ERROR : undefined label\n");
					goto ERROR;
				}
			}
			a0 = atoi(arg0), a1 = atoi(arg1);
			if (!(boundCheck(a0, 0, 7) && boundCheck(a1, 0, 7))) 
			{
				printf("ERROR : registers outside the range\n");
				goto ERROR;
			}
			record = opcodeNum << 22 | a0 << 19 | a1 << 16 | (offset & 0xffff);
			break;
		case 4:
			if (!(isNumber(arg0) && isNumber(arg1)))
			{
				printf("ERROR : non-integer register arguments\n");
				goto ERROR;
			}
			if (isNumber(arg2))
			{
				offset = atoi(arg2);
				if (!boundCheck(offset, -32768, 32767)) 
				{
					printf("ERROR : offsetField doesn't fit in 16bits\n");
					goto ERROR;
				}
			}
			else
			{
				offset = findLabel(arg2);
				if (offset == -1)
				{
					printf("ERROR : undefined label\n");
					goto ERROR;
				}
				offset = offset - pc - 1;
			}
			a0 = atoi(arg0), a1 = atoi(arg1);
			if (!(boundCheck(a0, 0, 7) && boundCheck(a1, 0, 7))) 
			{
				printf("ERROR : registers outside the range\n");
				goto ERROR;
			}
			record = opcodeNum << 22 | a0 << 19 | a1 << 16 | (offset & 0xffff);
			break;
		case 5:
			a0 = atoi(arg0), a1 = atoi(arg1);
			if (!(boundCheck(a0, 0, 7) && boundCheck(a1, 0, 7))) 
			{
				printf("ERROR : registers outside the range\n");
				goto ERROR;
			}
			record = opcodeNum << 22 | a0 << 19 | a1 << 16 | 0;
			break;
		case 6:
		case 7:
			record = opcodeNum << 22 | 0;
			break;
		case 8:
			if (isNumber(arg0)) 
			{
				record = atoi(arg0);
			}
			else
			{
				// find label
				// if label doesn't found, exit(1)
				int label_idx = findLabel(arg0);
				if (label_idx == -1)
				{
					printf("ERROR : label not found\n");
					goto ERROR;
				}
				else
				{
					record = label_idx;
				}
			}
			break;
		default:
			printf("ERROR : Unrecognized opcodes\n");
			goto ERROR;
		}
		
		fprintf(outFilePtr, "%d\n", record);
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);

ERROR:
	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	exit(1);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int opcode2Number(char *opcode) 
{
	if (!strcmp(opcode, "add")) 
	{
		return(0);
	}
	else if (!strcmp(opcode, "nor")) 
	{
		return(1);
	}
	else if (!strcmp(opcode, "lw"))
	{
		return(2);
	}
	else if (!strcmp(opcode, "sw")) 
	{
		return(3);
	}
	else if (!strcmp(opcode, "beq"))
	{
		return(4);
	}
	else if (!strcmp(opcode, "jalr"))
	{
		return(5);
	}
	else if (!strcmp(opcode, "halt"))
	{
		return(6); 
	}
	else if (!strcmp(opcode, "noop"))
	{
		return(7);
	}
	else if (!strcmp(opcode, ".fill"))
	{
		return(8);
	}
	return(-1);
}

int boundCheck(int arg, int lower, int upper) 
{
	if (lower <= arg && arg <= upper)
	{
		return(1);
	}
	return(0);
}

int addLabel(char *label, int pc) 
{
	for(int i = 0; i < 65536; i++)
	{
		if (label_data[i].is_used && !strcmp(label, label_data[i].label)) 
		{
			return 1;
		}
		if (!label_data[i].is_used)
		{
			strcpy(label_data[i].label, label);
			label_data[i].is_used = 1;
			label_data[i].index = pc;
			break;
		}
	}
	return 0;
}

int findLabel(char *label)
{
	for(int i = 0; i < 65536; i++)
	{
		if (label_data[i].is_used == 0) break;
		if (!strcmp(label, label_data[i].label))
		{
			return label_data[i].index;
		}
	}
	return -1;
}
