/*
 *
 * BaseConv.c
 *
 *
 */
 
#include <PalmOS.h>			// all the system toolbox headers
#include "BaseConvRsc.h"		// application resource defines

#define BaseConvAppType 'BScv'

enum {
	HEX  = 0,
	DEC,
	OCT,
	BIN
};

static MenuBarPtr CurrentMenu = NULL;

/*
 * Prototypes for internal functions
 */
//static void StartApplication(void);
//static Boolean MainFormHandleEvent(EventPtr event);
//static void EventLoop(void);

static void setHex(void);
static void setDec(void);
static void setOct(void);
static void setBin(void);

static void showHex(void);
static void showDec(void);
static void showOct(void);
static void showBin(void);

static char mode;
static void *pHex, *pDec, *pOct, *pBin;
static void *pHexVal, *pDecVal, *pOctVal, *pBinVal;
static void *pAbtn, *pBbtn, *pCbtn, *pDbtn, *pEbtn, *pFbtn;
static void *p2btn, *p3btn, *p4btn, *p5btn, *p6btn, *p7btn, *p8btn, *p9btn;
static void *pCode;
static void *pUp, *pDown;
static unsigned long value;


static void setHex()
{
	CtlSetValue(pHex, 1);
	CtlSetValue(pDec, 0);
	CtlSetValue(pOct, 0);
	CtlSetValue(pBin, 0);

	switch (mode) {
	case BIN:
		CtlShowControl(p2btn);
		CtlShowControl(p3btn);
		CtlShowControl(p4btn);
		CtlShowControl(p5btn);
		CtlShowControl(p6btn);
		CtlShowControl(p7btn);
	case OCT:
		CtlShowControl(p8btn);
		CtlShowControl(p9btn);
	}

	CtlShowControl(pAbtn);
	CtlShowControl(pBbtn);
	CtlShowControl(pCbtn);
	CtlShowControl(pDbtn);
	CtlShowControl(pEbtn);
	CtlShowControl(pFbtn);

	mode = HEX;
}


static void setDec()
{
	CtlSetValue(pHex, 0);
	CtlSetValue(pDec, 1);
	CtlSetValue(pOct, 0);
	CtlSetValue(pBin, 0);

	switch (mode) {
	case BIN:
		CtlShowControl(p2btn);
		CtlShowControl(p3btn);
		CtlShowControl(p4btn);
		CtlShowControl(p5btn);
		CtlShowControl(p6btn);
		CtlShowControl(p7btn);
	case OCT:
		CtlShowControl(p8btn);
		CtlShowControl(p9btn);
		break;
	case HEX:
		CtlHideControl(pAbtn);
		CtlHideControl(pBbtn);
		CtlHideControl(pCbtn);
		CtlHideControl(pDbtn);
		CtlHideControl(pEbtn);
		CtlHideControl(pFbtn);
		break;
	}

	mode = DEC;
}

static void setOct()
{
	CtlSetValue(pHex, 0);
	CtlSetValue(pDec, 0);
	CtlSetValue(pOct, 1);
	CtlSetValue(pBin, 0);

	switch (mode) {
	case BIN:
		CtlShowControl(p2btn);
		CtlShowControl(p3btn);
		CtlShowControl(p4btn);
		CtlShowControl(p5btn);
		CtlShowControl(p6btn);
		CtlShowControl(p7btn);
		break;
	case HEX:
		CtlHideControl(pAbtn);
		CtlHideControl(pBbtn);
		CtlHideControl(pCbtn);
		CtlHideControl(pDbtn);
		CtlHideControl(pEbtn);
		CtlHideControl(pFbtn);
	case DEC:
		CtlHideControl(p8btn);
		CtlHideControl(p9btn);
	}

	mode = OCT;
}

static void setBin()
{
	CtlSetValue(pHex, 0);
	CtlSetValue(pDec, 0);
	CtlSetValue(pOct, 0);
	CtlSetValue(pBin, 1);

	switch (mode) {
	case HEX:
		CtlHideControl(pAbtn);
		CtlHideControl(pBbtn);
		CtlHideControl(pCbtn);
		CtlHideControl(pDbtn);
		CtlHideControl(pEbtn);
		CtlHideControl(pFbtn);
	case DEC:
		CtlHideControl(p8btn);
		CtlHideControl(p9btn);
	case OCT:
		CtlHideControl(p2btn);
		CtlHideControl(p3btn);
		CtlHideControl(p4btn);
		CtlHideControl(p5btn);
		CtlHideControl(p6btn);
		CtlHideControl(p7btn);
	}

	mode = BIN;
}

