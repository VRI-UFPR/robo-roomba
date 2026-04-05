#include <stdio.h>
#include <unistd.h>

#include "irobot.h"

int main() {
    int res = irobot_init("/dev/ttyUSB0");
    if ( res < 0 ) {
        return -1;
    }

    irobot_start_full();
   
    // irobot_play(5);
    // sleep(1);

    
    irobot_move(10, 0);
    sleep(1);
    
    irobot_move_stop();
    
   
    irobot_close();
    return 0;
}