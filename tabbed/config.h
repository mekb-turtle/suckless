/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]        = "monospace:size=9";
static const char* normbgcolor  = "#0a0a0f";
static const char* normfgcolor  = "#ffaaff";
static const char* selbgcolor   = "#7f09c4";
static const char* selfgcolor   = "#ffdfff";
static const char* urgbgcolor   = "#ff09c4";
static const char* urgfgcolor   = "#ffffff";
static const char before[]      = "«";
static const char after[]       = "»";
static const char titletrim[]   = "...";
static const int  tabwidth      = 200;
static const Bool foreground    = True;
static       Bool urgentswitch  = False;

/*
 * Where to place a new tab when it is opened. When npisrelative is True,
 * then the current position is changed + newposition. If npisrelative
 * is False, then newposition is an absolute position.
 */
static int  newposition   = 1;
static Bool npisrelative  = True;

#define SETPROP(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
                "prop=\"`xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1 \\2@' |" \
                "xargs -0 printf %b | dmenu -l 10 -w $1`\" &&" \
                "xprop -id $1 -f $0 8s -set $0 \"$prop\"", \
                p, winid, NULL \
        } \
}

#define MODKEY0 ControlMask
#define MODKEY1 MODKEY0|ShiftMask
static Key keys[] = {
	/* modifier             key        function     argument */
	{ MODKEY1,              XK_Return, spawn,       { 0 } },
	{ MODKEY1,              XK_t,      spawn,       { 0 } },

	{ MODKEY1,              XK_w,      killclient,  { 0 } },

	{ MODKEY1,              XK_Right,  rotate,      { .i = +1 } },
	{ MODKEY1,              XK_Left,   rotate,      { .i = -1 } },
	{ MODKEY1,              XK_period, movetab,     { .i = +1 } },
	{ MODKEY1,              XK_comma,  movetab,     { .i = -1 } },
	{ MODKEY0,              XK_Tab,    rotate,      { .i = +1 } },
	{ MODKEY1,              XK_Tab,    rotate,      { .i = -1 } },

	{ MODKEY0,              XK_grave,  spawn,       SETPROP("_TABBED_SELECT_TAB") },
	{ MODKEY1,              XK_1,      move,        { .i = 0 } },
	{ MODKEY1,              XK_2,      move,        { .i = 1 } },
	{ MODKEY1,              XK_3,      move,        { .i = 2 } },
	{ MODKEY1,              XK_4,      move,        { .i = 3 } },
	{ MODKEY1,              XK_5,      move,        { .i = 4 } },
	{ MODKEY1,              XK_6,      move,        { .i = 5 } },
	{ MODKEY1,              XK_7,      move,        { .i = 6 } },
	{ MODKEY1,              XK_8,      move,        { .i = 7 } },
	{ MODKEY1,              XK_9,      move,        { .i = 8 } },
	{ MODKEY1,              XK_0,      move,        { .i = 9 } },

//	{ MODKEY0,              XK_u,      focusurgent, { 0 } },
//	{ MODKEY1,              XK_u,      toggle,      { .v = (void*) &urgentswitch } },

	{ 0,                    XK_F11,    fullscreen,  { 0 } },
};
