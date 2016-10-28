#pragma once

#include "LadderInput.h"
#include "LadderOutput.h"

template< typename... CONDITIONS >
struct all_false;

template<>
struct all_false<>
{
    const static bool value = true;
};

template< typename CONDITION, typename... CONDITIONS >
struct all_false< CONDITION, CONDITIONS... >
{
    const static bool value = !CONDITION::value && all_false<CONDITIONS...>::value;
};

template<typename T, typename... ALREADY_SET>
using NoDuplicates =
      typename std::enable_if<
          all_false< std::is_same<ALREADY_SET, T>... >::value
          >::type;

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
 * The ALREADY_SET set type parameter is to enforce setting an output only
 * once.  If you need to set an output more than once a cycle, figure out
 * how to combine the two sets (OR, AND)?
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
 *
 * Non working example:
 *
 *    LadderRung<0>
 *      .LD(input)
 *      .OR(input2)
 *      .MPS()
 *        .LD(input3)
 *        .AND(input4)
 *      .ORB()
 *      .OUT(output)
 *      .OUT(output);
 */
template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
class LadderRung
{
  Pentastate (&stack)[MAX_STACK_DEPTH];

  public:
    /**
     * Outputs the stack[STACK_DEPTH]
     */
    template<typename OUT_TYPE>
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET..., std::decay<OUT_TYPE>> OUT(OUT_TYPE& out);

    /**
     * Loads a value into the stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LD(LadderInput& in);
    /**
     * AND the stack[STACK_DEPTH] and input, placing the result in the stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> AND(LadderInput& in);
    /**
     * OR the stack[STACK_DEPTH] and input, placing the result in the stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> OR(LadderInput& in);

    /**
     * Loads INV(value) into the stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LDI(LadderInput& in);
    /**
     * AND the stack[STACK_DEPTH] and INV(input), placing the result in the stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> ANDI(LadderInput& in);
    /**
     * OR the stack[STACK_DEPTH] and INV(input), placing the result in the stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> ORI(LadderInput& in);

    /**
     * Inverts the value in the stack[STACK_DEPTH], storing the results in the
     * stack[STACK_DEPTH]
     */
    LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> INV();

    /**
     * Pushes the stack[STACK_DEPTH] onto the stack, rests the stack[STACK_DEPTH], and
     * increments the stack.
     */
    LadderRung<STACK_DEPTH + 1, MAX_STACK_DEPTH, ALREADY_SET...> MPS();
    /**
     * ORs the value on the top of the stack and the stack[STACK_DEPTH], placing the
     * result in the stack[STACK_DEPTH] and decrementing the stack.
     */
    LadderRung<STACK_DEPTH - 1, MAX_STACK_DEPTH, ALREADY_SET...> ORB();
    /**
     * ANDs the value on the top of the stack and the stack[STACK_DEPTH], placing the
     * result in the stack[STACK_DEPTH] and decrementing the stack.
     */
    LadderRung<STACK_DEPTH - 1, MAX_STACK_DEPTH, ALREADY_SET...> ANDB();

    LadderRung(Pentastate (&base_stack)[MAX_STACK_DEPTH]);
};

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: LadderRung(Pentastate (&base_stack)[MAX_STACK_DEPTH]) :
   stack(base_stack)
{
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
template<typename OUT_TYPE>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET..., std::decay<OUT_TYPE>> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: OUT(OUT_TYPE& out)
{
  static_assert( all_false< std::is_same<ALREADY_SET, std::decay<OUT_TYPE>>... >::value, "Already outputed");
  out.setValue(stack[STACK_DEPTH]);
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET..., std::decay<OUT_TYPE>>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: LD(LadderInput& in)
{
  stack[STACK_DEPTH] = in.value();
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: AND(LadderInput& in)
{
  stack[STACK_DEPTH] = PentastateLogic::AND(stack[STACK_DEPTH], in.value());
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: OR(LadderInput& in)
{
  stack[STACK_DEPTH] = PentastateLogic::OR(stack[STACK_DEPTH], in.value());
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: LDI(LadderInput& in)
{
  stack[STACK_DEPTH] = PentastateLogic::INV(in.value());
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: ANDI(LadderInput& in)
{
  stack[STACK_DEPTH] = PentastateLogic::AND(stack[STACK_DEPTH], PentastateLogic::INV(in.value()));
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: ORI(LadderInput& in)
{
  stack[STACK_DEPTH] = PentastateLogic::OR(stack[STACK_DEPTH], PentastateLogic::INV(in.value()));
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}


template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: INV()
{
  stack[STACK_DEPTH] = PentastateLogic::INV(stack[STACK_DEPTH]);
  return LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH + 1, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: MPS()
{
  static_assert( (STACK_DEPTH + 1) < MAX_STACK_DEPTH, "Max Stack Depth exceeded");
  stack[STACK_DEPTH + 1] = Pentastate::None;
  return LadderRung<STACK_DEPTH + 1, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: ORB()
{
  static_assert(STACK_DEPTH > 0, "Min Stack Depth exceeded");
  stack[STACK_DEPTH - 1] = PentastateLogic::OR(stack[STACK_DEPTH], stack[STACK_DEPTH - 1]);
  return LadderRung<STACK_DEPTH - 1, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

template<unsigned char STACK_DEPTH, unsigned char MAX_STACK_DEPTH, typename ...ALREADY_SET>
inline __attribute__((always_inline))
LadderRung<STACK_DEPTH - 1, MAX_STACK_DEPTH, ALREADY_SET...> LadderRung<STACK_DEPTH, MAX_STACK_DEPTH, ALREADY_SET...> :: ANDB()
{
  static_assert(STACK_DEPTH > 0, "Min Stack Depth exceeded");
  stack[STACK_DEPTH - 1] = PentastateLogic::AND(stack[STACK_DEPTH], stack[STACK_DEPTH - 1]);
  return LadderRung<STACK_DEPTH - 1, MAX_STACK_DEPTH, ALREADY_SET...>(stack);
}

