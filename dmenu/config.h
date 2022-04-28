/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

static int topbar = 1;                      /* -b  option; if 0, dmenu appears at bottom     */
/* -fn option overrides fonts[0]; default X11 font or font set */
static const char *fonts[] = {
	"Ubuntu:size=12"
};
static const char *prompt      = NULL;      /* -p  option; prompt to the left of input field */

static const char color[]                     = "#0a0a0f"; // #444444
static const char color_selected[]            = "#7f09c4";
static const char color_border[]              = "#12141f";
static const char color_border_selected[]     = "#8f17d7";
static const char color_panel_text[]          = "#ffaaff";
static const char color_panel_text_selected[] = "#ffcfff";

static const char *colors[SchemeLast][2] = {
	[SchemeNorm] = { color_panel_text,          color },
	[SchemeSel]  = { color_panel_text_selected, color_selected },
	[SchemeOut]  = { color_panel_text,          color },
};

/* -l option; if nonzero, dmenu uses vertical list with given number of lines */
static unsigned int lines      = 0;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";
