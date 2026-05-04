// ============================================================================
//  Header
// ============================================================================

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

#include "irobot.h"

#define DEBUG 1

// Comandos
#define IROBOT_CMD_START          128
#define IROBOT_CMD_MODE_SAFE      131
#define IROBOT_CMD_MODE_FULL      132
#define IROBOT_CMD_PRESS_SPOT     137
#define IROBOT_CMD_PRESS_CLEAN    138
#define IROBOT_CMD_SET_SPEED      137
#define IROBOT_CMD_PLAY_MUSIC     141
#define IROBOT_CMD_REQUEST_PACK   142
#define IROBOT_CMD_STOP           173

// Variavel global
int g_irobot_fd = -1;

#include <signal.h>

// ============================================================================
//  Signal Callback
// ============================================================================

void irobot_signal_callback(int dummy) {
    printf("FIM\n");
    irobot_close();
}


// ============================================================================
//  Init and Close
// ============================================================================

int irobot_init(const char* path_ttyusb) {
    signal(SIGINT, irobot_signal_callback);

    // Opens a communication with the iRobot in serial port 'port'.
    // Starts the robot in Safe Mode

    // Open the serial port
    // g_irobot_fd = open(path_ttyusb, O_RDWR | O_NOCTTY | O_NDELAY );
    g_irobot_fd = open(path_ttyusb, O_RDWR | O_NOCTTY );
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
        printf("Could not get settings from open port:\n");
        // std::cerr << " " << strerror(errno) << std::endl;
        return -1;
    }

    // Set iRobot serial settings
    settings.c_cflag = B115200 | CLOCAL | CREAD;
    // settings.c_iflag = IGNPAR;
    settings.c_oflag = 0;
    settings.c_lflag = 0;
    settings.c_cc[VMIN]  = 0;  // The minimum number of bytes to read before read() returns
    settings.c_cc[VTIME] = 1;  // The timeout in deciseconds (1/10th of a second)

    result = tcsetattr(g_irobot_fd, TCSANOW, &settings);
    if(result == -1) {
        printf("Could not set new settings\n");
        // std::cerr << "Could not set new settings: " << strerror(errno) << std::endl;
        return -1;
    }

    // Success
    return 0;
}

void irobot_close() {
    if (g_irobot_fd < 0) {
        return;
    }

    // Pára o robo
    irobot_move_stop();

    // Finaliza a comunicação
    irobot_send_cmd(IROBOT_CMD_STOP);
    close(g_irobot_fd);
    g_irobot_fd = -1;
}

// ============================================================================
//  Basic Functions
// ============================================================================

int irobot_write(const uint8_t* buffer, uint8_t nbytes ) {
    // Sai se nbytes == 0
    if ( nbytes == 0 ) {
        return 0;
    }

#if DEBUG
    char const* cmd_name = "        ";
    const uint8_t cmd_code = buffer[0];
    if ( cmd_code == IROBOT_CMD_START ) {
        cmd_name = "START   ";
    } else if ( cmd_code == IROBOT_CMD_MODE_FULL ) {
        cmd_name = "MODEFULL";
    } else if ( cmd_code == IROBOT_CMD_SET_SPEED ) {
        cmd_name = "SPEED   ";
    } else if ( cmd_code == IROBOT_CMD_STOP ) {
        cmd_name = "STOP    ";
    } else if ( cmd_code == IROBOT_CMD_REQUEST_PACK ) {
        cmd_name = "PACKAGE ";
    }
    printf("[IRobot::%s]: ", cmd_name);
    for (uint8_t i=0; i<nbytes; i++) {
        printf("%x ", buffer[i]);
    }
    printf("\n");
#endif

    // Envia os dados
    const int bytes = write(g_irobot_fd, (const void*) buffer, (size_t) nbytes );
    if ( bytes != nbytes ) {
        printf("Error\n");
    }
    return bytes;
}

int irobot_send_cmd(const uint8_t cmd) {
    irobot_write(&cmd, 1);
}

void irobot_sleep() {
    
}

// ============================================================================
//  Comandos
// ============================================================================

int irobot_start_safe() {
    irobot_send_cmd(IROBOT_CMD_START);
    irobot_send_cmd(IROBOT_CMD_MODE_SAFE);
    sleep(1);
    return 0;
}

int irobot_start_full() {
    irobot_send_cmd(IROBOT_CMD_START);
    irobot_send_cmd(IROBOT_CMD_MODE_FULL);
    sleep(1);
    return 0;
}

int irobot_play(uint8_t music_index) {
    const uint8_t buffer[2] = {IROBOT_CMD_PLAY_MUSIC, music_index};
    irobot_write(buffer, 2);
    return 0;
}

void irobot_move(int16_t speed, int16_t angle) {
    uint8_t buffer[5];
    buffer[0] = IROBOT_CMD_SET_SPEED;
    buffer[1] = (uint8_t) (speed>>8);
    buffer[2] = (uint8_t) (speed);
    buffer[3] = (uint8_t) (angle>>8);
    buffer[4] = (uint8_t) (angle);
    irobot_write(buffer, 5);
}

void irobot_move_stop() {
    // Stops the movement of the robot
    uint8_t buffer[5] = {IROBOT_CMD_SET_SPEED,0,0,0,0};
    irobot_write(buffer, 5);
}

void irobot_read(int* a, int* b, int* c, int* d) {
    // Stops the movement of the robot
    const uint8_t to_send[2] = {IROBOT_CMD_REQUEST_PACK,2};
    irobot_write(to_send, 2);

    // show
    char buffer[6];
    const int bytes = read(g_irobot_fd, buffer, (size_t) 6 );
    printf("opa %d\n", bytes);
}
