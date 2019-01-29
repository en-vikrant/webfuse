#include "wsfs/operations.h"

#include <string.h>
#include <jansson.h>

#include "wsfs/util.h"
#include "wsfs/jsonrpc.h"

#define WSFS_MAX_READ_LENGTH 4096

static wsfs_status wsfs_fill_buffer(
	char * buffer,
	size_t buffer_size,
	char const * format,
	char const * data,
	size_t count)
{
	wsfs_status status = WSFS_GOOD;

	size_t const copy_count = (buffer_size < count) ? buffer_size : count;
	if (0 < copy_count)
	{
		if (0 == strcmp("identity", format))
		{
			memcpy(buffer, data, copy_count);
		}
		else
		{
			status = WSFS_BAD;
		}
	}

	return status;
}

int wsfs_operation_read(
	const char * path,
 	char * buffer,
	size_t buffer_size,
	off_t  offset,
	struct fuse_file_info * WSFS_UNUSED_PARAM(file_info))
{
    struct fuse_context * context = fuse_get_context();
	struct wsfs_jsonrpc * rpc = context->private_data;

	int const length = (buffer_size <= WSFS_MAX_READ_LENGTH) ? (int) buffer_size : WSFS_MAX_READ_LENGTH;
	int result = 0;
	json_t * data = NULL;
	wsfs_status status = wsfs_jsonrpc_invoke(rpc, &data, "read", "sii", path, (int) offset, length);
	if (NULL != data)
	{
		json_t * data_holder = json_object_get(data, "data");
		json_t * format_holder = json_object_get(data, "format");
		json_t * count_holder = json_object_get(data, "count");

		if ((NULL != data_holder) && (json_is_string(data_holder)) &&
             	    (NULL != format_holder) && (json_is_string(format_holder)) &&
             	    (NULL != count_holder) && (json_is_integer(count_holder)))
		{
			char const * const data = json_string_value(data_holder);
			char const * const format = json_string_value(format_holder);
			int const count = json_integer_value(count_holder);

			status = wsfs_fill_buffer(buffer, buffer_size, format, data, count);
			if (WSFS_GOOD == status)
			{
				result = count;
			}
		}
		else
		{
			status = WSFS_BAD_FORMAT;
		}

		json_decref(data);
	}

	if (WSFS_GOOD != status)
	{
		result = wsfs_status_to_rc(status);
	}

    return result;
}