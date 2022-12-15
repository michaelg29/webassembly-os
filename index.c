#include <emscripten.h>
#include <string.h>

/**
 * @brief Send data to JavaScript.
 */
extern void trace(int channel, int data);

int SYSTICK;

#define MAX_IRQ 16
#define TIM_IRQ 0
#define KEY_IRQ 1
typedef void (*isr)();
isr NVIC[MAX_IRQ];

void tim_isr()
{
    trace(1, SYSTICK++);
}

void key_isr()
{
    trace(2, 1);

    int *ptr = (int *)4000;
    trace(2, *ptr);
}

// ===========================
// EXPORTED FUNCTIONS
// ===========================

/**
 * @brief Initialization function for the OS.
 */
EMSCRIPTEN_KEEPALIVE
int main()
{
    trace(0, 0);

    // initial value of SYSTICK
    SYSTICK = 0;

    // NVIC setup
    memset(NVIC, 0, sizeof(NVIC));
    NVIC[TIM_IRQ] = tim_isr;
    NVIC[KEY_IRQ] = key_isr;
}

EMSCRIPTEN_KEEPALIVE
void irq_handler(int irq)
{
    if (irq < MAX_IRQ && NVIC[irq])
    {
        NVIC[irq]();
    }
}
