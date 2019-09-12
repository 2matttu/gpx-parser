#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {LOOK_LESS, LOOK_A, ATLAT, LOOKLON, ATLON, LOOKTIME, ATTIME} state;
typedef enum {L

/* states for the FSM:
 * LOOKTRKPT: looking for "<trkpt "
 * LOOKLAT: looking for "lat" attribute
 * ATLAT: recording "lat" attribute
 * LOOKLON: looking for "lat" attribute
 * ATLON: recording "lon" attribute
 * LOOKTIME: looking for "<time>" tag
 * ATTIME: recording "time"
 */

int main(int argc, char **argv)
    {
    //current state of FSM
    state curr = LOOKTRKPT;
    return 0;

    //index of currently read sample
}
