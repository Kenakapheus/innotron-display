/*
 * Display.c
 *
 *  Created on: 05.08.2017
 *      Author: Lukas
 */

#include "stm32f0xx.h"

#include "util.h"
#include "heap.h"

#include "Display.h"

/*
 * Bitbanging for Display
 */

void disp_out(uint8_t d, uint8_t bits)
{
    for(uint8_t i = 0; i < bits; i++)
    {
        /*__NOP();
        __NOP();
        __NOP();*/
        __NOP();
        //_delay_us(1);
        GPIOB->BSRR = GPIO_BSRR_BR_10;

        if(d & (1L << i))
        {
            GPIOB->BSRR = GPIO_BSRR_BR_7;
        } else
        {
            GPIOB->BSRR = GPIO_BSRR_BS_7;
        }
        /*__NOP();
        __NOP();
        __NOP();*/
        __NOP();
        GPIOB->BSRR = GPIO_BSRR_BS_10;
    }
    /*SPI2->DR = d;
     while(SPI2->SR & SPI_SR_BSY)
     ;*/
}

void waitUS(int32_t t)
{
  for (int i = t*40; i > 0; i--)
  {
    __NOP();
  }
}


void disp_latch()
{
  // Latch Data
  waitUS(2);
  GPIOB->BSRR = GPIO_BSRR_BR_6;
  waitUS(20);
  GPIOB->BSRR = GPIO_BSRR_BS_6;
}