static void showHex()
{
#ifdef OLD
	char str[5];

	StrPrintF(str, "%x", value);
	CtlSetLabel(pHexVal, str);
#else
	char str[5];
	int i, digit;
	unsigned long tmp = value;

	for (i = 0; i < 4; i++) {
		digit = tmp % 16;
		if (digit > 9)
			str[3 - i] = 'A' + digit - 10;
		else
			str[3 - i] = '0' + digit;
		tmp = (unsigned long)(tmp / 16);

	}
	str[4] = '\0';
	CtlSetLabel(pHexVal, str);
#endif

	if (value < 0x100) {
		str[0] = (char)value;
		str[1] = '\0';
		CtlSetLabel(pCode, str);
	} else {
		CtlSetLabel(pCode, "\002");
	}
}

static void showDec()
{
#ifndef OLD
	char str[6];
	int i;
	unsigned long tmp = value;

	for (i = 0; i < 5; i++) {
		str[4 - i] = '0' + tmp % 10;
		tmp = (unsigned long)(tmp / 10);

	}
	str[5] = '\0';
	CtlSetLabel(pDecVal, str);
#else
	char str[6];

	StrPrintF(str, "%u", value);
	CtlSetLabel(pDecVal, str);
#endif
}

static void showOct()
{
	char str[7];
	int i;
	unsigned long tmp = value;

	for (i = 0; i < 6; i++) {
		str[5 - i] = '0' + tmp % 8;
		tmp = (unsigned long)(tmp / 8);

	}
	str[6] = '\0';
	CtlSetLabel(pOctVal, str);
}

static void showBin()
{
	char str[17];
	int i;
	unsigned long tmp = value;

	for (i = 0; i < 16; i++) {
		str[15 - i] = '0' + tmp % 2;
		tmp = (unsigned long)(tmp / 2);
	}
	str[16] = '\0';
	CtlSetLabel(pBinVal, str);
}

static void showUpDwn()
{
	switch(value) {
	case 0:
		CtlSetLabel(pDown, "\004");
		CtlSetEnabled(pDown, false);
		break;
	case 0xffff:
		CtlSetLabel(pUp, "\003");
		CtlSetEnabled(pUp, false);
		break;
	default:
		CtlSetLabel(pUp, "\001");
		CtlSetLabel(pDown, "\002");
		CtlSetEnabled(pUp, true);
		CtlSetEnabled(pDown, true);
		break;
	}
}

/*
 * StartApplication
 *
 */
static Err StartApplication(void)
{
	FormPtr	frm;

	// Initialize and draw the main memo pad form.
	frm = FrmInitForm(BaseConvMainForm);	
	FrmSetActiveForm(frm);
	FrmDrawForm(frm);

	pHex = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1020));
	pDec = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1021));
	CtlSetValue(pDec, 1);
	mode = DEC;
	pOct = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1022));
	pBin = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1023));

	pHexVal = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1024));
	pDecVal = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1025));
	pOctVal = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1026));
	pBinVal = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1027));
	CtlSetEnabled(pHexVal, false);
	CtlSetEnabled(pDecVal, false);
	CtlSetEnabled(pOctVal, false);
	CtlSetEnabled(pBinVal, false);

	p2btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1032));
	p3btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1033));
	p4btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1034));
	p5btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1035));
	p6btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1036));
	p7btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1037));
	p8btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1038));
	p9btn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1039));
	pAbtn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1040));
	pBbtn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1041));
	pCbtn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1042));
	pDbtn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1043));
	pEbtn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1044));
	pFbtn = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, 1045));

	pCode = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, lbl_ascode));
	CtlSetEnabled(pCode, false);

	pUp = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, rbtn_up));
	pDown = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, rbtn_down));

	value = 0;
	showHex();
	showDec();
	showOct();
	showBin();

	return errNone;
}


/*
 *
 * MainFormHandleEvent
 *
 */
