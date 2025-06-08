#include <glib-unix.h>
#include <glib.h>
#include <libmonkey.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include "mcp_handler.h"

#define APP_NAME "mcpserver"

static gboolean signal_handler(gpointer loop) {
    g_main_loop_quit((GMainLoop*)loop);
    syslog(LOG_INFO, "Application was stopped by SIGTERM or SIGINT.");
    return G_SOURCE_REMOVE;
}

__attribute__((noreturn)) __attribute__((format(printf, 1, 2))) static void
panic(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    vsyslog(LOG_ERR, format, arg);
    va_end(arg);
    exit(1);
}

int main(void) {
    GMainLoop* loop = NULL;

    openlog(APP_NAME, LOG_PID, LOG_USER);
    loop = g_main_loop_new(NULL, FALSE);

    mklib_ctx context = mklib_init(NULL, 0, 0, NULL);
    if (!context)
        panic("Could not create web server context, mklib_init failed.");

    mklib_callback_set(context, MKCB_DATA, get_mcp_callback());
    syslog(LOG_INFO, "Callback has been registered");

    mklib_start(context);
    syslog(LOG_INFO, "Server has started");

    g_unix_signal_add(SIGTERM, signal_handler, loop);
    g_unix_signal_add(SIGINT, signal_handler, loop);
    g_main_loop_run(loop);

    mklib_stop(context);
    g_main_loop_unref(loop);
}
