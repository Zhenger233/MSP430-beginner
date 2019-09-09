#ifndef		__AD_HEADER_
	#define 	__AD_HEADER_

	#define  LEN_VALUE 201

	void ADC12_init();
	extern unsigned int ADValue[LEN_VALUE];
        extern unsigned int flag;//采集完成标志
	//extern unsigned int f;
#endif
