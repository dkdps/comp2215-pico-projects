#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define TOGGLE 20
#define CHANGE_NOTE 21

int notes[6] = {
    82, // E
    110, // A
    147, // D
    196, // G
    246, // B
    330 // e
};

// https://www.i-programmer.info/programming/hardware/14849-the-pico-in-c-basic-pwm.html?start=2
uint32_t pwm_set_freq_duty(uint slice_num,
       uint chan,uint32_t f, int d)
{
 uint32_t clock = 125000000;
 uint32_t divider16 = clock / f / 4096 + 
                           (clock % (f * 4096) != 0);
 if (divider16 / 16 == 0)
 divider16 = 16;
 uint32_t wrap = clock * 16 / divider16 / f - 1;
 pwm_set_clkdiv_int_frac(slice_num, divider16/16,
                                     divider16 & 0xF);
 pwm_set_wrap(slice_num, wrap);
 pwm_set_chan_level(slice_num, chan, wrap * d / 100);
 return wrap;
}

int main()
{
    // Init
    stdio_init_all();
    gpio_init(TOGGLE);
    gpio_init(CHANGE_NOTE);
    
    gpio_set_dir(TOGGLE,GPIO_IN);
    gpio_set_dir(CHANGE_NOTE,GPIO_IN);

    gpio_pull_up(TOGGLE);
    gpio_pull_up(CHANGE_NOTE);

    gpio_set_function(18, GPIO_FUNC_PWM);
    

    uint slice_num = pwm_gpio_to_slice_num(18);
    uint chan = pwm_gpio_to_channel(18);
    uint play = 0;
    uint tone = 0;

    while (true)
    {
        if(!gpio_get(TOGGLE)){
            play = 1 - play;
            sleep_ms(300);
        }

        if(!gpio_get(CHANGE_NOTE)){

            tone = (tone+1)%6;

            pwm_set_freq_duty(slice_num,chan,notes[tone],50);
            sleep_ms(500);
        }

        if(play){
            // Set the PWM running
            pwm_set_enabled(slice_num, true);
        } else{
            pwm_set_enabled(slice_num, false);
        }

    }

    return 0;
}


