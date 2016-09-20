#include <iostream>
#include <random>
#include <time.h>
#include "LadderRung.h"

char pairToSignal(Pentastate clear, Pentastate approach)
{
  char S0w = ' ';
  if (PentastateLogic::isHigh(approach))
  {
    S0w = 'A';
  }
  else if (PentastateLogic::isHigh(clear))
  {
    S0w = 'C';
  }
  else
  {
    S0w = 'S';
  }
  return S0w;
}

int main()
{
  srand(time(NULL));

  auto T0w = FixedInput(Pentastate::Low);//SemiRandomInput();
  auto T0e = FixedInput(Pentastate::Low);//_INV(T0w.value()));
  auto T0 = TestOutput();

  auto Tm1e = SemiRandomInput();
  auto Tm1w = FixedInput(PentastateLogic::INV(Tm1e.value()));
  auto Tm1 = TestOutput();

  auto T1e = SemiRandomInput();//FixedInput(Pentastate::Low);//SemiRandomInput();
  auto T1w = FixedInput(PentastateLogic::INV(T1e.value()));//FixedInput(Pentastate::Low);//_INV(T1e.value()));
  auto T1 = TestOutput();

  auto S0wc = TestOutput();
  auto S0wa = TestOutput();

  auto S0ec = TestOutput();
  auto S0ea = TestOutput();

  /*
   *                    S0e
   * |   T-1   |     T0    |   T1      |
   *            S0w
   */

  LadderRung<0>()
    // Sets occupancy if there is a train going in either direction
    .LD(T0e)
    .OR(T0w)
    .OUT(T0)

    // Sets occupancy if there is a train going in either direction
    .LD(T1e)
    .OR(T1w)
    .OUT(T1)

    // Sets occupancy if there is a train going in either direction
    .LD(Tm1e)
    .OR(Tm1w)
    .OUT(Tm1)

    // Clear iff no train for the next two blocks AND appoaching, not
    // leaving the signal
    .LDI(T0)
    .ANDI(T1)
    .ANDI(Tm1w)
    .OUT(S0wc)

    // Clear iff no train for the next one block AND appoaching, not
    // leaving the signal AND train in two blocks is headed in the same
    // direction
    .LDI(T0)
    .AND(T1e)
    .ANDI(Tm1w)
    .OUT(S0wa)

    // Clear iff no train for the next two blocks AND appoaching, not
    // leaving the signal
    .LDI(T0)
    .ANDI(Tm1)
    .ANDI(T1e)
    .OUT(S0ec)

    // If I don't have a train
    .LDI(T0)
    // and T-1 has a westbound
    .AND(Tm1w)
    // and T1 doesn't have an east bound
    .ANDI(T1e)
    // east signal can be apprach
    .OUT(S0ea)
  ;


  char S0e = pairToSignal(S0ec.value(), S0ea.value());
  char S0w = pairToSignal(S0wc.value(), S0wa.value());

  std::cout << "             " << S0e << std::endl;
  std::cout << "|  " << (PentastateLogic::isHigh(Tm1e.value()) ? "->" : PentastateLogic::isHigh(Tm1w.value()) ? "<-" : "  ")
          << "  |  " << (PentastateLogic::isHigh(T0e.value()) ? "->" :  PentastateLogic::isHigh(T0w.value()) ? "<-" : "  ")
          << "  |  " << (PentastateLogic::isHigh(T1e.value()) ? "->" :  PentastateLogic::isHigh(T1w.value()) ? "<-" : "  ")
          << "  |" << std::endl;
  std::cout << "        " << S0w << std::endl;
}

