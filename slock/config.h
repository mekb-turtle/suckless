/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nobody";

static const char *colorname[NUMCOLS] = {
	[INIT] =   "#000000",     /* after initialization */
	[INPUT] =  "#7F09C4",     /* during input */
	[FAILED] = "#FF0000",     /* wrong password */
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;

/* time in seconds before the monitor shuts down */
static const int monitortime = 2;
