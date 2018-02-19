/*
 * DispenserADC.h
 *
 *  Created on: 6 Dec 2016
 *      Author: felix
 */

#include <stdint.h>

#ifndef ADC_DISPENSERADC_H_
#define ADC_DISPENSERADC_H_


void initADC();
void initADCfreerunning();
uint16_t ADC_Read(uint16_t channel);

#endif /* ADC_DISPENSERADC_H_ */
