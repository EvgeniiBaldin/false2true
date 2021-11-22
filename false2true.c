/* false mutated to true_ 
   in approximately 100 generations 
   mutation: 1 random letter in one random position
   10 mutated children and 1 parent
   selected the best between children and parent

   overall \sim 1000 mutations

   27 = 26 letters + _ symbol
   5 positions
   27^5=14,348,907
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NLETTER 6
#define NCHILD 10

int printSymbol(void) {
  char c;
  for (c=0x61;c<=0x7a;c++) {
    printf("%d %c\n",c,c);
  }
  c=0x5F;
  printf("%d %c\n",c,c);
  return 1;
}

char randomSymbol(void) {
  int r=rand()%27;
  if (r==26) return 0x5F; //symbol _
  else return r+0x61; //letter
}

int mutate(char *word,int size) {
  int n=rand()%size;//select char
  char newl=randomSymbol();
  word[n]=newl;
  return n;
}

int scorecalc(char *word,char *result,int size) {
  int score=0;
  for (int i=0;i<size;i++) {
    score=score+(word[i]-result[i])*(word[i]-result[i]);
  }
  return score;
}


int main() {
  srand(4); // random seed
  char word[NLETTER]="false\0";
  char result[NLETTER]="true_\0";
  char childs[NLETTER*NCHILD];
  int score[NCHILD];
  int ngen=0;
  int wscore=scorecalc(word,result,NLETTER-1);//calc parent score

  while (wscore!=0) {
    ngen++;
    wscore=scorecalc(word,result,NLETTER-1);//calc parent score
    //mutation
    for(int j=0;j<NCHILD;j++) {
      char *pchild=childs+NLETTER*j;
      //make a copy of parent
      strncpy(pchild,word,NLETTER);
      //mutate random position and random letter
      mutate(pchild,NLETTER-1);
      //calculate score
      score[j]=scorecalc(pchild,result,NLETTER-1);
    }
    
    //selection
    //select the best score (lower the better)
    int jbest=0;
    for(int j=1;j<NCHILD;j++) 
      if (score[j]<score[j-1]) 
	jbest=j;
    char *pbest=childs+NLETTER*jbest;

    //print generation
    printf("Generation %d parent %s score %d \n",ngen,word,wscore);
    for(int j=0;j<NCHILD;j++) {
      char *pchild=childs+NLETTER*j;
      printf(" child N%d %s score %d\n",j,pchild,score[j]);
    }
    
    // selecting next parent
    if (wscore>score[jbest])
      strncpy(word,pbest,NLETTER);


    // test for 0 (best score)
    if (score[jbest]==0) {
      printf("Hooray! false became true_\n");
      printf("%s\n",word);
    }
  }
}
