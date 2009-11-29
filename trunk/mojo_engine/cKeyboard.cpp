/********************************************************************************************************
/*
/*    cKeyboard.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*********************************************************************************************************/

#include "stdafx.h"
#include <windowsx.h>

using namespace mojo;

//=============================================================================================
// DATA
//=============================================================================================

//  VIRTUAL KEY TABLE
//  The index is equal to the virtual key code
//  If the key is extended, the wExVK equals VK + 256
//  Otherwise the wExVK is the same as the VK

cKeyboard::sExVK cKeyboard::aExVK [] =       
{
	{ L"",					L"",				0x00	},	// I put this in to make the indices work out
    { L"LBUTTON",			L"LButton",			0x01	},
    { L"RBUTTON",			L"RButton",			0x02	},
	{ L"",					L"",				0x03	}, // Ctrl-break == Cancel, handled as extended
    { L"MBUTTON",			L"MButton",			0x04	},	/* NOT contiguous with L & RBUTTON */
    { L"BUTTON4",			L"Button4",			0x05	},	/* NOT contiguous with L & RBUTTON */
    { L"BUTTON5",			L"Button5",			0x06	},	/* NOT contiguous with L & RBUTTON */
	{ L"",					L"",				0x07	},	// UNASSIGEND
    { L"BACKSPACE",			L"Backspace",		0x08	},
    { L"TAB",				L"Tab",				0x09	},
	{ L"WHEELFORWARD",		L"WheelForward",	0x0A	},	// reserved  -- I ADDED THIS
	{ L"WHEELBACKWARD",		L"WheelBackward",	0x0B	},	// reserved  -- I ADDED THIS
    { L"CLEAR",				L"Clear",			0x0C	},
    { L"ENTER",				L"Enter",			0x0D	},	// MS CALLS THIS RETURN
	{ L"",					L"",				0x0E	},	// I put this in to make the indices work out
	{ L"",					L"",				0x0F	},	// I put this in to make the indices work out
    { L"SHIFT",				L"Shift",			0x10	},
    { L"CTRL",				L"Ctrl",			0x11	},	// MS CALLS THIS CONTROL
    { L"ALT",				L"Alt",				0x12	},	// MS CALLS THIS MENU
    { L"PAUSE",				L"Pause",			0x13	},
    { L"CAPSLOCK",			L"CapsLock",		0x14	},	// MS CALLS THIS CAPITAL
    { L"KANA",				L"Kana",			0x15	},
    { L"HANGUL",			L"Hangul",			0x15	},	// no x16 after hangul
    { L"JUNJA",				L"Junja",			0x17	},
    { L"FINAL",				L"Final",			0x18	},
    { L"HANJA",				L"Hanja",			0x19	},
    { L"KANJI",				L"Kanji",			0x19	},	// no x1A after kanji
    { L"ESC",				L"Esc",				0x1B	},	// MICROSOFT CALLS THIS ESCAPE
    { L"CONVERT",        	L"Convert",			0x1C	},	// not sure i matched VK and DIK properly
    { L"NOCONVERT",      	L"NoConvert",		0x1D	},	// ms calls this NONCONVERT
    { L"ACCEPT",         	L"Accept",			0x1E	},
    { L"MODECHANGE",     	L"ModeChange",		0x1F	},
    { L"SPACE",				L"Space",			0x20	},
    { L"NUMPADPGUP",		L"NumpadPgUp",		0x21	}, // MICROSOFT CALLS THIS PRIOR
    { L"NUMPADPGDN",		L"NumpadPgDn",		0x22	}, // MICROSOFT CALLS THIS NEXT
    { L"NUMPADEND",			L"NumpadEnd",		0x23	},
    { L"NUMPADHOME",		L"NumpadHome",		0x24	},
    { L"NUMPADLEFT",		L"NumpadLeft",		0x25	},
    { L"NUMPADUP",			L"NumpadUp",		0x26	},
    { L"NUMPADRIGHT",		L"NumpadRight",		0x27	},
    { L"NUMPADDOWN",		L"NumpadDown",		0x28	},
    { L"SELECT",			L"Select",			0x29	},
    { L"PRINT",				L"Print",			0x2A	},
    { L"EXECUTE",			L"Execute",			0x2B	},
    { L"",					L"",				0x2C	}, // PRINTSCREEN MOVED TO EXTENDED
    { L"NUMPADINSERT",		L"NumpadInsert",	0x2D	}, // MICROSOFT CALLS THIS INSERT
    { L"NUMPADDELETE",		L"NumpadDelete",	0x2E	}, // MICROSOFT CALLS THIS DELETE
    { L"HELP",				L"Help",			0x2F	},
	{ L"0",					L"0",				0x30	},
	{ L"1",					L"1",				0x31	},
	{ L"2",					L"2",				0x32	},
	{ L"3",					L"3",				0x33	},
	{ L"4",					L"4",				0x34	},
	{ L"5",					L"5",				0x35	},
	{ L"6",					L"6",				0x36	},
	{ L"7",					L"7",				0x37	},
	{ L"8",					L"8",				0x38	},
	{ L"9",					L"9",				0x39	},
	{ L"",					L"",				0x3A	}, // I put this in to make the indices work out
	{ L"",					L"",				0x3B	}, // I put this in to make the indices work out
	{ L"",					L"",				0x3C	}, // I put this in to make the indices work out
	{ L"",					L"",				0x3D	}, // I put this in to make the indices work out
	{ L"",					L"",				0x3E	}, // I put this in to make the indices work out
	{ L"",					L"",				0x3F	}, // I put this in to make the indices work out
	{ L"",					L"",				0x40	}, // UNASSIGNED
	{ L"A",					L"A",				0x41	},
	{ L"B",					L"B",				0x42	},
	{ L"C",					L"C",				0x43	},
	{ L"D",					L"D",				0x44	},
	{ L"E",					L"E",				0x45	},
	{ L"F",					L"F",				0x46	},
	{ L"G",					L"G",				0x47	},
	{ L"H",					L"H",				0x48	},
	{ L"I",					L"I",				0x49	},
	{ L"J",					L"J",				0x4A	},
	{ L"K",					L"K",				0x4B	},
	{ L"L",					L"L",				0x4C	},
	{ L"M",					L"M",				0x4D	},
	{ L"N",					L"N",				0x4E	},
	{ L"O",					L"O",				0x4F	},
	{ L"P",					L"P",				0x50	}, 
	{ L"Q",					L"Q",				0x51	},
	{ L"R",					L"R",				0x52	},
	{ L"S",					L"S",				0x53	},
	{ L"T",					L"T",				0x54	},
	{ L"U",					L"U",				0x55	},
	{ L"V",					L"V",				0x56	},
	{ L"W",					L"W",				0x57	},
	{ L"X",					L"X",				0x58	},
	{ L"Y",					L"Y",				0x59	},
	{ L"Z",					L"Z",				0x5A	}, // LWin extended
	{ L"",					L"",				0x5B	},
	{ L"",					L"",				0x5C	}, // RWin extended
	{ L"",					L"",				0x5D	}, // APPS extended
	{ L"",					L"",				0x5E 	}, // RESERVED
	{ L"",					L"",				0x5F 	}, // SLEEP EXTENDED
    { L"NUMPAD0",			L"Numpad0",			0x60 	},
    { L"NUMPAD1",			L"Numpad1",			0x61 	},
    { L"NUMPAD2",			L"Numpad2",			0x62 	},
    { L"NUMPAD3",			L"Numpad3",			0x63 	},
    { L"NUMPAD4",			L"Numpad4",			0x64 	},
    { L"NUMPAD5",			L"Numpad5",			0x65 	},
    { L"NUMPAD6",			L"Numpad6",			0x66 	},
    { L"NUMPAD7",			L"Numpad7",			0x67 	},
    { L"NUMPAD8",			L"Numpad8",			0x68 	},
    { L"NUMPAD9",			L"Numpad9",			0x69 	},
    { L"MULTIPLY",			L"Multiply",		0x6A 	},
    { L"NUMPADPLUS",		L"NumpadPlus",		0x6B 	},
    { L"SEPARATOR",			L"Separator",		0x6C 	},
    { L"NUMPADMINUS",		L"NumpadMinus",		0x6D 	},
    { L"DECIMAL",			L"Decimal",			0x6E 	}, 
	{ L"",					L"",				0x6F 	},   // DIVIDE extended
    { L"F1",				L"F1",				0x70 	},
    { L"F2",				L"F2",				0x71 	},
    { L"F3",				L"F3",				0x72 	},
    { L"F4",				L"F4",				0x73 	},
    { L"F5",				L"F5",				0x74 	},
    { L"F6",             	L"F6",				0x75 	},
    { L"F7",             	L"F7",				0x76 	},
    { L"F8",             	L"F8",				0x77 	},
    { L"F9",             	L"F9",				0x78 	},
    { L"F10",            	L"F10",				0x79 	},
    { L"F11",            	L"F11",				0x7A 	},
    { L"F12",            	L"F12",				0x7B 	},
    { L"F13",            	L"F13",				0x7C 	},
    { L"F14",            	L"F14",				0x7D 	},
    { L"F15",            	L"F15",				0x7E 	},
    { L"F16",            	L"F16",				0x7F 	},
    { L"F17",            	L"F17",				0x80 	},
    { L"F18",            	L"F18",				0x81 	},
    { L"F19",            	L"F19",				0x82 	},
    { L"F20",            	L"F20",				0x83 	},
    { L"F21",            	L"F21",				0x84 	},
    { L"F22",            	L"F22",				0x85 	},
    { L"F23",            	L"F23",				0x86 	},
    { L"F24",            	L"F24",				0x87 	},
	{ L"",               	L"",				0x88 }, // UNASSIGNED
	{ L"",               	L"",				0x89 }, // UNASSIGNED
	{ L"",               	L"",				0x8A }, // UNASSIGNED
	{ L"",               	L"",				0x8B }, // UNASSIGNED
	{ L"",               	L"",				0x8C }, // UNASSIGNED
	{ L"",               	L"",				0x8D }, // UNASSIGNED
	{ L"",               	L"",				0x8E }, // UNASSIGNED
	{ L"",               	L"",				0x8F }, // UNASSIGNED
	{ L"",               	L"",				0x90 },
    { L"SCROLLLOCK",     	L"ScrollLock",		0x91 },
    { L"OEMNECEQUAL",		L"OemNecEqual",		0x92 },   // '=' key on numpad -- THIS IS EXTRA 92 SO I COMMENTED IT OUT
//  { 	L"OEMFJJISHO",							0x92 },   // 'Dictionary' key
    { L"OEMFJMASSHOU",		L"OemFjMasshou",	0x93 },   // 'Unregister word' key
    { L"OEMFJTOUROKU",		L"OemFjTouroku",	0x94 },   // 'Register word' key
    { L"OEMFJLOYA",			L"OemFjLoya",		0x95 },   // 'Left OYAYUBI' key
    { L"OEMFJROYA",			L"OemFjRoya",		0x96 },   // 'Right OYAYUBI' key
	{ L"",               	L"",				0x97 }, // UNASSIGNED
	{ L"",               	L"",				0x98 }, // UNASSIGNED
	{ L"",               	L"",				0x99 }, // UNASSIGNED
	{ L"",               	L"",				0x9A }, // UNASSIGNED
	{ L"",               	L"",				0x9B }, // UNASSIGNED
	{ L"",               	L"",				0x9C }, // UNASSIGNED
	{ L"",               	L"",				0x9D }, // UNASSIGNED
	{ L"",               	L"",				0x9E }, // UNASSIGNED
	{ L"",               	L"",				0x9F }, // UNASSIGNED
/*
 * VKL* & VKR* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
    { L"LSHIFT",         	L"LShift",			0xA0 },
    { L"RSHIFT",         	L"RShift",			0xA1 },
    { L"LCTRL",          	L"LCtrl",			0xA2 }, // MICROSOFT CALLS THIS LCONTROL
    { L"RCTRL",          	L"RCtrl",			0xA3 }, // MICROSOFT CALLS THIS RCONTROL
    { L"LALT",           	L"LAlt",			0xA4 }, // MICROSOFT CALLS THIS LMENU
    { L"RALT",           	L"RAlt",			0xA5 }, // MICROSOFT CALLS THIS RMENU

	{ L"",					L"",				0xA6 }, // A6 - B7 are extended keys listed below with 0x100 added to them
	{ L"",					L"",				0xA7 },
	{ L"",					L"",				0xA8 },
	{ L"",					L"",				0xA9 },
	{ L"",					L"",				0xAA },
	{ L"",					L"",				0xAB },
	{ L"",					L"",				0xAC },
	{ L"",					L"",				0xAD },
	{ L"",					L"",				0xAE },
	{ L"",					L"",				0xAF },
	{ L"",					L"",				0xB0 },
	{ L"",					L"",				0xB1 },
	{ L"",					L"",				0xB2 },
	{ L"",					L"",				0xB3 },
	{ L"",					L"",				0xB4 },
	{ L"",					L"",				0xB5 },
	{ L"",					L"",				0xB6 },
	{ L"",					L"",				0xB7 },
	{ L"",               	L"",				0xB8 },  // RESERVED
	{ L"",               	L"",				0xB9 },  // RESERVED
    { L"OEM1",				L"Oem1",			0xBA },  // ';:' for US
    { L"PLUS",				L"Plus",			0xBB },  // ms calls this OEMPLUS is + on main kb  // '+' any country
    { L"COMMA",				L"Comma",			0xBC },  // ',' any country
    { L"MINUS",				L"Minus",			0xBD },  // ms calls this OEMMINUS'-' on main kb - any country
    { L"PERIOD",			L"Period",			0xBE },  // '.' any country
    { L"OEM2",				L"Oem2",			0xBF },  // '/?' for US
    { L"OEM3",				L"Oem3",			0xC0 },  // MS CALLS THIS OEM3'`~' for US
    { L"",               	L"",				0xC1 },  // RESERVED
    { L"",               	L"",				0xC2 },  // RESERVED
    { L"",               	L"",				0xC3 },  // RESERVED
    { L"",               	L"",				0xC4 },  // RESERVED
    { L"",               	L"",				0xC5 },  // RESERVED
    { L"",               	L"",				0xC6 },  // RESERVED
    { L"",               	L"",				0xC7 },  // RESERVED
    { L"",               	L"",				0xC8 },  // RESERVED
    { L"",               	L"",				0xC9 },  // RESERVED
    { L"",               	L"",				0xCA },  // RESERVED
    { L"",               	L"",				0xCB },  // RESERVED
    { L"",               	L"",				0xCC },  // RESERVED
    { L"",               	L"",				0xCD },  // RESERVED
    { L"",               	L"",				0xCE },  // RESERVED
    { L"",               	L"",				0xCF },  // RESERVED
	{ L"",               	L"",				0xD0 },  // RESERVED
    { L"",               	L"",				0xD1 },  // RESERVED
    { L"",              	L"",				0xD2 },  // RESERVED
    { L"",               	L"",				0xD3 },  // RESERVED
    { L"",               	L"",				0xD4 },  // RESERVED
    { L"",               	L"",				0xD5 },  // RESERVED
    { L"",               	L"",				0xD6 },  // RESERVED
    { L"",               	L"",				0xD7 },  // RESERVED
    { L"",               	L"",				0xD8 },  // UNASSIGNED
    { L"",               	L"",				0xD9 },  // UNASSIGNED
    { L"",               	L"",				0xDA },  // UNASSIGNED
    { L"OEM4",           	L"Oem4",			0xDB },  //  MS CALLS THIS OEM4'[{' for US
    { L"OEM5",		    	L"Oem5",			0xDC },  // BACKSLASH ON US KEYBOARDS
    { L"OEM6",				L"Oem6",			0xDD },  //  ']}' for US
    { L"OEM7",				L"Oem7",			0xDE },  //  ''"' for US
    { L"OEM8",				L"Oem8",			0xDF },
    { L"",               	L"",				0xE0 },  // RESERVED
/*
 * Various extended or enhanced keyboards
 */
    { L"OEMAX",				L"OemAx",			0xE1 },  //  'AX' key on Japanese AX kbd
    { L"OEM102",			L"Oem102",			0xE2 },  //  "<>" or "\|" on RT 102-key kbd.
    { L"ICOHELP",			L"IcoHelp",			0xE3 },  //  Help key on ICO
    { L"ICO00",				L"Ico00",			0xE4 },  //  00 key on ICO
    { L"PROCESSKEY",     	L"ProcessKey",		0xE5 },
    { L"ICOCLEAR",			L"IcoClear",		0xE6 },
    { L"PACKET",         	L"Packet",			0xE7 },
    { L"",               	L"",				0xE8 }, // UNASSIGNED
/*
 * Nokia/Ericsson definitions
 */
    { L"OEMRESET",			L"OemReset",		0xE9 },
    { L"OEMJUMP",			L"OemJump",			0xEA },
    { L"OEMPA1",			L"OemPa1",			0xEB },
    { L"OEMPA2",			L"OemPa2",			0xEC },
    { L"OEMPA3",			L"OemPa3",			0xED },
    { L"OEMWSCTRL",			L"OemWsCtrl",		0xEE },
    { L"OEMCUSEL",			L"OemCuSel",		0xEF },
    { L"OEMATTN",			L"OemAttn",			0xF0 },
    { L"OEMFINISH",			L"OemFinish",		0xF1 },
    { L"OEMCOPY",			L"OemCopy",			0xF2 },
    { L"OEMAUTO",			L"OemAuto",			0xF3 },
    { L"OEMENLW",			L"OemEnlw",			0xF4 },
    { L"OEMBACKTAB",		L"OemBackTab",		0xF5 },
    { L"ATTN",           	L"Attn",			0xF6 },
    { L"CRSEL",          	L"CrSel",			0xF7 },
    { L"EXSEL",          	L"ExSel",			0xF8 },
    { L"EREOF",          	L"EREOF",			0xF9 },
    { L"PLAY",           	L"Play",			0xFA },
    { L"ZOOM",           	L"Zoom",			0xFB },
    { L"NONAME",			L"NoName",			0xFC },
    { L"PA1",            	L"Pa1",				0xFD },
    { L"OEMCLEAR",			L"OemClear",		0xFE },
	{ L"",					L"",				0xFF },

	// EXTENDED KEYS FOLLOW

	{ L"",					L"",				0x100 + 0x0 },
	{ L"",					L"",				0x100 + 0x1 },
	{ L"",					L"",				0x100 + 0x2 },
	{ L"CANCEL",			L"Cancel",			0x100 + 0x3 },
	{ L"",					L"",				0x100 + 0x4 },
	{ L"",					L"",				0x100 + 0x5 },
	{ L"",					L"",				0x100 + 0x6 },
	{ L"",					L"",				0x100 + 0x7 },
	{ L"",					L"",				0x100 + 0x8 },
	{ L"",					L"",				0x100 + 0x9 },
	{ L"",					L"",				0x100 + 0xA },
	{ L"",					L"",				0x100 + 0xB },
	{ L"",					L"",				0x100 + 0xC },
	{ L"NUMPADENTER",		L"NumpadEnter",		0x100 + 0xD },
	{ L"",					L"",				0x100 + 0xE },
	{ L"",					L"",				0x100 + 0xF },
	{ L"",					L"",				0x100 + 0x10 },
	{ L"",					L"",				0x100 + 0x11 },
	{ L"",					L"",				0x100 + 0x12 },
	{ L"",					L"",				0x100 + 0x13 },
	{ L"",					L"",				0x100 + 0x14 },
	{ L"",					L"",				0x100 + 0x15 },
	{ L"",					L"",				0x100 + 0x16 },
	{ L"",					L"",				0x100 + 0x17 },
	{ L"",					L"",				0x100 + 0x18 },
	{ L"",					L"",				0x100 + 0x19 },
	{ L"",					L"",				0x100 + 0x1A },
	{ L"",					L"",				0x100 + 0x1B },
	{ L"",					L"",				0x100 + 0x1C },
	{ L"",					L"",				0x100 + 0x1D },
	{ L"",					L"",				0x100 + 0x1E },
	{ L"",					L"",				0x100 + 0x1F },
	{ L"",					L"",				0x100 + 0x20 },
	{ L"PGUP",				L"PgUp",			0x100 + 0x21 },
	{ L"PGDN",				L"PgDn",			0x100 + 0x22 },
	{ L"END",				L"End",				0x100 + 0x23 },
	{ L"HOME",				L"Home",			0x100 + 0x24 },
	{ L"LEFT",				L"Left",			0x100 + 0x25 },
	{ L"UP",				L"Up",				0x100 + 0x26 },
	{ L"RIGHT",				L"Right",			0x100 + 0x27 },
	{ L"DOWN",				L"Down",			0x100 + 0x28 },
	{ L"",					L"",				0x100 + 0x29 },
	{ L"",					L"",				0x100 + 0x2A },
	{ L"",					L"",				0x100 + 0x2B },
	{ L"PRINTSCREEN",		L"PrintScreen",		0x100 + 0x2C },
	{ L"INSERT",			L"Insert",			0x100 + 0x2D },
	{ L"DELETE",			L"Delete",			0x100 + 0x2E },
	{ L"",					L"",				0x100 + 0x2F },
	{ L"",					L"",				0x100 + 0x30 },
	{ L"",					L"",				0x100 + 0x31 },
	{ L"",					L"",				0x100 + 0x32 },
	{ L"",					L"",				0x100 + 0x33 },
	{ L"",					L"",				0x100 + 0x34 },
	{ L"",					L"",				0x100 + 0x35 }, 
	{ L"",					L"",				0x100 + 0x36 },
	{ L"",					L"",				0x100 + 0x37 },
	{ L"",					L"",				0x100 + 0x38 },
	{ L"",					L"",				0x100 + 0x39 },
	{ L"",					L"",				0x100 + 0x3A },
	{ L"",					L"",				0x100 + 0x3B },
	{ L"",					L"",				0x100 + 0x3C },
	{ L"",					L"",				0x100 + 0x3D },
	{ L"",					L"",				0x100 + 0x3E },
	{ L"",					L"",				0x100 + 0x3F },
	{ L"",					L"",				0x100 + 0x40 },
	{ L"",					L"",				0x100 + 0x41 },
	{ L"",					L"",				0x100 + 0x42 },
	{ L"",					L"",				0x100 + 0x43 },
	{ L"",					L"",				0x100 + 0x44 },
	{ L"",					L"",				0x100 + 0x45 },
	{ L"",					L"",				0x100 + 0x46 },
	{ L"",					L"",				0x100 + 0x47 },
	{ L"",					L"",				0x100 + 0x48 },
	{ L"",					L"",				0x100 + 0x49 },
	{ L"",					L"",				0x100 + 0x4A },
	{ L"",					L"",				0x100 + 0x4B },
	{ L"",					L"",				0x100 + 0x4C },
	{ L"",					L"",				0x100 + 0x4D },
	{ L"",					L"",				0x100 + 0x4E },
	{ L"",					L"",				0x100 + 0x4F },
	{ L"",					L"",				0x100 + 0x50 },
	{ L"",					L"",				0x100 + 0x51 },
	{ L"",					L"",				0x100 + 0x52 },
	{ L"",					L"",				0x100 + 0x53 },
	{ L"",					L"",				0x100 + 0x54 },
	{ L"",					L"",				0x100 + 0x55 },
	{ L"",					L"",				0x100 + 0x56 },
	{ L"",					L"",				0x100 + 0x57 },
	{ L"",					L"",				0x100 + 0x58 },
	{ L"",					L"",				0x100 + 0x59 },
	{ L"",					L"",				0x100 + 0x5A },
	{ L"LWIN",				L"LWin",			0x100 + 0x5B },
	{ L"RWIN",				L"RWin",			0x100 + 0x5C },
	{ L"APPS",				L"Apps",			0x100 + 0x5D },
	{ L"",					L"",				0x100 + 0x5E },
	{ L"SLEEP",				L"Sleep",			0x100 + 0x5F },
	{ L"",					L"",				0x100 + 0x60 },
	{ L"",					L"",				0x100 + 0x61 },
	{ L"",					L"",				0x100 + 0x62 },
	{ L"",					L"",				0x100 + 0x63 },
	{ L"",					L"",				0x100 + 0x64 },
	{ L"",					L"",				0x100 + 0x65 },
	{ L"",					L"",				0x100 + 0x66 },
	{ L"",					L"",				0x100 + 0x67 },
	{ L"",					L"",				0x100 + 0x68 },
	{ L"",					L"",				0x100 + 0x69 },
	{ L"",					L"",				0x100 + 0x6A },
	{ L"",					L"",				0x100 + 0x6B },
	{ L"",					L"",				0x100 + 0x6C },
	{ L"",					L"",				0x100 + 0x6D },
	{ L"",					L"",				0x100 + 0x6E },
	{ L"DIVIDE",			L"Divide",			0x100 + 0x6F },
	{ L"",					L"",				0x100 + 0x70 },
	{ L"",					L"",				0x100 + 0x71 },
	{ L"",					L"",				0x100 + 0x72 },
	{ L"",					L"",				0x100 + 0x73 },
	{ L"",					L"",				0x100 + 0x74 },
	{ L"",					L"",				0x100 + 0x75 },
	{ L"",					L"",				0x100 + 0x76 },
	{ L"",					L"",				0x100 + 0x77 },
	{ L"",					L"",				0x100 + 0x78 },
	{ L"",					L"",				0x100 + 0x79 },
	{ L"",					L"",				0x100 + 0x7A },
	{ L"",					L"",				0x100 + 0x7B },
	{ L"",					L"",				0x100 + 0x7C },
	{ L"",					L"",				0x100 + 0x7D },
	{ L"",					L"",				0x100 + 0x7E },
	{ L"",					L"",				0x100 + 0x7F },
	{ L"",					L"",				0x100 + 0x80 },
	{ L"",					L"",				0x100 + 0x81 },
	{ L"",					L"",				0x100 + 0x82 },
	{ L"",					L"",				0x100 + 0x83 },
	{ L"",					L"",				0x100 + 0x84 },
	{ L"",					L"",				0x100 + 0x85 },
	{ L"",					L"",				0x100 + 0x86 },
	{ L"",					L"",				0x100 + 0x87 },
	{ L"",					L"",				0x100 + 0x88 },
	{ L"",					L"",				0x100 + 0x89 },
	{ L"",					L"",				0x100 + 0x8A },
	{ L"",					L"",				0x100 + 0x8B },
	{ L"",					L"",				0x100 + 0x8C },
	{ L"",					L"",				0x100 + 0x8D },
	{ L"",					L"",				0x100 + 0x8E },
	{ L"",					L"",				0x100 + 0x8F },
	{ L"NUMLOCK",			L"NumLock",			0x100 + 0x90 },
	{ L"",					L"",				0x100 + 0x91 },
	{ L"",					L"",				0x100 + 0x92 },
	{ L"",					L"",				0x100 + 0x93 },
	{ L"",					L"",				0x100 + 0x94 },
	{ L"",					L"",				0x100 + 0x95 },
	{ L"",					L"",				0x100 + 0x96 },
	{ L"",					L"",				0x100 + 0x97 },
	{ L"",					L"",				0x100 + 0x98 },
	{ L"",					L"",				0x100 + 0x99 },
	{ L"",					L"",				0x100 + 0x9A },
	{ L"",					L"",				0x100 + 0x9B },
	{ L"",					L"",				0x100 + 0x9C },
	{ L"",					L"",				0x100 + 0x9D },
	{ L"",					L"",				0x100 + 0x9E },
	{ L"",					L"",				0x100 + 0x9F },
	{ L"",					L"",				0x100 + 0xA0 },
	{ L"RSHIFT",			L"RShift",			0x100 + 0xA1 }, // my hardware says this is extended, but MS doesn't say so
	{ L"",					L"",				0x100 + 0xA2 },
	{ L"RCTRL",				L"RCtrl",			0x100 + 0xA3 },
	{ L"",					L"",				0x100 + 0xA4 },
	{ L"RALT",				L"RAlt",			0x100 + 0xA5 },
	{ L"BROWSERBACK",		L"BrowserBack",		0x100 + 0xA6 },
	{ L"BROWSERFORWARD",	L"BrowserForward",	0x100 + 0xA7 },
	{ L"BROWSERREFRESH",	L"BrowserRefresh",	0x100 + 0xA8 },
	{ L"BROWSERSTOP",		L"BrowserStop",		0x100 + 0xA9 },
	{ L"BROWSERSEARCH",		L"BrowserSearch",	0x100 + 0xAA },
	{ L"BROWSERFAVORITES",	L"BrowserFavorites", 0x100 + 0xAB },
	{ L"BROWSERHOME",		L"BrowserHome",		0x100 + 0xAC },
    { L"MUTE",				L"Mute",			0x100 +	0xAD }, // ms calls this VOLUME MUTE
	{ L"VOLUMEDOWN",		L"VolumeDown",		0x100 + 0xAE },
	{ L"VOLUMEUP",			L"VolumeUp",		0x100 + 0xAF },
	{ L"MEDIANEXTTRACK",	L"MediaNextTrack",	0x100 + 0xB0 },
	{ L"MEDIAPREVTRACK",	L"MediaPrevTrack",	0x100 + 0xB1 },
	{ L"MEDIASTOP",			L"MediaStop",		0x100 + 0xB2 },
	{ L"MEDIAPLAYPAUSE",	L"MediaPlayPause",	0x100 + 0xB3 },
	{ L"MAIL",				L"Mail",			0x100 + 0xB4 },
	{ L"MEDIASELECT",		L"MediaSelect",		0x100 + 0xB5 },
	{ L"LAUNCHAPP1",		L"LaunchApp1",		0x100 + 0xB6 },
	{ L"LAUNCHAPP2",		L"LaunchApp2",		0x100 + 0xB7 },
	{ L"",					L"",				0x100 + 0xB8 },
	{ L"",					L"",				0x100 + 0xB9 },
	{ L"",					L"",				0x100 + 0xBA },
	{ L"",					L"",				0x100 + 0xBB },
	{ L"",					L"",				0x100 + 0xBC },
	{ L"",					L"",				0x100 + 0xBD },
	{ L"",					L"",				0x100 + 0xBE },
	{ L"",					L"",				0x100 + 0xBF },
	{ L"",					L"",				0x100 + 0xC0 },
	{ L"",					L"",				0x100 + 0xC1 },
	{ L"",					L"",				0x100 + 0xC2 },
	{ L"",					L"",				0x100 + 0xC3 },
	{ L"",					L"",				0x100 + 0xC4 },
	{ L"",					L"",				0x100 + 0xC5 },
	{ L"",					L"",				0x100 + 0xC6 },
	{ L"",					L"",				0x100 + 0xC7 },
	{ L"",					L"",				0x100 + 0xC8 },
	{ L"",					L"",				0x100 + 0xC9 },
	{ L"",					L"",				0x100 + 0xCA },
	{ L"",					L"",				0x100 + 0xCB },
	{ L"",					L"",				0x100 + 0xCC },
	{ L"",					L"",				0x100 + 0xCD },
	{ L"",					L"",				0x100 + 0xCE },
	{ L"",					L"",				0x100 + 0xCF },
	{ L"",					L"",				0x100 + 0xD0 },
	{ L"",					L"",				0x100 + 0xD1 },
	{ L"",					L"",				0x100 + 0xD2 },
	{ L"",					L"",				0x100 + 0xD3 },
	{ L"",					L"",				0x100 + 0xD4 },
	{ L"",					L"",				0x100 + 0xD5 },
	{ L"",					L"",				0x100 + 0xD6 },
	{ L"",					L"",				0x100 + 0xD7 },
	{ L"",					L"",				0x100 + 0xD8 },
	{ L"",					L"",				0x100 + 0xD9 },
	{ L"",					L"",				0x100 + 0xDA },
	{ L"",					L"",				0x100 + 0xDB },
	{ L"",					L"",				0x100 + 0xDC },
	{ L"",					L"",				0x100 + 0xDD },
	{ L"",					L"",				0x100 + 0xDE },
	{ L"",					L"",				0x100 + 0xDF },
	{ L"",					L"",				0x100 + 0xE0 },
	{ L"",					L"",				0x100 + 0xE1 },
	{ L"",					L"",				0x100 + 0xE2 },
	{ L"",					L"",				0x100 + 0xE3 },
	{ L"",					L"",				0x100 + 0xE4 },
	{ L"",					L"",				0x100 + 0xE5 },
	{ L"",					L"",				0x100 + 0xE6 },
	{ L"",					L"",				0x100 + 0xE7 },
	{ L"",					L"",				0x100 + 0xE8 },
	{ L"",					L"",				0x100 + 0xE9 },
	{ L"",					L"",				0x100 + 0xEA },
	{ L"",					L"",				0x100 + 0xEB },
	{ L"",					L"",				0x100 + 0xEC },
	{ L"",					L"",				0x100 + 0xED },
	{ L"",					L"",				0x100 + 0xEE },
	{ L"",					L"",				0x100 + 0xEF },
	{ L"",					L"",				0x100 + 0xF0 },
	{ L"",					L"",				0x100 + 0xF1 },
	{ L"",					L"",				0x100 + 0xF2 },
	{ L"",					L"",				0x100 + 0xF3 },
	{ L"",					L"",				0x100 + 0xF4 },
	{ L"",					L"",				0x100 + 0xF5 },
	{ L"",					L"",				0x100 + 0xF6 },
	{ L"",					L"",				0x100 + 0xF7 },
	{ L"",					L"",				0x100 + 0xF8 },
	{ L"",					L"",				0x100 + 0xF9 },
	{ L"",					L"",				0x100 + 0xFA },
	{ L"",					L"",				0x100 + 0xFB },
	{ L"",					L"",				0x100 + 0xFC },
	{ L"",					L"",				0x100 + 0xFD },
	{ L"",					L"",				0x100 + 0xFE },
	{ L"",					L"",				0x100 + 0xFF },
};

