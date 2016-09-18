#include "Pentastate.h"

Pentastate _INV(const Pentastate a)
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

Pentastate _AND(const Pentastate a, const Pentastate b)
{
  if (a == Pentastate::High || a == Pentastate::Rising)
  {
    if (b == Pentastate::High || b == Pentastate::Rising)
    {
      return Pentastate::High;
    }
  }

  return Pentastate::Low;
}

Pentastate _OR(const Pentastate a,const Pentastate b)
{
  if (a == Pentastate::High || a == Pentastate::Rising)
  {
    return Pentastate::High;
  }
  if (b == Pentastate::High || b == Pentastate::Rising)
  {
    return Pentastate::High;
  }

  return Pentastate::Low;
}

Pentastate _XOR(const Pentastate a, const Pentastate b)
{
  return _AND(_OR(a,b),_INV(_AND(a,b)));
}

Pentastate _ANDP(const Pentastate a, const Pentastate b)
{
  if (a == Pentastate::Rising)
  {
    if (b == Pentastate::Rising)
    {
      return Pentastate::Rising;
    }
  }

  return Pentastate::Low;
}

Pentastate _ORP(const Pentastate a,const Pentastate b)
{
  if (a == Pentastate::Rising)
  {
    return Pentastate::Rising;
  }
  if (b == Pentastate::Rising)
  {
    return Pentastate::Rising;
  }

  return Pentastate::Low;
}

Pentastate _XORP(const Pentastate a, const Pentastate b)
{
  return _ANDP(_ORP(a,b),_INV(_ANDP(a,b)));
}

Pentastate _ANDF(const Pentastate a, const Pentastate b)
{
  if (a == Pentastate::Falling)
  {
    if (b == Pentastate::Falling)
    {
      return Pentastate::Falling;
    }
  }

  return Pentastate::Low;
}

Pentastate _ORF(const Pentastate a,const Pentastate b)
{
  if (a == Pentastate::Falling)
  {
    return Pentastate::Falling;
  }
  if (b == Pentastate::Falling)
  {
    return Pentastate::Falling;
  }

  return Pentastate::Low;
}

Pentastate _XORF(const Pentastate a, const Pentastate b)
{
  return _ANDF(_ORF(a,b),_INV(_ANDF(a,b)));
}

Pentastate fromInt(const int i)
{
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
