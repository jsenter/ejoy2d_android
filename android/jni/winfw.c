#include "opengl.h"
#include "ejoy2dgame.h"
#include "fault.h"
#include "screen.h"
#include "winfw.h"

#include <lauxlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

struct WINDOWGAME {
	struct game *game;
	int intouch;
	AAssetManager* assetMgr;
};

static const int BUFSIZE = 2048;

static struct WINDOWGAME *G = NULL;

static const char * startscript =
"local path,script = ...\n"
"require(\"ejoy2d.framework\").WorkDir = '/sdcard/ejoy2d'\n"
"assert(script, 'I need a script name')\n"
"path = string.match(path,[[(.*)/[^/]*$]])\n"
"package.path = [[/sdcard/ejoy2d/?.lua;/sdcard/ejoy2d/?/init.lua;/sdcard/ejoy2d/assets/?.lua;/sdcard/ejoy2d/assets/?/init.lua]]\n"
"local f = assert(loadfile(script))\n"
"f(script)\n"
;

static void stackdump_g(lua_State* l)
{
    int i;
    int top = lua_gettop(l);
 
    for (i = 1; i <= top; i++)
    {  /* repeat for each level */
        int t = lua_type(l, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                __android_log_print(ANDROID_LOG_INFO, "EJOY2D", "string: '%s'", lua_tostring(l, i));
                break;
            case LUA_TBOOLEAN:  /* booleans */
                __android_log_print(ANDROID_LOG_INFO, "EJOY2D", "boolean %s",lua_toboolean(l, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                __android_log_print(ANDROID_LOG_INFO, "EJOY2D", "number: %g", lua_tonumber(l, i));
                break;
            default:  /* other values */
                __android_log_print(ANDROID_LOG_INFO, "EJOY2D", "%s", lua_typename(l, t));
                break;
        }
    }
}



static struct WINDOWGAME *
create_game(JNIEnv * env, jobject asset) {
	struct WINDOWGAME * g = (struct WINDOWGAME *)malloc(sizeof(*g));
	g->game = ejoy2d_game();
	g->intouch = 0;
	g->assetMgr = AAssetManager_fromJava(env, asset);
	return g;
}

static int
traceback(lua_State *L) {
	const char *msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	else if (!lua_isnoneornil(L, 1)) {
	if (!luaL_callmeta(L, 1, "__tostring"))
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

#ifdef __APPLE__
static const char*
_read_exepath(char * buf, int bufsz) {
    return getenv("_");
}
#define read_exepath(buf,bufsz) _read_exepath(buf,bufsz)

#else
static const char*
read_exepath(char * buf, int bufsz) {
    int  count;
    count = readlink("/proc/self/exe", buf, bufsz);

    if (count < 0)
        return NULL;
    return buf;
}
#endif

static char * load_script(JNIEnv * env, const char * script) {
    AAsset* asset = AAssetManager_open(G->assetMgr, (const char *)script, AASSET_MODE_UNKNOWN);
    if (NULL == asset) {
        __android_log_print(ANDROID_LOG_ERROR, "EJOY2D", "%s, %s, %d: %s", __FILE__, __FUNCTION__, __LINE__, "Asset not found.");
        return JNI_FALSE;
    }
    long size = AAsset_getLength(asset);
    char* buffer = (char*) malloc (sizeof(char)*size);
    AAsset_read (asset,buffer,size);
    AAsset_close(asset);
    return buffer;
}

void
ejoy2d_win_init(JNIEnv * env, jobject asset, int w, int h) {
	G = create_game(env, asset);
	lua_State *L = ejoy2d_game_lua(G->game);
	lua_pushcfunction(L, traceback);
	int tb = lua_gettop(L);

	int err = luaL_loadstring(L, startscript);
	if (err) {
		const char *msg = lua_tostring(L,-1);
        __android_log_print(ANDROID_LOG_ERROR, "EJOY2D", "%s, %s, %d: %s", __FILE__, __FUNCTION__, __LINE__, msg);
	}

    lua_pushstring(L, "/sdcard/ejoy2d");
    lua_pushstring(L, "/sdcard/ejoy2d/ex04.lua");

	err = lua_pcall(L, 2, 0, tb);
	if (err) {
		const char *msg = lua_tostring(L,-1);
        __android_log_print(ANDROID_LOG_ERROR, "EJOY2D", "%s, %s, %d: %s", __FILE__, __FUNCTION__, __LINE__, msg);
	}

	lua_pop(L,1);

    font_init();
	screen_init(w, h,1.0f);
	ejoy2d_game_start(G->game);
}

void
ejoy2d_win_update() {
	ejoy2d_game_update(G->game, 0.01f);
}

void
ejoy2d_win_frame() {
	ejoy2d_game_drawframe(G->game);
}

void
ejoy2d_win_touch(float x, float y,int touch) {
	switch (touch) {
	case TOUCH_BEGIN:
		G->intouch = 1;
		break;
	case TOUCH_END:
		G->intouch = 0;
		break;
	case TOUCH_MOVE:
		if (!G->intouch) {
			return;
		}
		break;
	}
	// windows only support one touch id (0)
	int id = 0;
	ejoy2d_game_touch(G->game, id, x,y,touch);
}

