/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 10;       /* gap pixel between windows */
static const unsigned int snap      = 0;        /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=11" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=11";
static const char col_gray1[]       = "#282c34";
static const char col_gray2[]       = "#3e4451";
static const char col_gray3[]       = "#abb2bf";
static const char col_gray4[]       = "#c8ccd4";
static const char col_cyan[]        = "#61afef";
static const char *colors[][3]      = {
	/*                   fg         bg         border   */
	[SchemeNorm]     = { col_gray4, col_gray1, col_gray1 },
	[SchemeSel]      = { col_cyan,  col_gray1, col_cyan  },
	[SchemeStatus]   = { "#e5c07b", col_gray1, "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_cyan,  col_gray1, "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm] = { col_gray4, col_gray1, "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { col_cyan,  col_gray1, "#000000" }, // infobar middle selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm] = { col_cyan,  col_gray1, "#000000" }, // infobar middle unselected {text,background,not used but cannot be empty}
};

/* status bar */
static const Block blocks[] = {
	/* fg       command						interval	signal */
	{ "#e06c75", "acpi | head -1 | cut -d',' -f2 | xargs",	        60,		0       },
	{ "#e5c07b", "date +'%d/%m/%Y %H:%M '",	        		1,		0       },
};

/* inverse the order of the blocks, comment to disable */
/* #define INVERSED	1 */
/* delimeter between blocks commands. NULL character ('\0') means no delimeter. */
static char delimiter[] = " ";
/* max number of character that one block command can output */
#define CMDLENGTH	50

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal	noswallow	monitor */
	{ "st",       NULL,       NULL,       0,            0,           1,             0,        	-1 },
	{ "discord",  NULL,       NULL,       1 << 8,       0,		 0,		0,		-1 },
	{ "zoom",     NULL,       NULL,       1 << 7,       0,		 0,		0,		-1 },
	{ NULL,       NULL,       "Event Tester",  0,       0,           0,             1,        	-1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_cyan, "-sb", col_cyan, "-sf", col_gray1, NULL };
static const char *termcmd[]  = { "st", "-e", "fish", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_l,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_BackSpace, zoom,        {0} },
	{ MODKEY,             		XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,	                XK_f,  	   togglefloating, {0} },
	{ MODKEY,	                XK_space,  togglefullscr,  {0} },
	{ MODKEY, 	                XK_Tab,    layoutscroll,   {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    layoutscroll,   {.i = -1 } },
	{ MODKEY,                       XK_bracketright,  viewnext,       {0} },
	{ MODKEY,                       XK_bracketleft,   viewprev,       {0} },
	{ MODKEY|ShiftMask,             XK_bracketright,  tagtonext,      {0} },
	{ MODKEY|ShiftMask,             XK_bracketleft,   tagtoprev,      {0} },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_q, 	   spawn,          SHCMD("~/.local/bin/power.sh") },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask, 		XK_r,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },

	{ ClkStatusText,        0,              Button1,        sendstatusbar,   {.i = 1 } },
	{ ClkStatusText,        0,              Button2,        sendstatusbar,   {.i = 2 } },
	{ ClkStatusText,        0,              Button3,        sendstatusbar,   {.i = 3 } },
	{ ClkStatusText,        0,              Button4,        sendstatusbar,   {.i = 4 } },
	{ ClkStatusText,        0,              Button5,        sendstatusbar,   {.i = 5 } },
	{ ClkStatusText,        ShiftMask,      Button1,        sendstatusbar,   {.i = 6 } },

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
