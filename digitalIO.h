#ifndef _DIGITALIO_H_
#define _DIGITALIO_H_
#include "userIncludes.h"

using namespace std;

STATUS IOReadTask();

typedef class digitalIO
{
private:
	static const int IO_REG_BIT_NUM = 8;
	BOOL lastIORegBit[IO_REG_BIT_NUM];
	BOOL IORegBit[IO_REG_BIT_NUM];

public:
	STATUS init(void);
	STATUS write(UCHAR buff);
	STATUS read(UCHAR *buff);
	STATUS release();
	friend STATUS IOReadTask(void);
	friend STATUS checkInitStatus();
}DIO;
extern DIO digitalIO;

#endif
