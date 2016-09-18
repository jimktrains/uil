#pragma once
#include <iostream>

enum class Pentastate
{
  None,
  Low,
  Falling,
  Rising,
  High,
};

Pentastate _INV(const Pentastate a);

Pentastate _AND(const Pentastate a, const Pentastate b);
Pentastate _OR(const Pentastate a,const Pentastate b);
Pentastate _XOR(const Pentastate a, const Pentastate b);

Pentastate _ANDP(const Pentastate a, const Pentastate b);
Pentastate _ORP(const Pentastate a,const Pentastate b);
Pentastate _XORP(const Pentastate a, const Pentastate b);

Pentastate _ANDF(const Pentastate a, const Pentastate b);
Pentastate _ORF(const Pentastate a,const Pentastate b);
Pentastate _XORF(const Pentastate a, const Pentastate b);


Pentastate fromInt(const int i);

const char *toString(Pentastate x);
