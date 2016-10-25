#ifndef _FILE_H_
#define _FILE_H_
#include "userIncludes.h"
#include "dosFsLib.h"

using std::setw;
using std::setfill;
using rapidjson::Document;
using rapidjson::Value;

#define ALIG(n)  		setw(n) << setfill('0') <<
#define FILE_LINE_END 	"\r\n"

static const int TRAIL_SIZE = 3000;

typedef class log
{
private:
	ofstream logFileStream;
	ifstream trailFile;
	char*	 trailString;
	
public:
	Document jsonDoc;
	
public:
	log();
	~log();
	int writeLogFile(const char *op);
	int kk();
}LOG;
extern LOG file;

#endif
