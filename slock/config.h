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

/* Background image path, should be available to the user above */
static const char *background_images[NUMCOLS] = {
	[INIT] =   "/usr/share/backgrounds/waves/waves_base.png",      /* after initialization */
	[INPUT] =  "/usr/share/backgrounds/waves/waves_input.png",     /* during input */
	[FAILED] = "/usr/share/backgrounds/waves/waves_incorrect.png", /* wrong password */
	[CAPS] =   "/usr/share/backgrounds/waves/waves_caps.png",      /* caps lock */
};
