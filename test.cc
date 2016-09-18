#include <iostream>
#include <random>
#include <time.h>
#include "LadderRung.h"


int main()
{
  srand(time(NULL));

  LadderInput button1 = LadderInput();
  LadderOutput led1 = LadderOutput();

  LadderRung<0>()
    .LD(button1)
    .AND(button1)
    .MPS()
      .LD(button1)
      .OUT(led1)
    .ORB()
    .OUT(led1);
 ;
}
