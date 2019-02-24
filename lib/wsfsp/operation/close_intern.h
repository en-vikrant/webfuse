#ifndef WSFSP_OPERATION_CLOSE_INTERN_H
#define WSFSP_OPERATION_CLOSE_INTERN_H

#include "wsfsp/operation/close.h"
#include "wsfsp/provider_intern.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void wsfsp_close(
    struct wsfsp_invokation_context * context,
    json_t * params,
    int id);

extern void wsfsp_close_default(
    ino_t inode,
    uint32_t handle,
    int flags,
    void * user_data);

#ifdef __cplusplus
}
#endif

#endif