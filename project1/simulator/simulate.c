/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int opcode, arg0, arg1, arg2, offset, numOfInstructions;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    for (int i = 0; i < NUMREGS; i++)
    {
        state.reg[i] = 0;
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

		/* TODO: */
    state.pc = 0;
    numOfInstructions = 0;
    while(1)
    {
        numOfInstructions++;

        printState(&state);
        int line = state.mem[state.pc];

        opcode = (line >> 22) & 7;
        arg0 = (line >> 19) & 7;
        arg1 = (line >> 16) & 7;
        switch (opcode)
        {
        case 0:
            arg2 = line & 7;

            state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
            break;
        case 1:
            arg2 = line & 7;

            state.reg[arg2] = ~ (state.reg[arg0] | state.reg[arg1]);
            break;
        case 2:
            offset = convertNum(line & 0xffff);

            state.reg[arg1] = state.mem[state.reg[arg0] + offset];
            break;
        case 3:
            offset = convertNum(line & 0xffff);

            state.mem[state.reg[arg0] + offset] = state.reg[arg1];
            break;
        case 4:
            offset = convertNum(line & 0xffff);
            
            if (state.reg[arg0] == state.reg[arg1]) 
            {
                state.pc = offset + state.pc;
            }
            break;
        case 5:
            state.reg[arg1] = state.pc + 1;
            state.pc = state.reg[arg0] - 1;
            break;
        case 6:
            state.pc++;
            printf("machine halted\n");
            printf("total of %d instructions executed\n", numOfInstructions);
            printf("final state of machine:\n");

            printState(&state);

            exit(0);
        case 7:
            break;
        default:
            exit(1);
        }

        state.pc++;
    }

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
