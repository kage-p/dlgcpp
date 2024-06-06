//________________________________________________________________________________________
//
//  Slider Bar Custom Control
//  -------------------------
//
//  Control to imitate a slider (ie. similar to volume control. etc)
//  Supports colors, fonts, text/percentage display, mouse and keyboard control
//
//  Note: Define SLDR_DLL to export all specified functions.
//        Define SLDR_DEBUG to compile in debug mode (verbose console output).
//
//  TODO: Allow a "focus" border color (special message)
//        SLS_LOCKON for "focus > change" instead of "focus and change" (with mouse/keys)
//        Use WM_NOTIFY and notifications instead of WM_VSCROLL/WM_HSCROLL
//        Alphablend bar color on XP+.
//
//  (C) Copyright KGP Software, 2004-2017.
//  Updated: Apr 2004. Jan 2008.
//________________________________________________________________________________________

#pragma once

#define SLDR_CLASS "SliderControl32"            // Control class name

#define SLM_FIRST          WM_USER+400
#define SLM_SETRANGE       SLM_FIRST+0          // wParam is min value (typecasted) / lParam is max value
#define SLM_GETRANGE       SLM_FIRST+1          // If wParam is nonzero then return min range, else return max range.
#define SLM_SETPOS         SLM_FIRST+2          // Set position. wParam is position. lParam is notify message
#define SLM_GETPOS         SLM_FIRST+3          // Return current position.
#define SLM_SETDIRECTION   SLM_FIRST+4          // [TODO REMOVE] Sets direction, lParam is one of the SLS_ constants
#define SLM_SETLOCK        SLM_FIRST+5          // Lock the control if lParam is nonzero (don//t allow user interaction)
#define SLM_SETBKCOLOR     SLM_FIRST+6          // lParam used to set the back color (typecasted)
#define SLM_SETTEXTCOLOR   SLM_FIRST+7          // lParam used to set the text color (typecasted)
#define SLM_SETBARCOLOR    SLM_FIRST+8          // lParam used to set the bar color (typecasted)
#define SLM_USEPERCENT     SLM_FIRST+9          // lParam nonzero to display percentage value instead of text
#define SLM_SETIMAGE       SLM_FIRST+10         // wParam unused. lParam background image handle. Returns old image, if any.
#define SLM_GETIMAGE       SLM_FIRST+11         // Returns background image handle, if any.

                                                // Styles...
#define SLS_HORIZONTAL 0                        // TODO: DEFINE AS STYLE
#define SLS_VERTICAL 1                          // TODO: DEFINE AS STYLE
//#define SLS_LOCKON 0x0000004                  // Require focus before change is made.

int sldRegister();
int sldUnregister();
