// Letters and numbers (and anything else) are represented as unsigned 32bit numbers:
// 5x5 dots bitwise encoded by columns, then rows.
// i.E. the letter "M" is 
// 0 1 0 1 0     1        2        4        8        16
// 1 0 1 0 1     32       64       128      256      512
// 1 0 1 0 1     1024     2048     4096     8192     16384
// 1 0 1 0 1     32768    65536    131072   262114   524288
// 1 0 1 0 1     1048576  2097152  4194304  8388608  16777216
// This results in the uint32_t number "22730410"

long ArrCharacters[] = {
2266274, 2164834, 15798566, 3278979, 4340901, 3279911, 6593574, 2167047, 6592806, 6568230,
//  0       1        2         3       4        5        6        7        8        9

  9938214, 7642407, 6587686, 7644455, 7375911, 1084451, 6599718, 9747753, 7407687, 2265230, 9604265, 7373857, 22730410,
// 10-A      11-B    12-C      13-D     14-E    15-F     16-G     17-H     18-I     19-J     20-K     21-L      22-M

  18667121, 6595878, 1088807, 6726950, 9739559, 7608366, 4329631, 6595881, 2266277, 18732725, 5408933, 2164901, 15767823, 2099331, 1049633, 5248165, 4540074, 32800};
//  23-N     24-O     25-P     26-Q     27-R     28-S     29-T      30-U     31-V      32-W     33-X     34-Y      35-Z      36-?     37-!    38-!!  39-Heart  40-:


long w = 1;
long z = 5;

void drawChar(int _char, CRGB _color, uint8_t _hPos, uint8_t _vPos) {
  if(_hPos >= 21 || _vPos >= 10) return;  // entirety of character outside visible area, do nothing
  if(_char <= (sizeof(ArrCharacters))) {
    long theChar = ArrCharacters[_char];
    for(long i=0;i<25;i++) {
      if((w<<i)&theChar) {
        leds[XY((_hPos-z+(i%z)),_vPos-z+(i/z))] = _color;
      }
    }
  }
}