void disp_mapAndUpdate(uint32_t * rawBuffer)
{
    uint8_t * mappedBufferA = malloc(240);
    uint8_t * mappedBufferB = malloc(240);

    for(int32_t i = 0; i < 240; i++)
    {
        mappedBufferA[i] = 0x00;
        mappedBufferB[i] = 0x00;
    }

    int32_t pos = 0;
    int32_t rawPos = 0;

    for(int32_t j = 0; j < 12; j++)
    {

        //Collum 0
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 11)) ? (1 << 0) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 3)) ? (1 << 1) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 0)) ? (1 << 2) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 4)) ? (1 << 3) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 7)) ? (1 << 4) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 8)) ? (1 << 5) : 0;

        pos++;
        rawPos++;

        for(int32_t i = 0; i < 3; i++)
        {
            // Subblock (Col 1 - 3)

            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 7)) ? (1 << 0) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 8)) ? (1 << 1) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 11)) ? (1 << 2) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 3)) ? (1 << 3) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 0)) ? (1 << 4) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 4)) ? (1 << 5) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 7)) ? (1 << 6) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 8)) ? (1 << 7) : 0;

            pos++;
            rawPos += 1;

            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 0)) ? (1 << 0) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 4)) ? (1 << 1) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 7)) ? (1 << 2) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 8)) ? (1 << 3) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 11)) ? (1 << 4) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 3)) ? (1 << 5) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 0)) ? (1 << 6) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 4)) ? (1 << 7) : 0;

            pos++;
            rawPos += 2;

            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 11)) ? (1 << 0) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 3)) ? (1 << 1) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 0)) ? (1 << 2) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 4)) ? (1 << 3) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 7)) ? (1 << 4) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 8)) ? (1 << 5) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 11)) ? (1 << 6) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 3)) ? (1 << 7) : 0;

            pos++;
            rawPos += 1;
        }
    }
    rawPos -= 1;

    for(int32_t j = 0; j < 12; j++)
    {

        //Collum 0
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 12)) ? (1 << 0) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 20)) ? (1 << 1) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 23)) ? (1 << 2) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 19)) ? (1 << 3) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 16)) ? (1 << 4) : 0;
        mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 15)) ? (1 << 5) : 0;

        pos++;
        rawPos--;

        for(int32_t i = 0; i < 3; i++)
        {
            // Subblock (Col 1 - 3)

            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 16)) ? (1 << 0) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 15)) ? (1 << 1) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 12)) ? (1 << 2) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 20)) ? (1 << 3) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 23)) ? (1 << 4) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 19)) ? (1 << 5) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 16)) ? (1 << 6) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 15)) ? (1 << 7) : 0;

            pos++;
            rawPos--;

            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 23)) ? (1 << 0) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 19)) ? (1 << 1) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 16)) ? (1 << 2) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 15)) ? (1 << 3) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 12)) ? (1 << 4) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 20)) ? (1 << 5) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 23)) ? (1 << 6) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 19)) ? (1 << 7) : 0;

            pos++;
            rawPos -= 2;

            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 12)) ? (1 << 0) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 20)) ? (1 << 1) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 23)) ? (1 << 2) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 19)) ? (1 << 3) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 16)) ? (1 << 4) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos] & (0x100 << 15)) ? (1 << 5) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 12)) ? (1 << 6) : 0;
            mappedBufferA[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 20)) ? (1 << 7) : 0;

            pos++;
            rawPos--;
        }
    }

    pos = 0;
    rawPos = 0;

    for(int32_t j = 0; j < 12; j++)
    {

        //Collum 0
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 10)) ? (1 << 0) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 2)) ? (1 << 1) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 1)) ? (1 << 2) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 5)) ? (1 << 3) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 6)) ? (1 << 4) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 9)) ? (1 << 5) : 0;

        pos++;
        rawPos++;

        for(int32_t i = 0; i < 3; i++)
        {
            // Subblock (Col 1 - 3)

            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 6)) ? (1 << 0) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 9)) ? (1 << 1) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 10)) ? (1 << 2) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 2)) ? (1 << 3) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 1)) ? (1 << 4) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 5)) ? (1 << 5) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 6)) ? (1 << 6) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 9)) ? (1 << 7) : 0;

            pos++;
            rawPos++;

            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 1)) ? (1 << 0) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 5)) ? (1 << 1) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 6)) ? (1 << 2) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 9)) ? (1 << 3) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 10)) ? (1 << 4) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 2)) ? (1 << 5) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 1)) ? (1 << 6) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 5)) ? (1 << 7) : 0;

            pos++;
            rawPos += 2;

            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 10)) ? (1 << 0) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 2)) ? (1 << 1) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 1)) ? (1 << 2) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 5)) ? (1 << 3) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 6)) ? (1 << 4) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 9)) ? (1 << 5) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 10)) ? (1 << 6) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 2)) ? (1 << 7) : 0;

            pos++;
            rawPos++;
        }
    }

    rawPos -= 1;

    for(int32_t j = 0; j < 12; j++)
    {

        //Collum 0
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 13)) ? (1 << 0) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 21)) ? (1 << 1) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 22)) ? (1 << 2) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 18)) ? (1 << 3) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 17)) ? (1 << 4) : 0;
        mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 14)) ? (1 << 5) : 0;

        pos++;
        rawPos--;

        for(int32_t i = 0; i < 3; i++)
        {
            // Subblock (Col 1 - 3)

            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 17)) ? (1 << 0) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 14)) ? (1 << 1) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 13)) ? (1 << 2) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 21)) ? (1 << 3) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 22)) ? (1 << 4) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 18)) ? (1 << 5) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 17)) ? (1 << 6) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 14)) ? (1 << 7) : 0;

            pos++;
            rawPos--;

            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 22)) ? (1 << 0) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 18)) ? (1 << 1) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 17)) ? (1 << 2) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos - 1] & (0x100 << 14)) ? (1 << 3) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 13)) ? (1 << 4) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 21)) ? (1 << 5) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 22)) ? (1 << 6) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 18)) ? (1 << 7) : 0;

            pos++;
            rawPos -= 2;

            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 13)) ? (1 << 0) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 21)) ? (1 << 1) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 22)) ? (1 << 2) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 18)) ? (1 << 3) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 17)) ? (1 << 4) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos] & (0x100 << 14)) ? (1 << 5) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 13)) ? (1 << 6) : 0;
            mappedBufferB[pos] |= (rawBuffer[rawPos + 1] & (0x100 << 21)) ? (1 << 7) : 0;

            pos++;
            rawPos--;
        }
    }

    disp_out(0b00000000, 8);

    int32_t y = 240;

    for(int32_t x = 0; x < 24; x++)
    {
        y -= 10;
        disp_out(mappedBufferA[y + 9], 8); // A9
        disp_out(mappedBufferA[y + 8], 8); // A8
        disp_out(mappedBufferA[y + 7], 8); // A7
        disp_out(mappedBufferA[y + 6], 8); // A6
        disp_out(mappedBufferA[y + 5], 8); // A5

        disp_out(0, 8); // Command: 0 = Rows A, 4 = Rows B

        disp_out(mappedBufferA[y + 4], 8); // A4
        disp_out(mappedBufferA[y + 3], 8); // A3
        disp_out(mappedBufferA[y + 2], 8); // A2
        disp_out(mappedBufferA[y + 1], 8); // A1
        disp_out(mappedBufferA[y + 0], 8); // A0

        disp_out(0, 8);
    }

    disp_latch();

    disp_out(0b00000000, 8);

    y = 240;

    for(int32_t x = 0; x < 24; x++)
    {
        y -= 10;
        disp_out(mappedBufferB[y + 9], 8); // A9
        disp_out(mappedBufferB[y + 8], 8); // A8
        disp_out(mappedBufferB[y + 7], 8); // A7
        disp_out(mappedBufferB[y + 6], 8); // A6
        disp_out(mappedBufferB[y + 5], 8); // A5

        disp_out(4, 8); // Command: 0 = Rows A, 4 = Rows B

        disp_out(mappedBufferB[y + 4], 8); // A4
        disp_out(mappedBufferB[y + 3], 8); // A3
        disp_out(mappedBufferB[y + 2], 8); // A2
        disp_out(mappedBufferB[y + 1], 8); // A1
        disp_out(mappedBufferB[y + 0], 8); // A0

        disp_out(4, 8);
    }

    disp_latch();

    free(mappedBufferA);
    free(mappedBufferB);

}

void disp_clear()
{
    // Clear (Write 0 to all Pixels)
    for(int32_t i = 0; i < 48; i++)
    {
        disp_out(0x00, 8);
        disp_out(0x00, 8);
        disp_out(0x00, 8);
        disp_out(0x00, 8);
        disp_out(0x00, 8);

        disp_out(0x04, 8);
    }
    disp_latch();

    for(int32_t i = 0; i < 48; i++)
    {
        disp_out(0x00, 8);
        disp_out(0x00, 8);
        disp_out(0x00, 8);
        disp_out(0x00, 8);
        disp_out(0x00, 8);

        disp_out(0x00, 8);
    }
    disp_latch();
}
