#ifndef XMEDIA_CL_H
#define XMEDIA_CL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <streamer.h>

/*xmedia_cl_mem_flags */
#define XMEDIA_CL_MEM_USE_HOST_PTR (1 << 3)
#define XMEDIA_CL_MEM_ALLOC (1 << 2)

#define XMEDIA_CL_TYPE_NONE (1 << 0)
#define XMEDIA_CL_TYPE_ARG_POINTER (1 << 1)
#define XMEDIA_CL_TYPE_BASE_POINTER (1 << 2)
#define XMEDIA_CL_TYPE_END_POINTER (1 << 3)
#define XMEDIA_CL_TYPE_CMD_POINTER (1 << 4)
#define XMEDIA_CL_TYPE_OFFSET_POINTER (1 << 5)

/*xmedia_cl_error_code*/
#define XMEDIA_CL_SUCCESS 0
#define XMEDIA_CL_OUT_OF_HOST_MEMORY -6
#define XMEDIA_CL_INVALID_VALUE -30
#define XMEDIA_CL_INVALID_DEVICE_TYPE -31
#define XMEDIA_CL_INVALID_PLATFORM -32
#define XMEDIA_CL_INVALID_DEVICE -33
#define XMEDIA_CL_INVALID_CONTEXT -34
#define XMEDIA_CL_INVALID_COMMAND_QUEUE -36
#define XMEDIA_CL_INVALID_HOST_PTR -37
#define XMEDIA_CL_INVALID_MEM_OBJECT -38
#define XMEDIA_CL_INVALID_BINARY -42
#define XMEDIA_CL_INVALID_PROGRAM -44
#define XMEDIA_CL_INVALID_PROGRAM_EXECUTABLE -45
#define XMEDIA_CL_INVALID_KERNEL_NAME -46
#define XMEDIA_CL_INVALID_KERNEL -48
#define XMEDIA_CL_INVALID_ARG_INDEX -49
#define XMEDIA_CL_INVALID_ARG_VALUE -50
#define XMEDIA_CL_INVALID_ARG_SIZE -51
#define XMEDIA_CL_INVALID_KERNEL_ARGS -52
#define XMEDIA_CL_WAIT_EVENT_FAILED -56
#define XMEDIA_CL_INVALID_EVENT_WAIT_LIST -57
#define XMEDIA_CL_INVALID_EVENT -58
#define XMEDIA_CL_INVALID_OPERATION -59
#define XMEDIA_CL_INVALID_UNINIT -60
#define XMEDIA_CL_INVALID_BUFFER_SIZE -61
#define XMEDIA_CL_INVALID_USER_FUNC -62
#define XMEDIA_CL_ALREADY_INIT -63
#define XMEDIA_CL_INVALID_MODEL -64
#define XMEDIA_CL_READ_MODEL_FAIL -65
#define XMEDIA_CL_INSUFFICIENT_SIZE -66
#define XMEDIA_CL_CALL_INTERFUNC_ERR -67

typedef char xmedia_cl_s8;
typedef unsigned char xmedia_cl_u8;
typedef signed int xmedia_cl_s32;
typedef unsigned int xmedia_cl_u32;
typedef signed long long xmedia_cl_s64;
typedef unsigned long long xmedia_cl_u64;
typedef float xmedia_cl_float;
typedef double xmedia_cl_double;

typedef enum xmedia_cl_device_type {
    XMEDIA_CL_DEVICE_CPU = 0,
    XMEDIA_CL_DEVICE_NPU = 1,
    XMEDIA_CL_DEVICE_ALL = 2,
} xmedia_cl_device_type_e;

typedef enum tensor_type {
    input = 0,
    xmedia_const,
    output,
    intermediate,
    UNKNOWN
} xmedia_cl_tensor_type_e;

typedef enum xmedia_cl_status {
    XMEDIA_CL_QUEUED = 0,
    XMEDIA_CL_SUBMITTED = 1,
    XMEDIA_CL_RUNNING = 2,
    XMEDIA_CL_COMPLETED = 3,
    XMEDIA_CL_FAILED = 4,
} xmedia_cl_status_e;

