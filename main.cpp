#include "digitalIO.h"
#include "net.h"
#include "file.h"
#include "can.h"
#include "control.h"

using namespace std;

STATUS main()
{
	ui.init();
	printf("UI OK\n");
	init();
	printf("Init OK\n");
	digitalIO.init();
	printf("DIO\n");
	can.init();
	net.init();
	checkInitStatus();
	return 1;
}
