/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;         /* border pixel of windows */
static const unsigned int gapinitpx = 10;        /* gap pixel between windows */
static const unsigned int gappx     = gapinitpx; /* gap pixel between windows */
static const unsigned int snap      = 24;        /* snap pixel */
static const unsigned int systraypinning = 1;    /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;   	 /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 4;    /* systray spacing */
static const int systraypinningfailfirst = 1;    /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;         /* 0 means no systray */
static const int showbar            = 1;         /* 0 means no bar */
static const int topbar             = 1;         /* 0 means bottom bar */
static const char *fonts[]          = { "Ubuntu:size=12", "Twemoji:size=12" };
static const char dmenufont[]       = "Ubuntu:size=12";

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *alttags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "1", "2", "3", "4" };
static const char *alttags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance    title       tags mask     isfloating   monitor */
//	{ "Gimp",       NULL,       NULL,       0,            1,           -1 },
	{ "Librewolf",  NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "tile",       tile },    /* first entry is default */
	{ "float",      NULL },    /* no layout function means floating behavior */
	{ "grid",       grid },
	{ "monocle",    monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define SUPERKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ SUPERKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ SUPERKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ SUPERKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ SUPERKEY|ShiftMask,             KEY,      view,           {.ui = 1 << TAG} }, \
	{ SUPERKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static const char color[]                     = "#0a0a0f"; // #444444
static const char color_selected[]            = "#7f09c4";
static const char color_border[]              = "#12141f";
static const char color_border_selected[]     = "#8f17d7";
static const char color_panel_text[]          = "#ffaaff";
static const char color_panel_text_selected[] = "#ffcfff";
static const char *colors[][3]      = {
	/*               fg                         bg              border   */
	[SchemeNorm] = { color_panel_text,          color,          color_border },
	[SchemeSel]  = { color_panel_text_selected, color_selected, color_border_selected },
};
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "/home/mekb/.local/bin/dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", color, "-nf", color_panel_text, "-sb", color_selected, "-sf", color_panel_text_selected, NULL };

static const char *const autostart[] = {
	"dunst", NULL,
	"kitty", NULL,
	"picom", NULL,
	"hsetroot", "-solid", "#0a0a0f", NULL,
	"caffeine", NULL,
	"volumeicon", NULL,
	"flameshot", NULL,
	"nm-applet", NULL,
	"xautolock", "-time", "1", "-locker", "slock", NULL,
	NULL
};

static const char *decBri[] = { "xbacklight", "-dec", "15", NULL };
static const char *incBri[] = { "xbacklight", "-inc", "15", NULL };
static const char *pavu[] = { "pavucontrol", NULL };

