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
static const int fixedstextw = 550;   // make status text fixed width so systray doesn't keep moving
static const int fixedalignright = 1; // align status text on 0 = left, 1 = right, doesn't affect systray
static const int systraypinningfailfirst = 1;    /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;         /* 0 means no systray */
static const int showbar            = 1;         /* 0 means no bar */
static const int topbar             = 1;         /* 0 means bottom bar */
static const char *fonts[]          = { "Ubuntu:size=11" };
static const char dmenufont[]       = "Ubuntu:size=11";

/* tagging */
static char *tags[]    = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static char *alttags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char alturg = 0; // whether to use alt tag if 1 = an urgent window is on that tag, 0 = urgent or non-urgent window is on that tag

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance    title       tags mask                isfloating   monitor */
//	{ "Gimp",          NULL,       NULL,       0,                       1,           -1 },
	{ "Librewolf",     NULL,       NULL,       0,                       0,           -1 },
	{ "Ripcord",       NULL,       NULL,       1 << (LENGTH(tags)-1),   0,           1 },
	{ "Motrix",        NULL,       NULL,       1 << (LENGTH(tags)-2),   0,           1 },
	{ "PrismLauncher", NULL,       NULL,       1 << (LENGTH(tags)-3),   0,           1 },
	{ "Tk",            "Logout",   NULL,       TAGMASK,                 1,           0 },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", color, "-nf", color_panel_text, "-sb", color_selected, "-sf", color_panel_text_selected, NULL };

static const char *const autostart[] = {
	"dunst", NULL,
	"kitty", NULL,
	"picom", NULL,
	"bash", "-c", "exec hsetroot -solid '#0a0a0f' -fill $HOME/.wallpaper", NULL,
	//"caffeine", NULL,
	//"volumeicon", "-d", "default", NULL,
	"pnmixer", NULL,
	"flameshot", NULL,
	"nm-applet", NULL,
	"xautolock", "-time", "1", "-locker", "slock", NULL,
	"slstatus", NULL,
	//"slock", NULL, // uncomment if you want to lock on startup if you have autologin
	"numlockx", NULL,
	NULL
};

static const char *decBri[]  = { "xbacklight", "-dec", "15", NULL };
static const char *incBri[]  = { "xbacklight", "-inc", "15", NULL };
static const char *decVol[]  = { "pactl", "set-sink-volume", "0", "-5%",    NULL };
static const char *incVol[]  = { "pactl", "set-sink-volume", "0", "+5%",    NULL };
static const char *muteVol[] = { "pactl", "set-sink-mute",   "0", "toggle", NULL };
static const char *pavu[] = { "pavucontrol", NULL };

static Key keys[] = {
	/* how to get XK_ codes
	 * $ script -qefc xev /dev/null|grep "keysym 0x" --color=never
	 * find the part after keysym in brackets
	 * e.g
	 * state 0x10, keycode 36 (keysym 0xff0d, Return), same_screen YES,
	 * Return, then prefix with XK_, so XK_Return */
	/* modifier                     key             function             argument */
	// numpad volume/brightness
	{ SUPERKEY,                     XK_KP_Next,     spawn, {.v = decBri } },
	{ SUPERKEY,                     XK_KP_Prior,    spawn, {.v = incBri } },
	{ SUPERKEY,                     XK_KP_Down,     spawn, {.v = decVol } },
	{ SUPERKEY,                     XK_KP_Up,       spawn, {.v = incVol } },
	{ SUPERKEY,                     XK_KP_Begin,    spawn, {.v = muteVol } },
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
	{ SUPERKEY|ShiftMask,           XK_y,           spawn,           {.v = (const char*[]) { "tabbed", "-f", "surf", "-e", NULL }} },
	{ SUPERKEY,                     XK_k,           spawn,           {.v = (const char*[]) { "keepassxc", NULL }} },
	{ SUPERKEY,                     XK_m,           spawn,           {.v = (const char*[]) { "virt-manager", NULL }} },
	// lock
	{ SUPERKEY,                     XK_l,           spawn,           {.v = (const char*[]) { "slock", NULL }} },
	// screenshot
	{ SUPERKEY,                     XK_Print,       spawn,           {.v = (const char*[]) { "bash", "-c", "exec $HOME/.bin/screenshot", NULL }} }, // https://gist.github.com/mekb-turtle/169b6fb03627bdff40bbddfe9e1de70b
	{ 0,                            XK_Print,       spawn,           {.v = (const char*[]) { "flameshot", "gui", NULL }} },
	{ ShiftMask,                    XK_Print,       spawn,           {.v = (const char*[]) { "flameshot", "launcher", NULL }} },
	{ ControlMask,                  XK_Print,       spawn,           {.v = (const char*[]) { "bash", "-c", "exec $HOME/.bin/misc/flameshot_window", NULL }} }, // https://gist.github.com/mekb-turtle/288af4251b43cfe2becf06590da7f1a2
	{ MODKEY,                       XK_Print,       spawn,           {.v = (const char*[]) { "bash", "-c", "exec $HOME/.bin/misc/colorpickernotify", NULL }} },
	// shutdown gui
	{ MODKEY|ControlMask,           XK_Delete,      spawn,           {.v = (const char*[]) { "bash", "-c", "exec python $HOME/.bin/misc/shutdownprompt/shutdownprompt.py \"$0\"", pidstring, NULL }} }, // https://github.com/mekb-turtle/shutdownprompt
	// kill client
	{ MODKEY|ControlMask,           XK_End,         killclient,      {0} },
	// tags
	{ SUPERKEY,                     XK_Tab,         cycletags,       {.i = +1} },
	{ SUPERKEY|ShiftMask,           XK_Tab,         cycletags,       {.i = -1} },
	TAGKEYS(                        XK_1,                            0),
	TAGKEYS(                        XK_2,                            1),
	TAGKEYS(                        XK_3,                            2),
	TAGKEYS(                        XK_4,                            3),
	TAGKEYS(                        XK_5,                            4),
	TAGKEYS(                        XK_6,                            5),
	TAGKEYS(                        XK_7,                            6),
	TAGKEYS(                        XK_8,                            7),
	TAGKEYS(                        XK_9,                            8),
	// toggle bar
	{ SUPERKEY,                     XK_b,           togglebar,       {0} },
	// arrange window
	{ SUPERKEY,                     XK_Up,          zoom,            {0} },
	{ SUPERKEY,                     XK_Down,        togglefloating,  {0} },
	// cycle window
	{ MODKEY,                       XK_Tab,         focusstack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,         focusstack,      {.i = -1 } },
	// rearrange stack
	{ SUPERKEY|ShiftMask,           XK_Left,        incnmaster,      {.i = -1 } },
	{ SUPERKEY|ShiftMask,           XK_Right,       incnmaster,      {.i = +1 } },
	// move divider
	{ SUPERKEY,                     XK_Left,        setmfact,        {.f = -0.025} },
	{ SUPERKEY,                     XK_Right,       setmfact,        {.f = +0.025} },
	{ SUPERKEY|ControlMask,         XK_Left,        setmfact,        {.f = -0.0025} },
	{ SUPERKEY|ControlMask,         XK_Right,       setmfact,        {.f = +0.0025} },
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
	{ SUPERKEY|ShiftMask,           XK_comma,       focusmon,        {.i = -1 } },
	{ SUPERKEY|ShiftMask,           XK_period,      focusmon,        {.i = +1 } },
	{ MODKEY|ControlMask,           XK_Tab,         focusmon,        {.i = +1 } },
	{ MODKEY|ShiftMask|ControlMask, XK_Tab,         focusmon,        {.i = -1 } },
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