//=======================================================================================================
// CODE
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
//  NAME TO EXTENDED VK
//-------------------------------------------------------------------------------------------------------
WORD cKeyboard :: name_to_ex_vk ( const wchar_t * pName )
{
	const int iTableSize = sizeof(aExVK)/sizeof(sExVK);

	for ( unsigned i = 0; i < iTableSize; i ++ )
	{
		if ( 0 == _wcsicmp ( pName, aExVK[i].pwName ) )
		{
			return aExVK[i].wExVK;
		}
	}

	return 0;
}



//-------------------------------------------------------------------------------------------------------
// EXTENDED VK TO NAME
//-------------------------------------------------------------------------------------------------------
const wchar_t * cKeyboard::ex_vk_to_name ( WORD dwExVK )
{
	return aExVK[dwExVK].pwName;
}


//-------------------------------------------------------------------------------------------------------
// EXTENDED VK TO PRETTY NAME
//-------------------------------------------------------------------------------------------------------
const wchar_t * cKeyboard::ex_vk_to_pretty_name ( WORD dwExVK )
{
	return aExVK[dwExVK].pwPrettyName;
}

//-------------------------------------------------------------------------------------------------------
// PRETTY KEY EVENT
//-------------------------------------------------------------------------------------------------------
const wchar_t * cKeyboard :: pretty_key_event ( mojo::cStrW * pRet, const KBDLLHOOKSTRUCT * pHS )
{
	WORD wExVK = (WORD) pHS->vkCode + (( pHS->flags & LLKHF_EXTENDED ) ? 0x100 : 0 );
	const wchar_t * pwTrans = pHS->flags & LLKHF_UP ? L"Up" : L"Down" ;
	const wchar_t * pwName = mojo :: cKeyboard :: ex_vk_to_pretty_name ( wExVK );
	pRet->f ( L"%s %s", pwName, pwTrans );
	return pRet->cstr();
}


/********************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/********************************************************************************************************/
