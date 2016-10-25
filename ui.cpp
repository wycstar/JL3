#include "ui.h"

UI ui;

UGL_COLOR_TABLE colorTable[COLOR_NUM];

ui::ui():
	font16RepoBuffer(new unsigned char[ZH16_FILE_SIZE]),
	font24RepoBuffer(new unsigned char[ZH24_FILE_SIZE]),
	font32RepoBuffer(new unsigned char[ZH32_FILE_SIZE]),
	font48RepoBuffer(new unsigned char[ZH48_FILE_SIZE]),
	ascii16RepoBuffer(new unsigned char[ASCII16_FILE_SIZE]),
	ascii24RepoBuffer(new unsigned char[ASCII24_FILE_SIZE]),
	ascii32RepoBuffer(new unsigned char[ASCII32_FILE_SIZE]),
	ascii48RepoBuffer(new unsigned char[ASCII48_FILE_SIZE]),
	faultLine(0)
{
	
}

STATUS UI::displayFault(const char *str)
{
	char buf[UI_FAULT_STR_LEN];
	memset(buf, 0, UI_FAULT_STR_LEN);
	strcpy(buf, str);
	msgQSend(GV::faultLogMsg, buf, UI_FAULT_STR_LEN, WAIT_FOREVER, MSG_PRI_URGENT);
	return OK;
}

