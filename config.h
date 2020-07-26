/* See LICENSE file for copyright and license details. */
#include<X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode Nerd Font Mono:size=12" };
static const char dmenufont[]       = "FiraCode Nerd Font Mono:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "X" };
static const int EXTRA_TAG = 4;


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class             instance           title       tags mask          isfloating   monitor */
	{ "keepassxc",       "keepassxc",       NULL,       1 << EXTRA_TAG,    1,           -1 },
	{ "Pavucontrol",     "pavucontrol",     NULL,       1 << EXTRA_TAG,    1,           -1 },
	{ "Blueman-manager", "blueman-manager", NULL,       1 << EXTRA_TAG,    1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *brightnessupcmd[] = { "sudo", "systemcontrol", "brightness", "+100", "/sys/class/backlight/intel_backlight", NULL};
static const char *brightnessdowncmd[] = { "sudo", "systemcontrol", "brightness", "-100", "/sys/class/backlight/intel_backlight", NULL};
static const char *keysbacklightup[] = { "sudo", "systemcontrol", "brightness", "+1", "/sys/class/leds/tpacpi::kbd_backlight", NULL};
static const char *keysbacklightdown[] = { "sudo", "systemcontrol", "brightness", "-1", "/sys/class/leds/tpacpi::kbd_backlight", NULL};

static const char *volumeupcmd[] = { "amixer", "-D", "pulse", "-M", "sset", "Master", "10%+", "unmute", NULL};
static const char *volumedowncmd[] = { "amixer", "-D", "pulse", "-M", "sset", "Master", "10%-", "unmute", NULL};
static const char *volumemutecmd[] = { "amixer", "-D", "pulse", "-M", "sset", "Master", "mute", NULL};
static const char *suspend[] = { "sudo", "systemctl", "suspend-then-hibernate", NULL};
static const char *lock[] = { "xautolock", "-locknow", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_space,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_f,      fullscreen,       {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = suspend } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lock } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // tiled
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[1]} }, // floating
	{ MODKEY,                       XK_x,      toggleview,     {.ui = 1 << EXTRA_TAG } },
	{ MODKEY|ShiftMask,             XK_x,      tag,            {.ui = 1 << EXTRA_TAG } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

    // Media keys support
	{ 0,                       XF86XK_KbdBrightnessUp,   spawn,      {.v = keysbacklightup} },
	{ 0,                       XF86XK_KbdBrightnessDown, spawn,      {.v = keysbacklightdown} },
	{ 0,                       XF86XK_MonBrightnessUp,   spawn,      {.v = brightnessupcmd} },
	{ 0,                       XF86XK_MonBrightnessDown, spawn,      {.v = brightnessdowncmd} },
	{ 0,                       XF86XK_AudioLowerVolume,  spawn,      {.v = volumedowncmd} },
	{ 0,                       XF86XK_AudioRaiseVolume,  spawn,      {.v = volumeupcmd} },
	{ 0,                       XF86XK_AudioMute,         spawn,      {.v = volumemutecmd} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