static Boolean MainFormHandleEvent(EventPtr event)
{
	Boolean		handled = false;
	EventType	newEvent;
	unsigned long try;
	FormPtr	frm;

	switch (event->eType) {
	case ctlSelectEvent:
		switch (event->data.ctlSelect.controlID) {
		case 1002:
			MemSet(&newEvent, sizeof(EventType), 0);
			newEvent.eType = appStopEvent;
			EvtAddEventToQueue(&newEvent);
			handled = true;
			break;
		case 1020:
			if (mode != HEX) {
				setHex();
			}
			break;
		case 1021:
			if (mode != DEC) {
				setDec();
			}
			break;
		case 1022:
			if (mode != OCT) {
				setOct();
			}
			break;
		case 1023:
			if (mode != BIN) {
				setBin();
			}
			break;
		case 1030:
		case 1031: case 1032: case 1033:
		case 1034: case 1035: case 1036:
		case 1037: case 1038: case 1039:
			switch (mode) {
			case HEX:
				try = value;
				value = value * 16 + event->data.ctlSelect.controlID - 1030;
				if (value > 0xffff) value = try;
				break;
			case DEC:
				try = value;
				value = value * 10 + event->data.ctlSelect.controlID - 1030;
				if (value > 0xffff) value = try;
				break;
			case OCT:
				try = value;
				value = value * 8 + event->data.ctlSelect.controlID - 1030;
				if (value > 0xffff) value = try;
				break;
			case BIN:
				try = value;
				value = value * 2 + event->data.ctlSelect.controlID - 1030;
				if (value > 0xffff) value = try;
				break;
			}
			showHex();
			showDec();
			showOct();
			showBin();
			showUpDwn();
			break;
		case 1040: case 1041: case 1042:
		case 1043: case 1044: case 1045:
			switch (mode) {
			case HEX:
				try = value;
				value = value * 16 + event->data.ctlSelect.controlID - 1030;
				if (value > 0xffff) value = try;
				break;
			default:
				break;
			}
			showHex();
			showDec();
			showOct();
			showBin();
			showUpDwn();
			break;
		case 1046:
			value = 0;
			showHex();
			showDec();
			showOct();
			showBin();
			showUpDwn();
			break;
		case 1047:
			switch (mode) {
			case HEX:
				value = (unsigned long)(value / 16);
				break;
			case DEC:
				value = (unsigned long)(value / 10);
				break;
			case OCT:
				value = (unsigned long)(value / 8);
				break;
			case BIN:
				value = (unsigned long)(value / 2);
				break;
			default:
				break;
			}
			showHex();
			showDec();
			showOct();
			showBin();
			showUpDwn();
			break;
		default:
			break;
		}
		break;
	case ctlRepeatEvent:
    case keyDownEvent:
		if ( (event->data.keyDown.chr == pageUpChr)
			|| (event->data.ctlRepeat.controlID == rbtn_up) ) {
			if (value != 0xffff) {
				value++;
			}
		} else if ((event->data.keyDown.chr == pageDownChr)
			|| (event->data.ctlRepeat.controlID == rbtn_down)) {
			if (value != 0) {
				value--;
			}
		}
		showHex();
		showDec();
		showOct();
		showBin();
		showUpDwn();
		break;
	case menuEvent:
		switch (event->data.menu.itemID) {
		case mit_Hex:
			setHex();
			//SndPlaySystemSound(sndClick);
			SndPlaySystemSound(4);
			handled = true;
			break;
		case mit_Dec:
			setDec();
			SndPlaySystemSound(4);
			handled = true;
			break;
		case mit_Oct:
			setOct();
			SndPlaySystemSound(4);
			handled = true;
			break;
		case mit_Bin:
			setBin();
			SndPlaySystemSound(4);
			handled = true;
			break;
		case mit_About:
			FrmAlert(alt_About);
			handled = true;
			break;
		default:
			break;
		}
	}

     return (handled);
}


/*
 *
 * EventLoop
 *
 */
static void EventLoop(void)
{
	EventType	event;
	Err error;
	
	do {
		// Get the next available event.
		EvtGetEvent(&event, evtWaitForever);
		
		// Give the system a chance to handle the event.
		if (!SysHandleEvent (&event))
		  if (!MenuHandleEvent(CurrentMenu, &event, &error))
			// Give the application a chance to handle the event.
			if (!MainFormHandleEvent(&event))
				// Let the form object provide default handling of the event.
				FrmHandleEvent(FrmGetActiveForm(), &event);
	} 
	while (event.eType != appStopEvent);
}

/*
 *
 * StopApplication
 */
static void StopApplication(void)
{
	FrmSaveAllForms();
	FrmCloseAllForms();
}

/*
 *
 * PilotMain
 *
 */
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error = errNone;

	// Check for a normal launch.
	switch (cmd) {
	case sysAppLaunchCmdNormalLaunch:
		// Set up initial form.
		error = StartApplication();
			
		// Start up the event loop.
		EventLoop();
	
		StopApplication();
		break;
	}
	
	return(0);
}