typedef struct _xmedia_cl_program_buffer {
    xmedia_cl_device_type_e type;
    void **data;
    xmedia_cl_u32 *buffer_size;
    xmedia_cl_u32 num_kernels;
    xmedia_cl_u8 *num_args;
    xmedia_cl_u8 **args_type;
    xmedia_cl_s8 *kernel_name[0];
} xmedia_cl_program_buffer_t;

typedef struct _xmedia_cl_program_binary {
    xmedia_cl_device_type_e type;
    xmedia_cl_s8 *library_name;
    xmedia_cl_s8 *binary_name;
    xmedia_cl_u32 num_kernels;
    xmedia_cl_u8 *num_args;
    xmedia_cl_s8 *kernel_name[0];
} xmedia_cl_program_binary_t;

typedef enum {
    XMEDIA_CL_INT8 = 0,
    XMEDIA_CL_UINT8 = 1,
    XMEDIA_CL_INT16 = 2,
    XMEDIA_CL_UINT16 = 3,
    XMEDIA_CL_FP16 = 4,
    XMEDIA_CL_INT32 = 5,
    XMEDIA_CL_FP32 = 6,
    XMEDIA_CL_DATA_TYPE_END
} xmedia_cl_data_type;

typedef struct _xmedia_cl_tensor_shape {
    int dims[4];
    int pch[4];
    xmedia_cl_data_type type;
} xmedia_cl_tensor_shape;

typedef struct _xmedia_cl_tensor_quant {
    float scale;
    int zp;
} xmedia_cl_tensor_quant;

typedef struct _xmedia_cl_tensor_list {
    int num;
    int *id;
} xmedia_cl_tensor_list;

typedef struct _xmedia_cl_tensor {
    int tensor_id;
    void *addr;
    xmedia_cl_tensor_shape shape;
    xmedia_cl_tensor_quant quant;
} xmedia_cl_tensor;

typedef struct _xmedia_cl_tensor_info_inout {
    unsigned int num;
    xmedia_cl_tensor *tensor;
} xmedia_cl_tensor_info_inout;

typedef struct _xmedia_cl_program_buffer *xmedia_cl_program_buffer;
typedef struct _xmedia_cl_program_binary *xmedia_cl_program_binary;
typedef struct _xmedia_cl_device_id *xmedia_cl_device_id;
typedef struct _xmedia_cl_event *xmedia_cl_event;
typedef struct _xmedia_cl_context *xmedia_cl_context;
typedef struct _xmedia_cl_command_queue *xmedia_cl_command_queue;
typedef struct _xmedia_cl_memory *xmedia_cl_memory;
typedef struct _xmedia_cl_program *xmedia_cl_program;
typedef struct _xmedia_cl_kernel *xmedia_cl_kernel;

typedef void *xmedia_cl_graph;

xmedia_cl_s32 xmedia_cl_init();
xmedia_cl_s32 xmedia_cl_uninit();

// get the device list
xmedia_cl_s32 xmedia_cl_get_device_ids(xmedia_cl_device_type_e device_type,
                                       xmedia_cl_device_id *devices,
                                       xmedia_cl_u32 *num_devices);

xmedia_cl_s32 xmedia_cl_release_device_ids(xmedia_cl_device_id *devices,
                                           xmedia_cl_u32 *num_devices);

xmedia_cl_context xmedia_cl_create_context(xmedia_cl_u32 num_devices,
                                           xmedia_cl_device_id *devices,
                                           xmedia_cl_s32 *err_code);

xmedia_cl_s32 xmedia_cl_release_context(xmedia_cl_context context);

xmedia_cl_command_queue
xmedia_cl_create_command_queue(xmedia_cl_context context,
                               xmedia_cl_device_id device,
                               xmedia_cl_s32 *err_code);

xmedia_cl_s32 xmedia_cl_release_command_queue(xmedia_cl_command_queue queue);

// event_list is array list, the last event is reference of event
xmedia_cl_s32 xmedia_cl_enqueue_command(xmedia_cl_command_queue command_queue,
                                        xmedia_cl_kernel kernel,
                                        xmedia_cl_u32 num_event_list,
                                        xmedia_cl_event *event_list,
                                        xmedia_cl_event *event);

