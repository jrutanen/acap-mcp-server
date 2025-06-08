// mcp_handler.c
#include <libmonkey.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <jansson.h>
#include "mcp_handler.h"
#include "list_tools.h"

#define APP_NAME "mcpserver"

static char json_buffer[2048];

// MCP handler to serve only list_tools
static int serve_web_page(const mklib_session* session,
                          const char* vhost,
                          const char* cgi_path,
                          const char* get,
                          unsigned long get_len,
                          const char* post,
                          unsigned long post_len,
                          unsigned int* status,
                          const char** content,
                          unsigned long* content_len,
                          char* header) {
    (void)session;
    (void)vhost;
    (void)cgi_path;
    (void)get;
    (void)get_len;
    (void)post_len;

    json_error_t error;
    json_t *root = json_loads(post, 0, &error);

    if (!root || !json_is_object(root)) {
        snprintf(json_buffer, sizeof(json_buffer), "{\"error\":\"Invalid JSON\"}");
        *status = 400;
    } else {
        const char* command = json_string_value(json_object_get(root, "command"));

        if (command && strcmp(command, "list_tools") == 0) {
            json_t* response = list_tools();
            char* result = json_dumps(response, JSON_COMPACT);
            snprintf(json_buffer, sizeof(json_buffer), "%s", result);
            free(result);
            json_decref(response);
            *status = 200;
        } else {
            snprintf(json_buffer, sizeof(json_buffer), "{\"error\":\"Unknown command\"}");
            *status = 404;
        }
        json_decref(root);
    }

    sprintf(header, "Content-type: application/json");
    *content = json_buffer;
    *content_len = strlen(json_buffer);

    return MKLIB_TRUE;
}

cb_data get_mcp_callback(void) {
    return serve_web_page;
}
