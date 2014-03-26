#include <avr/interrupt.h>
#include <avr/io.h>


#include "control.h"

int main (void) {

        setupControls();

        // Wait untill black button is pressed.
        while (!buttonPressed());

        // Main code here

}