STATUS UI::init(void)
{
	char *uglDisplatName = NULL;
	UINT uglDisplatID = 0;
	UGL_REG_DATA *pRegistryData = NULL;
	UGL_MODE_INFO modeInfo;
	memset(font16RepoBuffer, 0, ZH16_FILE_SIZE);
	memset(font24RepoBuffer, 0, ZH24_FILE_SIZE);
	memset(font32RepoBuffer, 0, ZH32_FILE_SIZE);
	memset(font48RepoBuffer, 0, ZH48_FILE_SIZE);
	memset(ascii16RepoBuffer, 0, ASCII16_FILE_SIZE);
	memset(ascii24RepoBuffer, 0, ASCII24_FILE_SIZE);
	memset(ascii32RepoBuffer, 0, ASCII32_FILE_SIZE);
	memset(ascii48RepoBuffer, 0, ASCII48_FILE_SIZE);
	switch(uglDisplayOpen(uglDisplatHandle, &uglDisplatName, &uglDisplatID))
	{
	case UGL_STATUS_ERROR:
		printf("Ugl Device Open ERROR!\n");
		return ERROR;
	case UGL_STATUS_FINISHED:
		printf("Ugl Device Open Already Open!\n");
		return ERROR;
	default:
		printf("Ugl Device Open OK!\n");
	}
	if(NULL == (pRegistryData = uglRegistryFind (UGL_DISPLAY_TYPE, &uglDisplatID, 0, 0)))
	{
		printf("WindML Display not found!\n");
		uglDisplayClose(uglDisplatHandle);
		return UGL_STATUS_ERROR;
	}
	deviceID = reinterpret_cast<UGL_DEVICE_ID>(pRegistryData->id);
	pRegistryData = uglRegistryFind(UGL_INPUT_SERVICE_TYPE, &uglDisplatID, 0, 0);
	if(pRegistryData == UGL_NULL)
	{
		printf("WindML Input not found!\n");
		uglDisplayClose(uglDisplatHandle);
		return UGL_STATUS_ERROR;
	}
	inputServiceID = reinterpret_cast<UGL_INPUT_SERVICE_ID>(pRegistryData->id);
	graphicContext = uglGcCreate(deviceID);
	pRegistryData = uglRegistryFind(UGL_FONT_ENGINE_TYPE, &uglDisplatID, 0, 0);
	if(pRegistryData == UGL_NULL)
	{
		printf("WindML Font Engine not found!\n");
		uglDisplayClose(uglDisplatHandle);
		return UGL_STATUS_ERROR;
	}
	uglInfo(deviceID, UGL_MODE_INFO_REQ, &modeInfo);
	displayWidth  = modeInfo.width;
	displayHeight = modeInfo.height;
	colorTable[BACKGROUND_COLOR].rgbColor = UGL_MAKE_RGB(84, 84, 255);
	colorTable[FOREGROUND_NORMAL].rgbColor = UGL_MAKE_RGB(255, 255, 255);
	colorTable[FOREGROUND_ALERT].rgbColor = UGL_MAKE_RGB(255, 153, 102);
	for(int i = 0;i < COLOR_NUM;i++){
		uglColorAlloc(deviceID,
					  &colorTable[i].rgbColor,
					  UGL_NULL,
					  &colorTable[i].uglColor,
					  1);}
	uglBackgroundColorSet(graphicContext,
						  colorTable[BACKGROUND_COLOR].uglColor);
	uglForegroundColorSet(graphicContext,
						  colorTable[FOREGROUND_NORMAL].uglColor);
	uglLineStyleSet(graphicContext, UGL_LINE_STYLE_SOLID);
	uglLineWidthSet(graphicContext, 1);
	uglRectangle(graphicContext,
				 0,
				 0,
				 displayWidth,
				 displayHeight);
    ifstream file;
    string str;
    file.open("/ata0a/resource/font/GB2312");
    char *end = 0;
    while(getline(file, str))
        GBMap.insert(make_pair(strtoul(str.substr(9, 4).c_str(), &end, 16),
							   strtoul(str.substr(2, 4).c_str(), &end, 16)));
    file.close();
    string hanBasePath("/ata0a/resource/font/HZK");
    string ascBasePath("/ata0a/resource/font/ASC");
    for(int i = 2;i < 7;i++){
    	if(5 == i) continue;
    	stringstream ss;
    	ss << i * 8;
    	filePathMap.insert(make_pair(i * 8 , hanBasePath + ss.str()));
    	filePathMap.insert(make_pair(i * 80, ascBasePath + ss.str()));}
    STAT st;
    font16RepoFile.open("/ata0a/resource/font/HZK16", ios::in|ios::binary);
    font24RepoFile.open("/ata0a/resource/font/HZK24", ios::in|ios::binary);
    font32RepoFile.open("/ata0a/resource/font/HZK32", ios::in|ios::binary);
    font48RepoFile.open("/ata0a/resource/font/HZK48", ios::in|ios::binary);
    ascii16RepoFile.open("/ata0a/resource/font/ASC16", ios::in|ios::binary);
    ascii24RepoFile.open("/ata0a/resource/font/ASC24", ios::in|ios::binary);
    ascii32RepoFile.open("/ata0a/resource/font/ASC32", ios::in|ios::binary);
    ascii48RepoFile.open("/ata0a/resource/font/ASC48", ios::in|ios::binary);

    stat("/ata0a/resource/font/HZK16", &st);
    font16RepoFile.read((char *)font16RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/HZK24", &st);
    font24RepoFile.read((char *)font24RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/HZK32", &st);
    font32RepoFile.read((char *)font32RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/HZK48", &st);
    font48RepoFile.read((char *)font48RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/ASC16", &st);
    ascii16RepoFile.read((char *)ascii16RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/ASC24", &st);
    ascii24RepoFile.read((char *)ascii24RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/ASC32", &st);
    ascii32RepoFile.read((char *)ascii32RepoBuffer, st.st_size);
    stat("/ata0a/resource/font/ASC48", &st);
    ascii48RepoFile.read((char *)ascii48RepoBuffer, st.st_size);
    
    fontRepoMap.insert(make_pair(16 , font16RepoBuffer));
    fontRepoMap.insert(make_pair(24 , font24RepoBuffer));
    fontRepoMap.insert(make_pair(32 , font32RepoBuffer));
    fontRepoMap.insert(make_pair(48 , font48RepoBuffer));
    fontRepoMap.insert(make_pair(160, ascii16RepoBuffer));
    fontRepoMap.insert(make_pair(240, ascii24RepoBuffer));
    fontRepoMap.insert(make_pair(320, ascii32RepoBuffer));
    fontRepoMap.insert(make_pair(480, ascii48RepoBuffer));
    
	font16RepoFile.close();
	font24RepoFile.close();
	font32RepoFile.close();
	font48RepoFile.close();
	ascii16RepoFile.close();
	ascii24RepoFile.close();
	ascii32RepoFile.close();
	ascii48RepoFile.close();
	GV::faultLogMsg = msgQCreate(20,UI_FAULT_STR_LEN,MSG_Q_FIFO);
	uglDrawCustomBackground();
	taskSpawn("tPrintError",  100, VX_FP_TASK, 20000, (FUNCPTR)printFault,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	taskSpawn("tPrintStatus", 100, VX_FP_TASK, 20000, (FUNCPTR)printStatus, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	taskDelay(sysClkRateGet() * 2);
	displayFault("请等待转台初始化完成!");
	taskDelay(sysClkRateGet() * 3);
	return OK;
}

STATUS UI::release(void)
{
	uglFontDestroy(fontFixed);
	uglGcDestroy(graphicContext);
	uglDisplayClose(uglDisplatHandle);
	return UGL_STATUS_OK;
}


STATUS UI::uglDrawPixelFontA(const ZH_FONT_SIZE size,
							 const unsigned char ascii[],
							 int &xPos,
							 int  yPos,
							 UGL_COLOR color = colorTable[FOREGROUND_NORMAL].uglColor)
{
	int pSize[7] = {0, 0, 16, 48, 64, 0, 144};
	unsigned char buffer[pSize[size / 8]];
	unsigned long offSet = (ascii[0] - (size == 48 ? 32 : 0)) * pSize[size / 8];
	memcpy((char *)buffer, fontRepoMap[size * 10] + offSet, sizeof(buffer));
	for(int i = 0;i < size;i++){
		for(int j = 0;j < (pSize[size / 8] / size);j++){
			for(int k = 0; k < 8; k++){
				if(buffer[i * (pSize[size / 8] / size) + j] << k & 0x80){
					uglPixelSet(graphicContext,
								xPos + j * 8 + k,
								yPos + i,
								color);}}}}
	xPos += pSize[size / 8] * 9 / size;
	return UGL_STATUS_OK;
}

STATUS UI::uglDrawPixelFontW(const ZH_FONT_SIZE size,
							 const unsigned char zhHans[],
							 int &xPos,
							 int  yPos,
							 UGL_COLOR color = colorTable[FOREGROUND_NORMAL].uglColor)
{
	unsigned long unicode = ((zhHans[0] & 0x0f) << 12) +
	                        ((zhHans[1] & 0x3f) <<  6) +
	                         (zhHans[2] & 0x3f);
	unsigned char buffer[size * size / 8];
	unsigned long hanCode = GBMap[unicode] - (0x20 << 8) - 0x20;
	unsigned long offSet = ((((hanCode >> 8) & 0xff) - (((size % 3) == 0) ? 16 : 1)) * 94 +
							  (hanCode & 0xff) - 1) * size * size / 8;
	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, fontRepoMap[size] + offSet, sizeof(buffer));
		for(int i = 0;i < size;i++){
		for(int j = 0;j < size / 8;j++){
			for(int k = 0; k < 8; k++){
				if(buffer[i * size / 8 + j] << k & 0x80){
					uglPixelSet(graphicContext,
								xPos + ((size % 16) == 0 ? (j * 8 + k) : i),
								yPos + ((size % 16) == 0 ? i : (j * 8 + k)),
								color);}}}}
	xPos += (size * 9 / 8);
	return UGL_STATUS_OK;
}

STATUS UI::uglDrawTextAll(char *text,
						  const ZH_FONT_SIZE size,
						  int xPos,
						  int yPos,
						  UGL_COLOR color = colorTable[FOREGROUND_NORMAL].uglColor)
{
	unsigned char unicode[100][3];
	memset(unicode, 0, sizeof(unicode));
	int unicodeIndex = 0;
	while(*text != 0)
	{
	    memcpy(unicode[unicodeIndex++], text, (*text & 0x80) >> 7 ? 3 : 1);
	    text += (*text & 0x80) >> 7 ? 3 : 1;
	}
	for(int i = 0; unicode[i][0] != 0; i++)
	{
		if(unicode[i][1] == 0){
			uglDrawPixelFontA(size,
							  unicode[i],
							  xPos,
							  yPos,
							  color);}
			
		else{
			uglDrawPixelFontW(size,
							  unicode[i],
							  xPos,
							  yPos,
							  color);}
	}
	return UGL_STATUS_OK;
}

STATUS UI::uglDrawCustomBackground()
{
	//状态显示区域
	char *slogan = "4C23-3转台系统控制软件";
	uglDrawTextAll(slogan,
				   ZH_FONT_48,
				   230,
				   20);
	char axisLabel[5][20] = {
			"平台俯仰轴",
			"平台偏航轴",
			"平台滚动轴",
			"陀螺俯仰轴",
			"陀螺偏航轴"};
	for(UINT i = 0;i < 5;i++){
		uglDrawTextAll(axisLabel[i],
					   ZH_FONT_24,
					   204 * i + 35,
					   100);}
	//错误提示区域
	char *faultRegionTips = "错误信息";
	uglDrawTextAll(faultRegionTips,
				   ZH_FONT_24,
				   displayWidth / 2 - 52,
				   288);
	uglDrawLine(25,
				300,
				displayWidth / 2 - 70,
				300,
				colorTable[FOREGROUND_NORMAL].uglColor,
				UGL_LINE_STYLE_SOLID,
				5);
	uglDrawLine(displayWidth / 2 + 70,
				300,
				displayWidth - 25,
				300,
				colorTable[FOREGROUND_NORMAL].uglColor,
				UGL_LINE_STYLE_SOLID,
				5);
	uglDrawLine(25,
				300,
				25,
				displayHeight - 25,
				colorTable[FOREGROUND_NORMAL].uglColor,
				UGL_LINE_STYLE_SOLID,
				5);
	uglDrawLine(25,
				displayHeight - 25,
				displayWidth - 25,
				displayHeight - 25,
				colorTable[FOREGROUND_NORMAL].uglColor,
				UGL_LINE_STYLE_SOLID,
				5);
	uglDrawLine(displayWidth - 25,
				300,
				displayWidth - 25,
				displayHeight - 25,
				colorTable[FOREGROUND_NORMAL].uglColor,
				UGL_LINE_STYLE_SOLID,
				5);
	return UGL_STATUS_OK;
}

STATUS printStatus()
{
	char angleValue[AXIS_NUM][20];
	while(1)
	{
		for(int i = 0;i < 5;i++){
			memset(angleValue[i], 0, sizeof(angleValue[i]));
			sprintf(angleValue[i], "%8.4f%c", GV::axisActPos[i],'\xb0');}
		uglBatchStart(ui.graphicContext);
		uglForegroundColorSet(ui.graphicContext,
							  colorTable[FOREGROUND_NORMAL].uglColor);
		for(int i = 0;i < 5;i++){
			ui.uglDrawTextAll(angleValue[i],
							  ZH_FONT_24,
							  204 * i + 17,
							  140);}
		taskDelay(sysClkRateGet() / 2);
		uglForegroundColorSet(ui.graphicContext, colorTable [BACKGROUND_COLOR].uglColor);
		uglBackgroundColorSet(ui.graphicContext, colorTable [BACKGROUND_COLOR].uglColor);
	    uglRectangle(ui.graphicContext,
					 3,
					 139,
					 ui.displayWidth - 1,
					 165);
		uglBatchEnd(ui.graphicContext);
	}
	return OK;
}

STATUS printFault()
{
	char buf[UI_FAULT_STR_LEN];
	while(1){
		memset(buf, 0, UI_FAULT_STR_LEN);
		msgQReceive(GV::faultLogMsg, buf, UI_FAULT_STR_LEN, WAIT_FOREVER);
		if(strlen(buf) != 0){
			uglBatchStart(ui.graphicContext);
			uglForegroundColorSet(ui.graphicContext,
								  colorTable[FOREGROUND_ALERT].uglColor);
			ui.uglDrawTextAll(buf,
							  ZH_FONT_24,
							  35,
							  320 + 29 * ((ui.faultLine)++),
							  colorTable[FOREGROUND_ALERT].uglColor);
			uglBatchEnd(ui.graphicContext);
			if((ui.faultLine) >= 14){
				uglRectangle(ui.graphicContext,
							 30,
							 315,
							 ui.displayWidth - 30,
							 ui.displayHeight - 30);
				ui.faultLine = 0;}}}
	uglBatchStart(ui.graphicContext);
	ui.uglDrawTextAll("程序已退出!",
					   ZH_FONT_24,
					   35,
					   320 + 29 * ((ui.faultLine)++),
					   colorTable[FOREGROUND_ALERT].uglColor);
	uglBatchEnd(ui.graphicContext);
	return OK;
}

void UI::uglDrawLine(UGL_POS x1,
					 UGL_POS y1,
					 UGL_POS x2,
					 UGL_POS y2,
					 UGL_COLOR color,
					 UGL_LINE_STYLE lineStyle,
					 UGL_SIZE lineWidth)
{
	uglLineStyleSet(graphicContext, lineStyle);
	uglLineWidthSet(graphicContext, lineWidth);
	uglForegroundColorSet(graphicContext, color);
	uglLine(graphicContext, x1, y1, x2, y2);
}
