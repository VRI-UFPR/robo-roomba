#include <stdint.h>

int irobot_init(const char* path_ttyusb);
void irobot_close();

int irobot_start_safe();
int irobot_start_full();
void irobot_stop();


int irobot_play(uint8_t music_index);


int irobot_send_cmd(const uint8_t cmd);


void irobot_move(int16_t speed, int16_t angle);
void irobot_move_stop();