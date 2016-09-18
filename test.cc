#include <iostream>
#include <random>
#include <time.h>
#include "LadderRung.h"


int main()
{
  srand(time(NULL));

  auto button1 = RandomInput();
  auto led1 = TestOutput();

  LadderRung<0>()
    .LD(button1)
    .AND(button1)
    .OUT(led1)
    .MPS()
      .LD(button1)
      .OR(button1)
      .OUT(led1)
    .ORB()
    .OUT(led1);
 ;
}
