#pragma once

#include "LadderInput.h"
#include "LadderOutput.h"

template<unsigned char STACK_DEPTH>
class LadderRung
{
  public:
    static Pentastate accum;
    LadderRung<STACK_DEPTH> LD(LadderInput& in);
    LadderRung<STACK_DEPTH> OUT(LadderOutput& out);

    LadderRung<STACK_DEPTH> AND(LadderInput& in);
    LadderRung<STACK_DEPTH> OR(LadderInput& in);

    LadderRung<STACK_DEPTH + 1> MPS();
    LadderRung<STACK_DEPTH - 1> ORB();

};

constexpr unsigned char MAX_STACK_DEPTH = 3;

template<unsigned char STACK_DEPTH>
Pentastate LadderRung<STACK_DEPTH>::accum = Pentastate::None;

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: LD(LadderInput& in)
{
  accum = in.value();
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: OUT(LadderOutput& out)
{
  out.setValue(accum);
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: AND(LadderInput& in)
{
  accum = _AND(accum, in.value());
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: OR(LadderInput& in)
{
  accum = _OR(accum, in.value());
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1> LadderRung<STACK_DEPTH> :: ORB()
{
  static_assert(STACK_DEPTH > 0, "Min Stack Depth exceeded");
  return LadderRung<STACK_DEPTH - 1>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH + 1> LadderRung<STACK_DEPTH> :: MPS()
{
  static_assert(STACK_DEPTH < MAX_STACK_DEPTH, "Max Stack Depth exceeded");
  LadderRung<STACK_DEPTH + 1>::accum = Pentastate::None;
  return LadderRung<STACK_DEPTH + 1>();
}
