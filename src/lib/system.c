#include <stdint.h>

#include <pit.h>

/* Sleep the processe for N milliseconds
*/
void sleep(float ms){
    uint64_t target = ticks + (uint64_t)(ms);
    while (ticks < target)
    {
        __asm__ __volatile__("hlt");
    }
}