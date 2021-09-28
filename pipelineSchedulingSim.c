#include <stdio.h>
#include <stdlib.h>
struct pipeline {
	char instruction[8];//whole instruction string
	int destIndex;//destination register
	int regInd1;//register of operand 1
	int regInd2;//register of operand 2
	int depDelay;//clock cycle the instruction fetch is performed + 1
};
int instructCount;//number of instructions
struct pipeline *a;

void printChart() {//prints the pipeline chart 
	int i = 0, j = 0;
	while (i<instructCount) {
		j = 0;
		printf("Instruction %d Fetched at Cycle %d\n%d)", (i + 1), (a[i].depDelay + 1), (i + 1));
		while (j < a[i].depDelay) {//prints the proper amount of tabs
			printf("\t");
			j++;
		}
		printf("IF\tID\tEX\tMM\tWB\n");
		i++;
	}
	return;
}

void enter() {//user input is stored into its corresponding variables
	int i = 0;
	/* Prompt for total number of instructions */
	printf("Enter number of instructions:\n");
	scanf("%d", &instructCount);
	while (i < instructCount) {//reads each instruction and stores the register numbers in their proper variables
		printf("%d)\n", i + 1);
		scanf("%s", a[i].instruction); //scans in the instruction as a string
		a[i].destIndex = a[i].instruction[1] - '0'; //saves the destination register number
		a[i].regInd1 = a[i].instruction[4] - '0';//saves the first operand register number
		a[i].regInd2 = a[i].instruction[7] - '0';//saves the second operand register number
		a[i].depDelay = i;//sets the clock cycle the instruction fetch is performed, delays will be added if a dependency is found
		i++;
	}
	return;
}

void delayCalc() {//calculates and adds stalls to instructions if a dependency is found
	int i = 0, j = 0;
	/* For each instruction i from 2 to total number of instructions, initialize delay as 0 and check for dependency
	between instruction (i-2) and i, as well as between instruction (i-1) and i */
	while (i < instructCount) { //loops through each instruction, checking for dependencies and adding stalls when needed
		if (i > 0) { //checks to make sure this instruction is not the first
			if (a[i - 1].destIndex == a[i].regInd1 || a[i - 1].destIndex == a[i].regInd2) { //checks for a dependency in the instruction before the current one
				j = i;
				while (j < instructCount) {
					a[j].depDelay += 2; //adds a delay of two cycles to every instruction if there is a dependency
					j++;
				}
			}
		}
		if (i > 1) { //checks to make sure that this instruction is not the first or second
			if (a[i - 2].destIndex == a[i].regInd1 || a[i - 2].destIndex == a[i].regInd2) { //checks for a dependency two instructions ago
				if ((a[i].depDelay - a[i - 2].depDelay) < 3) { //checks to make sure that the instruction hasn't already been delayed by a previous instruction's dependency
					j = i;
					while (j < instructCount) {
						a[j].depDelay += 1; //adds a delay of one cycle to every instruction if there is a dependency
						j++;
					}
				}
			}
		}
		i++;
	}
	printChart();//takes the data that has been collected and calculated and prints it out
	return;
}

int main() {//menu
	/* Declare local variables */
	int input = 0;
	a = (struct pipeline *)malloc(sizeof(struct pipeline));
	/* Until user quits, print menu, prompt for user input, and select corresponding function */
	while (input < 3) {
		printf("1) Enter instructions\n2) Determine when instructions are fetched\n3) Exit\nEnter Selection:\n");
		scanf("%d", &input);
		switch (input) {
		case 1:
			enter();
			break;
		case 2:
			delayCalc();
			break;
		case 3:
			break;
		}
	}
	//free(a);
	return 1;
}
