#pragma once

#include "LadderInput.h"
#include "LadderOutput.h"

/**
 * Represents a rung in the ladder logic.
 *
 * This class is quite odd.  It's templated by the stack depth,
 * but that value is never used outside a template.  Moreover,
 * the class only has a static data member, and all member functions
 * have `__attribute__((always_inline))`.
 *
 * The template is used to specify the stack depth. It is check at compile
 * time whenever the stack depth is changed to ensure it doesn't over or
 * under flow.
 *
 * The `always_inline` is present to force the compiler to _not_ generate
 * copies of the class for each for each level in the stack.  Additionally,
 * this shouldn't be too bad since each function only has a single
 * statement and returns a new `LadderRung` at the correct stack depth. The
 * new object is shuffled away in the inlining and becomes useful only to
 * the type checker, which results in less code being generated and run upon
 * each function call.
 *
 * Example:
 *
 *    LadderRung<0>
 *      .LD(input)
 *      .OR(input2)
 *      .MPS()
 *        .LD(input3)
 *        .AND(input4)
 *      .ORB()
 *      .OUT(output);
 */
template<unsigned char STACK_DEPTH>
class LadderRung
{
  public:
    static Pentastate accum;
    static constexpr unsigned char MAX_STACK_DEPTH = 3;
    /**
     * Outputs the accumulator
     */
    LadderRung<STACK_DEPTH> OUT(LadderOutput& out);

    /**
     * Loads a value into the accumulator
     */
    LadderRung<STACK_DEPTH> LD(LadderInput& in);
    /**
     * AND the accumulator and input, placing the result in the accumulator
     */
    LadderRung<STACK_DEPTH> AND(LadderInput& in);
    /**
     * OR the accumulator and input, placing the result in the accumulator
     */
    LadderRung<STACK_DEPTH> OR(LadderInput& in);

    /**
     * Loads INV(value) into the accumulator
     */
    LadderRung<STACK_DEPTH> LDI(LadderInput& in);
    /**
     * AND the accumulator and INV(input), placing the result in the accumulator
     */
    LadderRung<STACK_DEPTH> ANDI(LadderInput& in);
    /**
     * OR the accumulator and INV(input), placing the result in the accumulator
     */
    LadderRung<STACK_DEPTH> ORI(LadderInput& in);

    /**
     * Inverts the value in the accumulator, storing the results in the
     * accumulator
     */
    LadderRung<STACK_DEPTH> INV();

    /**
     * Pushes the accumulator onto the stack, rests the accumulator, and
     * increments the stack.
     */
    LadderRung<STACK_DEPTH + 1> MPS();
    /**
     * ORs the value on the top of the stack and the accumulator, placing the
     * result in the accumulator and decrementing the stack.
     */
    LadderRung<STACK_DEPTH - 1> ORB();
    /**
     * ANDs the value on the top of the stack and the accumulator, placing the
     * result in the accumulator and decrementing the stack.
     */
    LadderRung<STACK_DEPTH - 1> ANDB();
};


template<unsigned char STACK_DEPTH>
Pentastate LadderRung<STACK_DEPTH>::accum = Pentastate::None;

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: OUT(LadderOutput& out)
{
  out.setValue(accum);
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: LD(LadderInput& in)
{
  accum = in.value();
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
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: LDI(LadderInput& in)
{
  accum = _INV(in.value());
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: ANDI(LadderInput& in)
{
  accum = _AND(accum, _INV(in.value()));
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: ORI(LadderInput& in)
{
  accum = _OR(accum, _INV(in.value()));
  return LadderRung<STACK_DEPTH>();
}


template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH> LadderRung<STACK_DEPTH> :: INV()
{
  accum = _INV(accum);
  return LadderRung<STACK_DEPTH>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH + 1> LadderRung<STACK_DEPTH> :: MPS()
{
  static_assert(STACK_DEPTH < MAX_STACK_DEPTH, "Max Stack Depth exceeded");
  LadderRung<STACK_DEPTH + 1>::accum = Pentastate::None;
  return LadderRung<STACK_DEPTH + 1>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1> LadderRung<STACK_DEPTH> :: ORB()
{
  static_assert(STACK_DEPTH > 0, "Min Stack Depth exceeded");
  LadderRung<STACK_DEPTH - 1>::accum = _OR(accum, LadderRung<STACK_DEPTH - 1>::accum);
  return LadderRung<STACK_DEPTH - 1>();
}

template<unsigned char STACK_DEPTH>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1> LadderRung<STACK_DEPTH> :: ANDB()
{
  static_assert(STACK_DEPTH > 0, "Min Stack Depth exceeded");
  LadderRung<STACK_DEPTH - 1>::accum = _AND(accum, LadderRung<STACK_DEPTH - 1>::accum);
  return LadderRung<STACK_DEPTH - 1>();
}

