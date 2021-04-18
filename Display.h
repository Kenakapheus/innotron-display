/*
 * Display.h
 *
 *  Created on: 05.08.2017
 *      Author: Lukas
 */

// Display size: 24x156 px
// Buffer Size: 32Bit x 156

#ifndef DISPLAY_H_
#define DISPLAY_H_

void disp_mapAndUpdate(uint32_t * rawBuffer);
void disp_clear();

#endif /* DISPLAY_H_ */
