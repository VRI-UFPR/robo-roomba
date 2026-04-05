// Written by: Erick Cobos T
// Date: 10-Nov-2015

#include "iRobot.h"
#include <unistd.h>

//An example of how to use the iRobot class in Unix
int main(){
    iRobot myiRobot("/dev/ttyUSB0"); // You can use 'dmesg | grep tty' to obtain the port.

    myiRobot.moveAhead(5);
    sleep(2);
    


    // for (int i=0; i<10; i++) {
        // myiRobot.turnLeft();

    // Move around
    /* myiRobot.moveAhead(40);
    myiRobot.turnLeft();
    myiRobot.moveAhead(40);
    myiRobot.turnRight();
    myiRobot.moveAhead(40);
    myiRobot.moveAhead(40);
    myiRobot.moveAhead(40);
    myiRobot.turnRight();
    myiRobot.moveAhead(40);
    myiRobot.turnLeft();
    myiRobot.moveAhead(40);
    myiRobot.turnRight();
    myiRobot.moveAhead(40);*/
}
