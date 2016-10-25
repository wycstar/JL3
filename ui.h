#ifndef _UI_H_
#define _UI_H_
#include "userIncludes.h"

static const int ASCII16_FILE_SIZE = 4100;
static const int ASCII24_FILE_SIZE = 12300;
static const int ASCII32_FILE_SIZE = 16400;
static const int ASCII48_FILE_SIZE = 13700;

static const int ZH16_FILE_SIZE = 300000;
static const int ZH24_FILE_SIZE = 500000;
static const int ZH32_FILE_SIZE = 1100000;
static const int ZH48_FILE_SIZE = 2000000;

static const int COLOR_NUM = 3;
#define BACKGROUND_COLOR  0
#define FOREGROUND_NORMAL 1
#define FOREGROUND_ALERT  2

typedef struct _tColorTable{
	UGL_ARGB  rgbColor;
	UGL_COLOR uglColor;
}UGL_COLOR_TABLE;

typedef enum {
	ZH_FONT_16 = 16,
	ZH_FONT_24 = 24,
	ZH_FONT_32 = 32,
	ZH_FONT_48 = 48
}ZH_FONT_SIZE;

STATUS printStatus();
STATUS printFault();

typedef class ui
{
private:
	UGL_DEVICE_ID deviceID;
	UGL_GC_ID graphicContext;
	UGL_INPUT_SERVICE_ID inputServiceID;
	UGL_FONT_ID fontDialog;
	UGL_FONT_ID fontSystem;
	UGL_FONT_ID fontFixed;
	UGL_FONT_DRIVER_ID fontDriverID;
	UGL_DDB_ID stdDdb;
	UGL_MDDB_ID patternDdb;
	UGL_CDDB_ID cursorDdb;
	UGL_TDDB_ID transDdb;
	int displayHeight;
	int displayWidth;
	int mode;
	char *uglDisplatHandle;
	ifstream font16RepoFile;
	ifstream font24RepoFile;
	ifstream font32RepoFile;
	ifstream font48RepoFile;
	ifstream ascii16RepoFile;
	ifstream ascii24RepoFile;
	ifstream ascii32RepoFile;
	ifstream ascii48RepoFile;
	UCHAR *font16RepoBuffer;
	UCHAR *font24RepoBuffer;
	UCHAR *font32RepoBuffer;
	UCHAR *font48RepoBuffer;
	UCHAR *ascii16RepoBuffer;
	UCHAR *ascii24RepoBuffer;
	UCHAR *ascii32RepoBuffer;
	UCHAR *ascii48RepoBuffer;
	map<ULONG, ULONG> GBMap;
	map<UINT,  UCHAR *> fontRepoMap;
	map<UINT,  string> filePathMap;
	int faultLine;
	
public:
	ui();
	STATUS displayFault(const char *str);
	STATUS init(void);
	STATUS release(void);
	STATUS uglDrawPixelFontA(const ZH_FONT_SIZE size,
							 const unsigned char ascii[],
							 int &xPos,
							 int  yPos,
							 UGL_COLOR color);
	STATUS uglDrawPixelFontW(const ZH_FONT_SIZE size,
							 const unsigned char zhHans[],
							 int &xPos,
							 int  yPos,
							 UGL_COLOR color);
	STATUS uglDrawTextAll(char *text,
						  const ZH_FONT_SIZE size,
						  int xPos,
						  int yPos,
						  UGL_COLOR color);
	void uglDrawLine(UGL_POS x1,
					 UGL_POS y1,
					 UGL_POS x2,
					 UGL_POS y2,
					 UGL_COLOR color,
					 UGL_LINE_STYLE lineStyle,
					 UGL_SIZE lineWidth);
	STATUS uglDrawCustomBackground();
	friend STATUS printStatus();
	friend STATUS printFault();
	
}UI;
extern UI ui;

#endif
