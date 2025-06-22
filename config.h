/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 1;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 10; /* gap pixel between windows */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
#define GCOL(a,b,c) {0x ##a / 255.0f, 0x ##b / 255.0f, 0x ##c / 255.0f, 1.0f }
static const float rootcolor[]             = GCOL(22, 22, 22);
static const float bordercolor[]           = GCOL(42, 43, 45);
static const float focuscolor[]            = GCOL(B9, B8, BA);
static const float urgentcolor[]           = GCOL(FF, 00, 00);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */

/* keyboard layout change notification for status bar */
//static const char  kblayout_file[] = "/tmp/dwl-keymap";
//static const char *kblayout_cmd[]  = {"pkill", "-RTMIN+3", "someblocks", NULL};

static const char *cursor_theme            = NULL;
static const char cursor_size[]            = "24"; /* Make sure it's a valid integer, otherwise things will break */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* passthrough */
static int passthrough = 0;

/* Autostart */
static const char *const autostart[] = {
        "/home/raiku/.dwlstart", NULL,
        NULL /* terminate */
};


/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	/* examples: */
	// { "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      centeredmaster },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect              x  y  resx resy rate mode adaptive*/
	/* example of a HiDPI laptop monitor:
	//{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },

	*/

	{ "eDP-1",    0.5f,  1,      1.33f,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 0, 0,  -1,  -1, 240.0f, 1, 0},
  { "HDMI-A-1", 0.5f,  1,      1.00f,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 0, 0,  4096, 2160, 60.0f, 1, 0},
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = "caps:swapescape",
};

static const int repeat_rate = 60;
static const int repeat_delay = 175;

