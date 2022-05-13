#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "fonts.h"
#include "st7735.h"

void mandelbrot()
{
    // Defines.

    // Iterations
    const int MAXIT = 50;
    int i;
    // Screen dimensions and pixel coords.
    int pX, pY;
    const float width = 160;
    const float height = 80;
    // Logical coordinate space.
    float cX, cY;
    const float xMin = -1.5;
    const float xMax = 1.5;
    const float yMin = -1.0;
    const float yMax = 1.0;
    // Pixels
    float pixelWidth = (xMax - xMin)/width;
    float pixelHeight = (yMax - yMin)/height;
    // Colour
    const int maxCol = 255;

    /** Maths */
    /*
       z = z*x + z*y*i
       (z*y)^2
    */
    float zx, zy;
    /*
       (z*x)^2
       (z*y)^2 */
    float zx2, zy2;

    // Escape radius
    const float er = 2;
    float er2 = er * er;

    /** Generation. */
    for (pY = 0; pY < height; pY++)
    {
        cY = yMin + pY * pixelHeight;
        if(fabs(pY) < pixelHeight/2){
            pY = 0.0;
        }

        for(pX=0; pX < width; pX++){
            cX = xMin + pX * pixelWidth;

            /** Initial orbit value z = 0 */
            zx = 0.0;
            zy = 0.0;
            zx2 = zx * zx;
            zx2 = zy * zy;

            /** Iterate */
            for (i = 0; i < MAXIT && ((zx2 * zy2) < er2); i++)
            {
                zy = 2 * zx * zy + cY;
                zx = zx2 - zy2 + cX;
                zx2 = zx * zx;
                zy2 = zy * zy;
            }


            /** Compute Colour*/
            if (i == MAXIT)
            {
                ST7735_DrawPixel(pY,pX, ST7735_BLACK);
            }else{
                double r = 255.0*((float)i/(float)MAXIT);
                ST7735_DrawPixel(pY,pX, ST7735_COLOR565((int)r,0,128));
            }

        }
    }
}

void ship()
{
    // Defines.

    // Iterations
    const int MAXIT = 300;
    int i;
    // Screen dimensions and pixel coords.
    int pX, pY;
    const float width = 160;
    const float height = 80;
    // Logical coordinate space.
    float cX, cY;

    /**
     *  x : -1.5 -> 1.5
     *  y : -2.5 -> 1.5 
     *
     * Is a good place to start.
     */

    const float xMin = -1.5;
    const float xMax = 0.6;
    const float yMin = 0;
    const float yMax = 3.5;
    // Pixels
    float pixelWidth = (xMax - xMin)/width;
    float pixelHeight = (yMax - yMin)/height;
    // Colour
    const int maxCol = 255;

    /** Maths */
    /*
       z = z*x + z*y*i
       (z*y)^2
    */
    float zx, xtemp, zy;
    /*
       (z*x)^2
       (z*y)^2 */
    float zx2, zy2;

    /** Generation. */
    for (pY = 0; pY < height; pY++)
    {
        cY = yMin + pY * pixelHeight;

        for(pX=0; pX < width; pX++){
            cX = xMin + pX * pixelWidth;

            /** Initial orbit value z = 0 */
            zx = cX;
            zy = cY;
            zx2 = zx * zx;
            zx2 = zy * zy;

            i = 0;
            /** Iterate */
            while(zx2 + zy2 < 4 && i< MAXIT){
                xtemp = zx2 - zy2 + cX;
                zy = 2*fabs(zx*zy) - cY;
                zx = xtemp;
                zx2 = zx * zx;
                zx2 = zy * zy;
                i = i + 1;
            }

            /** Compute Colour*/
            if (i == MAXIT)
            {
                ST7735_DrawPixel(pY,pX, ST7735_BLACK);
            }else{
                double r = 255.0*((float)i/(float)MAXIT);
                ST7735_DrawPixel(pY,pX, ST7735_COLOR565((int)r,50,128));
            }

        }
    }
}

int main()
{
    stdio_init_all();

    setvbuf(stdout, NULL, _IONBF, 0); // Disable line and block buffering on stdout (for talking through serial)
    sleep_ms(1000);                   // Give the Pico some time to think...
    ST7735_Init();                    // Initialise the screen

    mandelbrot();
    //ship();

    sleep_ms(20000); // Sleep for 20 seconds
}
