#include <stdlib.h>
#include <stdio.h>

/* kill sending -1 goes to all process for which the calling process has permission to send except 1 ( init ), check it by [ man kill ] */

#include <signal.h> /*for signal() and raise()*/

int main(){
    kill(-1, SIGKILL);
}
