// ============================================================================
//  Cabeçalho
// ============================================================================

#include <stdio.h>
#include <unistd.h>

#include "irobot.h"


// ============================================================================
//  Main
// ============================================================================

int main() {
    // Inicializa a comunicação com o roomba
    int res = irobot_init("/dev/ttyUSB0");
    if ( res < 0 ) {
        return -1;
    }

    // Inicializa o roomba no modo irrestrito
    irobot_start_full();
   
    // Movimenta o roomba para frente por 1 segundo
    irobot_move(100, 0);
    sleep(1);
    
    // Pára o movimento do roomba
    irobot_move_stop();
    
    // Fecha a comunicação com o roomba
    irobot_close();
    return 0;
}