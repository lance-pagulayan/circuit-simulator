#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Variables{
  char varName[25];
  int* bitval;
}Variables;

int inputVars;
int outputVars;
int iTmp = 0;
Variables* vars;
Variables* vars2;
Variables* tempVars;

Variables* findVariable(char*);
void freeMyVars(Variables*, int);

int main(int argc, char* argv[]){
  FILE* fp = fopen(argv[1], "r");
  char instr[20];

  fscanf(fp, "%s %d", instr, &inputVars);
  int row = pow(2, inputVars);
  vars = malloc(sizeof(Variables) * inputVars);
  for(int i = 0; i < inputVars; i++){
    fscanf(fp, "%s", vars[i].varName);
    vars[i].bitval = malloc(sizeof(int) * row);
  }
  fscanf(fp, "%s %d", instr, &outputVars);
  vars2 = malloc(sizeof(Variables) * outputVars);
  for(int i = 0; i < outputVars; i++){
    fscanf(fp, "%s", vars2[i].varName);
    vars2[i].bitval = malloc(sizeof(int) * row);
  }
  tempVars = malloc(sizeof(Variables) * 50);
  for(int i = 0; i < 50; i++){
    tempVars[i].bitval = NULL; //malloc(sizeof(int) * row);
  }

  for(int i = 0; i < inputVars; i++){
    for(int j = 0; j < row; j++){
      int nBits = inputVars - (i + 1);
      vars[i].bitval[j] = (j >> nBits) & 1;
    }
  }

  char inp1[20];
  char inp2[20];
  char temp[20];

  while(fscanf(fp, "%s", instr) == 1){
    if(strcmp(instr, "NOT") != 0){
      fscanf(fp, "%s %s %s", inp1, inp2, temp);
      Variables* arg1 = findVariable(inp1);
      Variables* arg2 = findVariable(inp2);
      Variables* arg3 = findVariable(temp);
      if(strcmp(instr, "AND") == 0){
        for(int i = 0; i < row; i++){
	  arg3->bitval[i] = arg1->bitval[i] && arg2->bitval[i];
        }
        iTmp++;
      }
      if(strcmp(instr, "OR") == 0){
        for(int i = 0; i < row; i++){
	  arg3->bitval[i] = arg1->bitval[i] || arg2->bitval[i];
        }
        iTmp++;
      }
      if(strcmp(instr, "NAND") == 0){
        for(int i = 0; i < row; i++){
	  arg3->bitval[i] = !(arg1->bitval[i] && arg2->bitval[i]);
        }
        iTmp++;
      }
      if(strcmp(instr, "NOR") == 0){
        for(int i = 0; i < row; i++){
	  arg3->bitval[i] = !(arg1->bitval[i] || arg2->bitval[i]);
        }
        iTmp++;
      }
      if(strcmp(instr, "XOR") == 0){
        for(int i = 0; i < row; i++){
	  arg3->bitval[i] = ( (arg1->bitval[i] || arg2->bitval[i]) && !(arg1->bitval[i] && arg2->bitval[i]) );
        }
	iTmp++;
      }
    }else{
      fscanf(fp, "%s %s", inp1, inp2);
      Variables* arg1 = findVariable(inp1);
      Variables* arg2 = findVariable(inp2);
      for(int i = 0; i < row; i++){
	arg2->bitval[i] = !(arg1->bitval[i]);
      }
      iTmp++;
    }
  }

  for(int i = 0; i < row; i++){
    for(int j = 0; j < inputVars; j++){
      printf("%d ", vars[j].bitval[i]);
    }
    for(int w = 0; w < outputVars; w++){
      printf("%d ", vars2[w].bitval[i]);
    }
    printf("\n");
  }

  freeMyVars(vars, inputVars);
  freeMyVars(vars2, outputVars);
  freeMyVars(tempVars, 50);

  return 0;
}

Variables* findVariable(char* name){
  for(int i = 0; i < inputVars; i++){
    if(strcmp(vars[i].varName, name) == 0){
      return &vars[i];
    }
  }
  for(int i = 0; i < 50; i++){
    if(strcmp(tempVars[i].varName, name) == 0){
      return &tempVars[i];
    }
  }
  for(int i = 0; i < outputVars; i++){
    if(strcmp(vars2[i].varName, name) == 0){
      return &vars2[i];
    }
  }
  //when we encounter a new temp variable
  strcpy(tempVars[iTmp].varName, name);
  tempVars[iTmp].bitval = malloc(sizeof(int) * (pow(2, inputVars)));
  return &tempVars[iTmp];
}

void freeMyVars(Variables* varName, int x){
  for(int i = 0; i < x; i++){
    if(varName[i].bitval != NULL){
      free(varName[i].bitval);
    }
  }
  free(varName);
}
