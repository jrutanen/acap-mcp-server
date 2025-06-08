#include "list_tools.h"

json_t* list_tools(void) {
    json_t* response = json_object();
    json_t* tools = json_array();

    json_array_append_new(tools, json_string("get_location"));
    json_array_append_new(tools, json_string("get_status"));
    json_array_append_new(tools, json_string("get_metadata"));

    json_object_set_new(response, "tools", tools);
    return response;
}