xmedia_cl_s32 xmedia_cl_wait_for_events(xmedia_cl_u32 num_events,
                                        xmedia_cl_event *events);

xmedia_cl_s32 xmedia_cl_release_event(xmedia_cl_event event);

xmedia_cl_s32 xmedia_cl_query_event_status(xmedia_cl_event event,
                                           xmedia_cl_s32 *status);

xmedia_cl_program xmedia_cl_create_program_with_binary(
    xmedia_cl_context context, xmedia_cl_device_id device,
    const char *binary_name, xmedia_cl_s32 *err_code);

xmedia_cl_s32 xmedia_cl_release_program(xmedia_cl_program program);

xmedia_cl_kernel xmedia_cl_create_kernel(xmedia_cl_program program,
                                         const char *kernel_name,
                                         xmedia_cl_s32 *err_code);

xmedia_cl_s32 xmedia_cl_release_kernel(xmedia_cl_kernel kernel);

xmedia_cl_memory xmedia_cl_create_buffer(xmedia_cl_context context,
                                         xmedia_cl_u32 mem_flags,
                                         xmedia_cl_u32 size, void **host_ptr,
                                         xmedia_cl_s32 *err_code);

xmedia_cl_s32 xmedia_cl_release_memory_object(xmedia_cl_memory mem_obj);

xmedia_cl_s32 xmedia_cl_set_kernel_arg(xmedia_cl_kernel kernel,
                                       xmedia_cl_u32 arg_index,
                                       xmedia_cl_u32 arg_size, void *arg_value);

// wait for all the command of the queue
xmedia_cl_s32 xmedia_cl_finish(xmedia_cl_command_queue queue);

xmedia_cl_s32 xmedia_cl_create_binary(xmedia_cl_program_binary program_binary);

xmedia_cl_program xmedia_cl_create_program_with_buffer(
    xmedia_cl_context context, xmedia_cl_device_id device,
    xmedia_cl_u32 num_buffers, xmedia_cl_program_buffer *buffer,
    xmedia_cl_s32 *err_code);

void xmedia_cl_create_graph(char *graph_name, char *graph_version,
                            xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_create_tensor(xmedia_cl_graph hd,
                                      xmedia_cl_tensor_shape *shape,
                                      xmedia_cl_tensor_quant *quant,
                                      xmedia_cl_tensor_type_e type, char *data,
                                      xmedia_cl_s32 offset);
xmedia_cl_s32 xmedia_cl_create_node(xmedia_cl_graph hd, char *name,
                                    xmedia_cl_device_type_e type,
                                    xmedia_cl_u32 addr_offset,
                                    xmedia_cl_tensor_list *input,
                                    xmedia_cl_tensor_list *output, char *option,
                                    xmnpu_streamer_t *streamer[3]);
void xmedia_cl_build_graph(xmedia_cl_graph hd);

xmedia_cl_s32 xmedia_cl_graph_loadmodel_from_file(xmedia_cl_context *context,
                                                  char *model,
                                                  xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_graph_loadmodel_from_buff(xmedia_cl_context *context,
                                                  char *model,
                                                  xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_graph_get_input(xmedia_cl_graph graph,
                                        xmedia_cl_u32 input_num,
                                        xmedia_cl_tensor_info_inout *input);
xmedia_cl_s32 xmedia_cl_graph_set_input(xmedia_cl_graph graph,
                                        xmedia_cl_s32 tensor_id, void *addr);
xmedia_cl_s32 xmedia_cl_graph_process(xmedia_cl_graph graph);
xmedia_cl_s32 xmedia_cl_graph_submit(xmedia_cl_graph graph,
                                     xmedia_cl_event *event);
xmedia_cl_s32 xmedia_cl_graph_get_output(xmedia_cl_graph graph,
                                         xmedia_cl_u32 output_num,
                                         xmedia_cl_tensor_info_inout *output);
xmedia_cl_s32 xmedia_cl_graph_unload(xmedia_cl_graph graph);

#ifdef __cplusplus
}
#endif

#endif
