/* Target MCU: PIC12F508 */

#include <xc.h>

void main(void) {
    TRISGPIO = 0x00;
    while (1) {
        GPIO = 0xff;
        _delay(1000);
        GPIO = 0x00;
        _delay(1000);
    }
    return;
}
