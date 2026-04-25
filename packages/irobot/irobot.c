#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

#include "irobot.h"


#define IROBOT_CMD_START       128
#define IROBOT_CMD_MODE_SAFE   131
#define IROBOT_CMD_MODE_FULL   132


#define IROBOT_CMD_PRESS_SPOT   137
#define IROBOT_CMD_PRESS_CLEAN  138

#define IROBOT_CMD_SET_SPEED   137



#define IROBOT_CMD_PLAY_MUSIC  141


int g_irobot_fd = -1;

int irobot_init(const char* path_ttyusb) {
    // Opens a communication with the iRobot in serial port 'port'.
    // Starts the robot in Safe Mode

    // Open the serial port
    g_irobot_fd = open(path_ttyusb, O_RDWR | O_NOCTTY | O_NDELAY );
    if (g_irobot_fd == -1)  {
        printf("error1\n");
        /*std::cerr << "Could not open serial port " << path_ttyusb << ": " << strerror(errno) << std::endl;
        if(errno == EACCES) {
             std::cerr << "Run 'sudo chmod a+rw "<< path_ttyusb << "' in a terminal to gain access to the port.";
        }*/
        return -1;
    }

    // Get control settings
    struct termios settings;
    int result = tcgetattr(g_irobot_fd, &settings);
    if (result == -1) {
        printf("error2\n");
        // std::cerr << "Could not get settings from open port: " << strerror(errno) << std::endl;
        // std::cout << "The execution failed. Press ENTER to continue.";
        return -1;
    }

    // Set iRobot serial settings
    settings.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    settings.c_iflag = IGNPAR;
    settings.c_oflag = 0;
    settings.c_lflag = 0;
    settings.c_cc[VMIN]  = 0;
    settings.c_cc[VTIME] = 0;

    result = tcsetattr(g_irobot_fd, TCSANOW, &settings);
    if(result == -1) {
        printf("error3\n");
        // std::cerr << "Could not set new settings: " << strerror(errno) << std::endl;
        // std::cout << "The execution failed. Press ENTER to continue.";
        return -1;
    }

    // Start the robot in Safe Mode (128 131)
    // irobot_start();
    // usleep(200000);
    return 0;
}


void irobot_close() {
    printf("close\n");
    // irobot_send_cmd(133);
    irobot_send_cmd(173);

    
   
    // irobot_stop();
    close(g_irobot_fd);
    g_irobot_fd = -1;
}

int irobot_write(const uint8_t* buffer, uint8_t nbytes ) {
    const int bytes = write(g_irobot_fd, (const void*) buffer, (size_t) nbytes );
    if ( bytes != nbytes ) {
        printf("Error\n");
    } 
}

int irobot_send_cmd(const uint8_t cmd) {
    irobot_write(&cmd, 1);
}

void irobot_sleep() {
    
}

int irobot_start_safe() {
    printf("start safe\n");
    irobot_send_cmd(IROBOT_CMD_START);
    irobot_send_cmd(IROBOT_CMD_MODE_SAFE);
    sleep(2);
    printf("started\n");
    return 0;
}

int irobot_start_full() {
    printf("start full\n");
    irobot_send_cmd(IROBOT_CMD_START);
    irobot_send_cmd(IROBOT_CMD_MODE_FULL);
    sleep(1);
    printf("started\n");
    return 0;
}

int irobot_play(uint8_t music_index) {
    // irobot_send_cmd(7); reset

    printf("play music\n");
    const uint8_t buffer[2] = {IROBOT_CMD_PLAY_MUSIC, music_index};
    irobot_write(buffer, 2);
    return 0;
}

void irobot_move(int16_t speed, int16_t angle) {
    printf("move\n");
    uint8_t buffer[5];
    buffer[0] = IROBOT_CMD_SET_SPEED;
    buffer[1] = (uint8_t) (speed>>8);
    buffer[2] = (uint8_t) (speed);
    buffer[3] = (uint8_t) (angle>>8);
    buffer[4] = (uint8_t) (angle);
    irobot_write(buffer, 5);

    for (int i=0; i<5; i++) {
	    printf("%d ", buffer[i]);
    }
    printf("\n");
}



void irobot_move_stop() {
    printf("stop\n");
    // Stops the movement of the robot
    uint8_t buffer[5] = {IROBOT_CMD_SET_SPEED,0,0,0,0};
    irobot_write(buffer, 5);
}
