#include <iostream>
#include <random>
#include <time.h> 

enum class Pentastate
{
  None,
  Low,
  Falling,
  Rising,
  High,
};

Pentastate _AND(Pentastate a, Pentastate b)
{
  return (a == Pentastate::High && b == Pentastate::High) ? Pentastate::High : Pentastate::Low;
}

Pentastate _OR(Pentastate a, Pentastate b)
{
  return (a == Pentastate::High || b == Pentastate::High) ? Pentastate::High : Pentastate::Low;
}

Pentastate _INV(Pentastate a)
{
  switch(a) {
    case Pentastate::None: return Pentastate::None;
    case Pentastate::Low: return Pentastate::High;
    case Pentastate::Falling: return Pentastate::Rising;
    case Pentastate::Rising: return Pentastate::Falling;
    case Pentastate::High: return Pentastate::Low;
    // Satisfies the compiler
    default: return Pentastate::None;
  }
}

Pentastate _XOR(Pentastate a, Pentastate b)
{
  return _AND(_OR(a,b),_INV(_AND(a,b)));
}

class LadderInput 
{ 
  public:
    Pentastate value();
};

class LadderOutput : public LadderInput 
{
  public:
    void setValue(Pentastate value);
};

Pentastate LadderInput :: value()
{
  int i = rand() % 4;
  switch(i)
  {
    case 0: return Pentastate::Low;
    case 1: return Pentastate::Falling;
    case 2: return Pentastate::Rising;
    case 3: return Pentastate::High;
    // Satisfy the compiler
    default: return Pentastate::None;
  }
}

const char *toString(Pentastate x)
{
  switch(x)
  {
    case Pentastate::None: return "None";
    case Pentastate::Low: return "Low";
    case Pentastate::Falling: return "Falling";
    case Pentastate::Rising: return "Rising";
    case Pentastate::High: return "High";
    // Satisfy the compiler
    default: return "error";
  }
}

void LadderOutput :: setValue(Pentastate value)
{
  std::cout << toString(value) << std::endl;
}

Pentastate accum = Pentastate::None;

template<unsigned char STACK_DEPTH>
class LadderRung
{
  public:
    LadderRung<STACK_DEPTH>* LD(LadderInput& in);
    LadderRung<STACK_DEPTH>* AND(LadderInput in);
    LadderRung<STACK_DEPTH>* OR(LadderInput in);
    LadderRung<STACK_DEPTH>* INV();

    LadderRung<STACK_DEPTH + 1>* PUSH();
    LadderRung<STACK_DEPTH - 1>* POP();
    LadderRung<STACK_DEPTH - 1>* ORB();
    LadderRung<STACK_DEPTH - 1>* ANDB();

    LadderRung<STACK_DEPTH>* SET(LadderOutput out);
    LadderRung<STACK_DEPTH>* RST(LadderOutput out);
};

Pentastate stack[] = { Pentastate::None, Pentastate::None, Pentastate::None };
constexpr auto MAX_STACK_DEPTH = sizeof(stack) / sizeof(*stack);


template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH>* LadderRung<STACK_DEPTH> :: LD(LadderInput& in)
{
  accum = in.value();
  return this;
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH>* LadderRung<STACK_DEPTH> :: SET(LadderOutput out)
{
  out.setValue(accum);
  return this;
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH>* LadderRung<STACK_DEPTH> :: RST(LadderOutput out)
{
  return SET(Pentastate::Low);
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1>* LadderRung<STACK_DEPTH> :: ORB()
{
  
  static_assert(STACK_DEPTH > 0, "Cannot ORB from an empty stack");
  accum = _OR(accum,stack[STACK_DEPTH]);
  return reinterpret_cast<LadderRung<STACK_DEPTH - 1> *>(this);
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1>* LadderRung<STACK_DEPTH> :: ANDB()
{
  
  static_assert(STACK_DEPTH > 0, "Cannot ORB from an empty stack");
  accum = _AND(accum, stack[STACK_DEPTH]);
  return reinterpret_cast<LadderRung<STACK_DEPTH - 1> *>(this);
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH>* LadderRung<STACK_DEPTH> :: AND(LadderInput in)
{
  accum = _AND(accum,in.value());
  return this;
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH>* LadderRung<STACK_DEPTH> :: INV()
{
  accum = _INV(accum);
  return this;
}
template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH>* LadderRung<STACK_DEPTH> :: OR(LadderInput in)
{
  accum = _OR(accum, in.value());
  return this;
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH + 1>*  LadderRung<STACK_DEPTH> :: PUSH()
{
  static_assert(STACK_DEPTH < MAX_STACK_DEPTH, "Stack is too large");
  stack[STACK_DEPTH] = accum;
  return reinterpret_cast<LadderRung<STACK_DEPTH + 1> *>(this);
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1>*  LadderRung<STACK_DEPTH> :: POP()
{
  static_assert(STACK_DEPTH > 0, "Cannot pop from an empty stack");
  stack[STACK_DEPTH] = accum;
  return reinterpret_cast<LadderRung<STACK_DEPTH - 1> *>(this);
}

int main()
{
  srand(time(NULL));

  LadderInput button1 = LadderInput();
  LadderOutput led1 = LadderOutput();

  LadderRung<0>()
    .LD(button1)
    ->SET(led1)
    ->PUSH()
      ->LD(button1)
      ->SET(led1)
    ->ORB()
    ->SET(led1);
}
