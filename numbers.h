// Numbers are represented as unsigned 16bit numbers:
// 3x5 dots bitwise encoded by columns, then rows.
// For example, the number "3" is 
// 1 1 0    '1'    '2'     4
// 0 0 1     8      16    '32'
// 0 1 0     64    '128'   256
// 0 0 1     512    1024  '2048'
// 1 1 0    '4096' '8192'  16384
// This results in the uint16_t number "14499"        (1+2+32+128+2048+4096+8192=14499)

uint16_t ArrNumbers[] = {
 11114 ,9370, 29994, 14499, 18861, 14543, 10958, 9511, 10922, 14762};
// 0     1      2      3      4       5      6     7      8      9

void drawNum(int _num, CRGB _color, uint8_t _hPos, uint8_t _vPos) {
  if(_hPos >= 21 || _vPos >= 10) return;  // start position outside visible area, do nothing
  if(_num < 10) {
    uint16_t theNum = ArrNumbers[_num];
    for(uint8_t i=0;i<16;i++) {
      if((1<<i)&theNum) {
       leds[XY(_hPos-5+(i%3),_vPos-5+(i/3))] = _color;
      }
    }
  }
}
