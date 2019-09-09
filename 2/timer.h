#ifndef		__TIMER_HEADER_
	#define 	__TIMER_HEADER_
        extern unsigned long int time;
        extern unsigned char refresh;
        extern unsigned int fre;
        extern unsigned int m;
	void TimerAInit( void );	//初始化一次TimerA
        void TimerAStop( void );
        void TimerAClear( void );
#endif