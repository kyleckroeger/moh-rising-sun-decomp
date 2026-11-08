#ifndef COLOR_H
#define COLOR_H
// Four-byte colour passed by value; channel names are descriptive.
class CColor {
  public:
    unsigned char r, g, b, a;
    CColor() {}
    CColor(unsigned char ar, unsigned char ag, unsigned char ab, unsigned char aa) : r(ar), g(ag), b(ab), a(aa) {}
};
#endif
