#include "file.h"

LOG file;

log::log():trailString(new char[TRAIL_SIZE])
{
	logFileStream.open("/ata00:2/sysLog.ini",ios::app);
	if(NULL == logFileStream){
		DISPLAY_FAULT("日志文件打开失败!");}
	ST_DATETIME tTimeStruct;
	GetSysDateTime(&tTimeStruct);
	stringstream ss;
	ss << tTimeStruct.wYear+2000 << "年" << ALIG(2) static_cast<unsigned>(tTimeStruct.ucMonth) << "月" <<
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucDay)    << "日" << "  " <<
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucHour)   << ":"  << 
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucMinute) << ":"  << 
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucSecond);
	logFileStream << "[[" << FILE_LINE_END << "开机时间:" << ss.str() << FILE_LINE_END;
	logFileStream.close();
	STAT st;
	stat("/ata00:2/trail.json", &st);
	memset(trailString, 0, TRAIL_SIZE);
	ifstream trailFile("/ata00:2/trail.json",ios::in);
    trailFile.read(trailString, st.st_size);
    jsonDoc.Parse<0>(trailString);
	trailFile.close();
}

LOG::~log()
{

}

int LOG::kk()
{
    int sTime = jsonDoc["1"]["seg"].GetInt();
    printf(trailString);
    printf("\n%d\n", sTime);
    for(int i = 0; i < sTime;i++)
    {
    	printf("%f-", jsonDoc["1"]["angle"][i].GetDouble());
    }
    printf("\n");
	return 1;
}

int LOG::writeLogFile(const char *op)
{
	stringstream ss;
	ST_DATETIME  tTimeStruct;
	GetSysDateTime(&tTimeStruct);
	ss << 							  tTimeStruct.wYear+2000 << "年" << 
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucMonth) 	 << "月" <<
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucDay)     << "日" << "  " <<
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucHour)    << ":"  << 
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucMinute)  << ":"  << 
		ALIG(2) static_cast<unsigned>(tTimeStruct.ucSecond)  << "  " << op;
	logFileStream.open("/ata00:2/sysLog.ini",ios::app);
	logFileStream << ss.str() << FILE_LINE_END;
	logFileStream.close();
	return 0;
}