static Key keys[] = {
	/* modifier                     key             function             argument */
	// numpad volume/brightness
	{ SUPERKEY,                     XK_KP_Next,     spawn, {.v = decBri } },
	{ SUPERKEY,                     XK_KP_Prior,    spawn, {.v = incBri } },
	{ SUPERKEY,                     XK_KP_Right,    spawn, {.v = pavu } },
	// dmenu run
	{ SUPERKEY,                     XK_r,           spawn,           {.v = dmenucmd } },
	// clear clipboard
	{ SUPERKEY,                     XK_space,       spawn,           {.v = (const char*[]) { "clip", "clear", NULL }} },
	// notifications
	{ ControlMask,                  XK_Pause,       spawn,           {.v = (const char*[]) { "dunstctl", "set-paused", "toggle", NULL }} },
	{ SUPERKEY,                     XK_q,           spawn,           {.v = (const char*[]) { "dunstctl", "history-pop", NULL }} },
	{ SUPERKEY|ShiftMask,           XK_q,           spawn,           {.v = (const char*[]) { "dunstctl", "close-all", NULL }} },
	// applications
	{ SUPERKEY,                     XK_t,           spawn,           {.v = (const char*[]) { "kitty", NULL }} },
	{ SUPERKEY,                     XK_y,           spawn,           {.v = (const char*[]) { "librewolf", NULL }} },
	// lock
	{ SUPERKEY,                     XK_l,           spawn,           {.v = (const char*[]) { "slock", NULL }} },
	// screenshot
	{ 0,                            XK_Print,       spawn,           {.v = (const char*[]) { "flameshot", "gui", NULL }} },
	{ ShiftMask,                    XK_Print,       spawn,           {.v = (const char*[]) { "flameshot", "launcher", NULL }} },
	{ ControlMask,                  XK_Print,       spawn,           {.v = (const char*[]) { "bash", "-c", "$HOME/.bin/misc/flameshot_window", NULL }} },
	{ MODKEY,                       XK_Print,       spawn,           {.v = (const char*[]) { "bash", "-c", "$HOME/.bin/misc/colorpickernotify", NULL }} },
	// shutdown gui
	{ MODKEY|ControlMask,           XK_Delete,      spawn,           {.v = (const char*[]) { "bash", "-c", "python $HOME/.bin/misc/shutdownprompt/shutdownprompt.py", pidstring, NULL }} },
	// kill client
	{ MODKEY|ControlMask,           XK_End,         killclient,      {0} },
	// tags
	TAGKEYS(                        XK_1,                            0),
	TAGKEYS(                        XK_2,                            1),
	TAGKEYS(                        XK_3,                            2),
	TAGKEYS(                        XK_4,                            3),
//	TAGKEYS(                        XK_5,                            4),
//	TAGKEYS(                        XK_6,                            5),
//	TAGKEYS(                        XK_7,                            6),
//	TAGKEYS(                        XK_8,                            7),
//	TAGKEYS(                        XK_9,                            8),
	// toggle bar
	{ SUPERKEY,                     XK_b,           togglebar,       {0} },
	// arrange window
	{ SUPERKEY,                     XK_Up,          zoom,            {0} },
	{ SUPERKEY,                     XK_Down,        togglefloating,  {0} },
	// cycle window
	{ MODKEY,                       XK_Tab,         focusstack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,         focusstack,      {.i = -1 } },
	// rearrange stack
	{ SUPERKEY|ShiftMask,           XK_Left,        incnmaster,      {.i = +1 } },
	{ SUPERKEY|ShiftMask,           XK_Right,       incnmaster,      {.i = -1 } },
	// move divider
	{ SUPERKEY,                     XK_Left,        setmfact,        {.f = -0.025} },
	{ SUPERKEY,                     XK_Right,       setmfact,        {.f = +0.025} },
	{ SUPERKEY|ControlMask,         XK_Left,        setmfact,        {.f = -0.001} },
	{ SUPERKEY|ControlMask,         XK_Right,       setmfact,        {.f = +0.001} },
	// switch layout
	{ MODKEY|ControlMask,           XK_space,       setlayout,       {0} },
	{ MODKEY|ControlMask,           XK_1,           setlayout,       {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_2,           setlayout,       {.v = &layouts[1]} },
	{ MODKEY|ControlMask,           XK_3,           setlayout,       {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_4,           setlayout,       {.v = &layouts[3]} },
	// mon
	{ SUPERKEY,                     XK_comma,       focusmon,        {.i = -1 } },
	{ SUPERKEY,                     XK_period,      focusmon,        {.i = +1 } },
	{ SUPERKEY|ShiftMask,           XK_comma,       tagmon,          {.i = -1 } },
	{ SUPERKEY|ShiftMask,           XK_period,      tagmon,          {.i = +1 } },
	/*
	// gaps
	{ SUPERKEY|ShiftMask,           XK_minus,       setgaps,         {.i = -1 } },
	{ SUPERKEY|ShiftMask,           XK_equal,       setgaps,         {.i = +1 } },
	{ SUPERKEY|ShiftMask,           XK_0,           setgaps,         {.i = 0  } },
	*/
	/*
	{ SUPERKEY,                     XK_Down,        toggleview,      {0} },
	{ MODKEY,                       XK_Tab,         view,            {0} },
	{ MODKEY,                       XK_0,           view,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,           tag,             {.ui = ~0 } },
	*/
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask                    button          function        argument */
	// click layout in panel to toggle previous
	{ ClkLtSymbol,          0,                            Button1,        setlayout,      {0} },
	// move window
	{ ClkClientWin,         SUPERKEY,                     Button1,        movemouse,      {0} },
	// resize window
	{ ClkClientWin,         SUPERKEY,                     Button3,        resizemouse,    {0} },
	// toggle floating
	{ ClkClientWin,         SUPERKEY|ControlMask,         Button1,        togglefloating, {0} },
	{ ClkClientWin,         SUPERKEY,                     Button2,        togglefloating, {0} },
	// kill window
	{ ClkClientWin,         MODKEY|ControlMask,           Button3,        killclient,     {0} },
	// tags
	{ ClkTagBar,            0,                            Button1,        view,           {0} },
	{ ClkTagBar,            ControlMask,                  Button1,        toggleview,     {0} },
	{ ClkTagBar,            ShiftMask,                    Button1,        tag,            {0} },
	{ ClkTagBar,            ShiftMask,                    Button1,        view,           {0} },
	{ ClkTagBar,            ControlMask|ShiftMask,        Button1,        toggletag,      {0} },
	// cycle window
	{ ClkWinTitle,          0,                            Button1,        focusstack,     {.i = +1} },
	{ ClkWinTitle,          ShiftMask,                    Button1,        focusstack,     {.i = -1} },
	// switch window
	{ ClkWinTitle,          ControlMask,                  Button1,        zoom,           {0} },
	/*
	{ ClkLtSymbol,          0,                            Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,                            Button2,        spawn,          {.v = termcmd } },
	*/
};
