#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_adc.h>

void leds_init() {
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_AN;
	gpio.GPIO_Pin = (GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5);
	GPIO_Init(GPIOA, &gpio);
}

void adc_init() {
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef adc_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit();
	ADC_StructInit(&ADC_InitStructure);
	adc_init.ADC_Mode = ADC_DualMode_RegSimult_InjecSimult;
	adc_init.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_CommonInit(&adc_init);
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
}

u16 readADC1(u8 channel) {
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

void Delay(unsigned int Val) {
	for (; Val != 0; Val--);
}

int main(void) {
	leds_init();
	adc_init();
	unsigned int bin_code;
	double voltage_x,voltage_y,voltage_z;
	do {
		bin_code = readADC1(ADC_Channel_1);
		voltage_x = bin_code*2.96 / 0xfff;
		//Delay(500000);
		bin_code = readADC1(ADC_Channel_3);
		voltage_y = bin_code*2.96 / 0xfff;
		//Delay(500000);
		bin_code = readADC1(ADC_Channel_5);
		voltage_z = bin_code*2.96 / 0xfff;
		//Delay(500000);
	} while (1);
}
