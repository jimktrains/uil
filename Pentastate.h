#pragma once

/**
 * Represents the state of a binary value and the transition between states
 */
enum class Pentastate
{
  None,
  Low,
  Falling,
  Rising,
  High,
};

class PentastateLogic
{

  public:
    static Pentastate INV(const Pentastate a);

    static Pentastate AND(const Pentastate a, const Pentastate b);
    static Pentastate OR(const Pentastate a,const Pentastate b);
    static Pentastate XOR(const Pentastate a, const Pentastate b);

    static Pentastate ANDP(const Pentastate a, const Pentastate b);
    static Pentastate ORP(const Pentastate a,const Pentastate b);
    static Pentastate XORP(const Pentastate a, const Pentastate b);

    static Pentastate ANDF(const Pentastate a, const Pentastate b);
    static Pentastate ORF(const Pentastate a,const Pentastate b);
    static Pentastate XORF(const Pentastate a, const Pentastate b);


    static Pentastate fromInt(const int i);

    static const char *toString(Pentastate x);

    static bool isHigh(Pentastate a);
    static bool isLow(Pentastate a);
};
