#include "wf/jsonrpc/impl/method.h"
#include <stdlib.h>
#include <string.h>

struct wf_jsonrpc_method * wf_jsonrpc_impl_method_create(
    char const * method_name,
    wf_jsonrpc_method_invoke_fn * invoke,
    void * user_data)
{
    struct wf_jsonrpc_method * method = malloc(sizeof(struct wf_jsonrpc_method));
    if (NULL != method)
    {
        method->next = NULL;
        method->name = strdup(method_name);
        method->invoke = invoke;
        method->user_data = user_data;
    }

    return method;
}

void wf_jsonrpc_impl_method_dispose(
    struct wf_jsonrpc_method * method)
{
    free(method->name);
    free(method);
}