#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/sync.h"
#include "ff.h"
#include "f_util.h"
#include "hw_config.h"

#define TOGGLE 20
#define CHANGE_NOTE 21
#define AUDIO_PIN 18

// File data.
uint8_t* data = NULL;
UINT bytesRead;
UINT size;
int songsize = 0;
FRESULT frs;
FRESULT frb;
FIL fil;

// Song tracking.
const char *songs[4] = {"song.ba","song2.ba","song3.ba","song4.ba"};

int songnum = 1;
int total_songs = 4;
int wav_pos = 0;
bool done; // Start by loading file.

void pwm_interrupt_handler(){
    
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));

    if(wav_pos < (songsize<<3) - 1){
        // set pwm level
        // allow pwm value to repeat for 8 cycles
        pwm_set_gpio_level(AUDIO_PIN, data[wav_pos>>3]);

        wav_pos++;
    } else {
        // loop to start.
        done = true;

        // Free memory for next song.
        wav_pos = 0;
    }
}

void loadSong(){

    if(data != NULL){
        free(data);
        printf("Memory Freed\n");
    }

    songnum = (songnum + 1) % total_songs;

    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    sd_card_t *pSD = sd_get_by_num(0);

    // Register the work area of the volume.
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    printf("Mount OK\n");

    // Open file.
    const char* const filename = songs[songnum];
    printf("Loading %d %s\n", songnum, filename);

    fr = f_open(&fil, filename, FA_READ);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    printf("Open OK\n");
    

    //  Wave data length
    frs = f_read(&fil, &songsize, 4, &bytesRead);

    // Size of file, allocate buffer.
    size = f_size(&fil);
    data = malloc(songsize);
    printf("Malloc OK\n");

    // Read from file
    frb = f_read(&fil, data, (UINT) (size - 4), &bytesRead);

    if (frb != FR_OK) {
        printf("f_read failed\n");
        panic("Cannot run, file cannot be read.");
    }

    // Close the file.
    fr = f_close(&fil);

    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    printf("Close OK\n");

    // Deregister the workspace.
    f_unmount(pSD->pcName);
    
}


int main()
{
    // Init
    stdio_init_all();
    set_sys_clock_khz(176000, true);
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
    done=true;

    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);

    // Setup PWM interrup to fire when cycle complete.
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);

    // Point interrupt to handler.
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler);

    // configure PWM for audio
    pwm_config config = pwm_get_default_config();
    

    // clkdiv depends on sample rate. 11khz - 8 divs

    pwm_config_set_clkdiv(&config, 8.0f);
    pwm_config_set_wrap(&config, 250);
    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(AUDIO_PIN,0);

    // Main loop
    while(1){
        // Play the audio File
        while(done==false){
            __wfi(); // Wait for interrupt
        }

        // Disable interrupts
        irq_set_enabled(PWM_IRQ_WRAP, false);
        
        // Load the next one
        loadSong();

        // Re-enable interrupts and play audio
        done=false;
        irq_set_enabled(PWM_IRQ_WRAP, true);
        
    }

    return 0;
}


