#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "userIncludes.h"
#include "digitalIO.h"

#define NEG false
#define POS true
using std::max_element;
using std::min_element;

STATUS runPreset();

class control
{
public:
	control();
	~control();
	STATUS driverEnable(const int node[AXIS_NUM]) const;
	STATUS driverEnableAct(const int node[AXIS_NUM]) const;
	STATUS driverEnableCheck(const int node[AXIS_NUM]) const;
	STATUS driverDisable(const int node[AXIS_NUM]) const;
	STATUS driverDisableAct(const int node[AXIS_NUM]) const;
	STATUS driverDisableCheck(const int node[AXIS_NUM]) const;
	STATUS findZeroPos() const;
	STATUS runCustom(const float pos, const float vel, const int channel) const;
	STATUS stop() const;
	STATUS error(const char *content,const int node[AXIS_NUM]) const;
	STATUS exit() const;
	STATUS faultLEDControl(const BOOL &isLightOn,const int node[AXIS_NUM]) const;
	friend STATUS runPreset();
};
typedef control CONTROL;
extern CONTROL control;

#endif