/* Trackpad */
static const int tap_to_click = 0;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 0;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_ALT,          KEY,            focusnthmon,     {.ui = TAG} }, \
	{ MODKEY|WLR_MODIFIER_ALT|WLR_MODIFIER_SHIFT, SKEY, tagnthmon,       {.ui = TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[] = { "foot", NULL };

static const char *spotifycmd[] = { "spotif", NULL };
static const char *discordcmd[] = { "vesktop", NULL };
static const char *qutecmd[]    = { "qutebrowser", NULL };
static const char *gimpcmd[]    = { "gimp", NULL };
static const char *scrn[]   = { "scrn", "0", NULL };
static const char *scrncp[]    = { "scrn", "1", NULL };
static const char *scrngm[]    = { "scrn", "2", NULL };

static const char *volUP[]       = { "chvol", "+", NULL };
static const char *volDOWN[]     = { "chvol", "-", NULL };
static const char *volUPUP[]     = { "chvol", "++", NULL };
static const char *volDOWNDOWN[] = { "chvol", "--", NULL };

static const char *brtUP[]      = { "brightnessctl", "s", "+30%", NULL };
static const char *brtDOWN[]    = { "brightnessctl", "s", "30%-", NULL };
static const char *brtsUP[]     = { "brightnessctl", "s", "+1%", NULL };
static const char *brtsDOWN[]   = { "brightnessctl", "s", "1%-", NULL };

static const char *chadcmd[]  = { "mpv", "/home/raiku/Misc/Downloads/chad-1.mp4", "--loop", NULL };
static const char *reliefcmd[]  = { "mpv", "/home/raiku/Misc/Downloads/Huge_Ship_Shaft_Forging_and_Machining_Process.webm", "--loop", NULL };
static const char *yajucmd[]  = { "mpv", "/home/raiku/Misc/Downloads/yaju-1.mp4", NULL };
static const char *walcccmd[]  = { "mpv", "/home/raiku/Misc/Downloads/Computer_chip_walking_to_Stayin_Alive.webm", "--loop", "--fullscreen", NULL };
static const char *gamblecore[]  = { "mpv", "/home/raiku/Misc/Downloads/Gamblecore.webm", NULL };
static const char *necu[]  = { "mpv", "/home/raiku/Misc/Downloads/neco-arc.mp4", "--loop", NULL };
static const char *neoc[]  = { "mpv", "/home/raiku/Misc/Downloads/neco.gif", "--loop", NULL };
static const char *think[]  = { "mpv", "/home/raiku/Misc/Downloads/Wise_Mystical_Tree.mkv", NULL };
static const char *sgate[]  = { "mpv", "/home/raiku/Misc/Downloads/SteinsGate-Farewell.webm", NULL };
static const char *dream[]  = { "mpv", "/home/raiku/Misc/Downloads/1HourDreamSpeedRunMusic.webm", NULL };
static const char *fumen[]  = { "mpv", "/home/raiku/Misc/Downloads/FUckYouMen.mp4", NULL };
static const char *moyai[]  = { "sh", "-c", "mpv /home/raiku/Misc/Downloads/moyai.mp4 --loop & mpv --volume=150 /home/raiku/Misc/Downloads/Vineboomsoundeffect.m4a", NULL };
static const char *dufen[]  = { "swaybg", "-i", "/home/raiku/Misc/Downloads/Dufensmirz.jpg", NULL };
static const char *biaoqing[]  = { "biaoqing", NULL };
static const char *cacafire[]  = { "cacafire", NULL };
static const char *clipcell[]  = { "clipcel", NULL };
static const char *necc[]  = { "nec", NULL };
static const char *necck[]  = { "killall", "neco", NULL };
static const char *brainrot[]  = { "brainrot", NULL };
static const char *parkour[]  = { "brainrot", "/home/raiku/Misc/Downloads/1HourMinecraftParkour.webm", "/home/raiku/Misc/Downloads/2HourMinecraftParkour.webm", NULL };
static const char *woomer[]  = { "woomer", NULL };

static const char *r2kcmd[]     = { "r2k", NULL };

#define ShiftMask (1<<0)
#define ControlMask (1<<2)
static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                     key                   function          argument */
	{ MODKEY|ShiftMask,             XKB_KEY_Return,       spawn,            {.v = termcmd } },
  { MODKEY|ShiftMask,             XKB_KEY_S,            spawn,            {.v = spotifycmd } },
 	{ MODKEY|ShiftMask,             XKB_KEY_D,            spawn,            {.v = discordcmd } },
	{ MODKEY|ShiftMask,             XKB_KEY_P,            spawn,            {.v = qutecmd } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_P,            spawn,            {.v = parkour } },
	{ MODKEY|ShiftMask,             XKB_KEY_O,            spawn,            {.v = qutecmd } },
	{ MODKEY|ShiftMask,             XKB_KEY_G,            spawn,            {.v = gimpcmd } },
	{ MODKEY|ShiftMask,             XKB_KEY_J,            spawn,            {.v = chadcmd } },
	{ MODKEY|ShiftMask,             XKB_KEY_K,            spawn,            {.v = reliefcmd } },
	{ MODKEY|ShiftMask,             XKB_KEY_L,            spawn,            {.v = yajucmd } },
	{ MODKEY|ShiftMask,             XKB_KEY_B,            spawn,            {.v = biaoqing } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_B,            spawn,            {.v = brainrot } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_L,            spawn,            {.v = cacafire } },
	{ MODKEY|ShiftMask,             XKB_KEY_H,            spawn,            {.v = walcccmd } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_G,            spawn,            {.v = gamblecore } },
	{ MODKEY|ShiftMask,             XKB_KEY_N,            spawn,            {.v = necu } },
	{ MODKEY|ShiftMask,             XKB_KEY_U,            spawn,            {.v = neoc } },
	{ MODKEY|ShiftMask,             XKB_KEY_T,            spawn,            {.v = think } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_S,            spawn,            {.v = sgate } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_T,            spawn,            {.v = dream } },
	{ MODKEY|ShiftMask,             XKB_KEY_M,            spawn,            {.v = moyai } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_E,            spawn,            {.v = dufen } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_F,            spawn,            {.v = fumen } },
	{ MODKEY|ControlMask,           XKB_KEY_x,            spawn,            {.v = brtDOWN } },
	{ MODKEY|ControlMask,           XKB_KEY_c,            spawn,            {.v = brtUP } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_X,            spawn,            {.v = brtsDOWN } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_C,            spawn,            {.v = brtsUP } },
	{ MODKEY|ControlMask,           XKB_KEY_s,            spawn,            {.v = volDOWN } },
	{ MODKEY|ControlMask,           XKB_KEY_d,            spawn,            {.v = volUP } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_S,            spawn,            {.v = volDOWNDOWN } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_D,            spawn,            {.v = volUPUP } },
	{ MODKEY,                       XKB_KEY_o,            spawn,            {.v = clipcell } },
	{ MODKEY|ShiftMask,             XKB_KEY_W,            spawn,            {.v = woomer } },
	{ MODKEY,                       XKB_KEY_n,            spawn,            {.v = necc } },
	{ MODKEY|ControlMask|ShiftMask, XKB_KEY_N,            spawn,            {.v = necck } },
	{ MODKEY,                       XKB_KEY_p,            spawn,            {.v = dmenucmd } },
	{ MODKEY,                       XKB_KEY_i,            spawn,            {.v = r2kcmd } },
  { 0,                            XKB_KEY_Print,        spawn,            {.v = scrn } },
  { MODKEY,                       XKB_KEY_Print,        spawn,            {.v = scrncp } },
  { MODKEY|ShiftMask,             XKB_KEY_Print,        spawn,            {.v = scrngm } },
	{ MODKEY,                       XKB_KEY_j,            focusstack,       {.i = +1} },
	{ MODKEY,                       XKB_KEY_k,            focusstack,       {.i = -1} },
	//{ MODKEY|ShiftMask,             XKB_KEY_I,            incnmaster,       {.i = +1} },
	//{ MODKEY|ShiftMask,             XKB_KEY_D,            incnmaster,       {.i = -1} },
	{ MODKEY,                       XKB_KEY_h,            setmfact,         {.f = -0.05f} },
	{ MODKEY,                       XKB_KEY_l,            setmfact,         {.f = +0.05f} },
	{ MODKEY,                       XKB_KEY_Return,       zoom,             {0} },
	{ MODKEY,                       XKB_KEY_Tab,          view,             {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT,    XKB_KEY_C,            killclient,       {0} },
	{ MODKEY,                       XKB_KEY_t,            setlayout,        {.v = &layouts[0]} },
	//{ MODKEY,                       XKB_KEY_f,            setlayout,        {.v = &layouts[1]} },
	//{ MODKEY,                       XKB_KEY_m,            setlayout,        {.v = &layouts[2]} },
	//{ MODKEY,                       XKB_KEY_o,            setlayout,        {0} },
	{ MODKEY|ShiftMask,             XKB_KEY_space,        togglefloating,   {0} },
	{ MODKEY|ShiftMask,             XKB_KEY_F,            togglefullscreen, {0} },
	{ MODKEY,                       XKB_KEY_0,            view,             {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT,    XKB_KEY_parenright,   tag,              {.ui = ~0} },
	{ MODKEY|ControlMask,           XKB_KEY_h,            focusmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|ControlMask,           XKB_KEY_l,            focusmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|ShiftMask|ControlMask, XKB_KEY_H,            tagmon,           {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|ShiftMask|ControlMask, XKB_KEY_L,            tagmon,           {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          XKB_KEY_1,    XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2,    XKB_KEY_at,                         1),
	TAGKEYS(          XKB_KEY_3,    XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4,    XKB_KEY_dollar,                     3),
	TAGKEYS(          XKB_KEY_5,    XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6,    XKB_KEY_asciicircum,                5),
	TAGKEYS(          XKB_KEY_7,    XKB_KEY_ampersand,                  6),
	TAGKEYS(          XKB_KEY_8,    XKB_KEY_asterisk,                   7),
	TAGKEYS(          XKB_KEY_9,    XKB_KEY_parenleft,                  8),
	//{ MODKEY|WLR_MODIFIER_SHIFT,    XKB_KEY_Q,          quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_ALT|WLR_MODIFIER_LOGO|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT, XKB_KEY_P, togglepassthrough, {0} },
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove},   WLR_BUTTON_PRESSED },
//{ MODKEY, BTN_MIDDLE, togglefloating, {0}, WLR_BUTTON_PRESSED },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize}, WLR_BUTTON_PRESSED },
};
