/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nobody";

static const char *colorname[NUMCOLS] = {
	[INIT] =   "#1e1e2e",     /* after initialization */
	[INPUT] =  "#f5c2e7",     /* during input */
	[FAILED] = "#f38ba8",     /* wrong password */
	[CAPS] =   "#f5e0dc",     /* caps lock */
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 0;

