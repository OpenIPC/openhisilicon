#ifndef XMEDIA_CL_COMMON_H
#define XMEDIA_CL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_CL_MAX_DIMS_NUM 8
#define XMEDIA_CL_BATCH_NUM 8

/*xmedia_cl_job_schedule_prio_flags */
#define XMEDIA_CL_JOB_SCHEDULE_PRIO_MIN 0
#define XMEDIA_CL_JOB_SCHEDULE_PRIO_MEDIUM 1
#define XMEDIA_CL_JOB_SCHEDULE_PRIO_HIGH 2
#define XMEDIA_CL_JOB_SCHEDULE_PRIO_MAX 3

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
#define XMEDIA_CL_ERROR_MODEL_TYPE -68
#define XMEDIA_CL_ERROR_ADDR_ALIGN -69
#define XMEDIA_CL_UNSUPPORT_DYNAMIC_OUTPUT -70
#define XMEDIA_CL_MODEL_DECOMPRESS_FAIL -71
#define XMEDIA_CL_ERROR_COMPRESS_TYPE -72
#define XMEDIA_CL_ERROR_PROC_TYPE -73
#define XMEDIA_CL_OUT_OF_MAX_BATCH -74
#define XMEDIA_CL_NOT_FIND_FILE -75
#define XMEDIA_CL_CONTEXT_WORKSPACE_FAIL -76
#define XMEDIA_CL_PRIVATE_DATA_NULL -77
#define XMEDIA_CL_PRIVATE_DATA_SIZE_ERROR -78
#define XMEDIA_CL_UNSUPPORT_INFO_TYPE -79

typedef char xmedia_cl_s8;
typedef unsigned char xmedia_cl_u8;
typedef signed int xmedia_cl_s32;
typedef unsigned int xmedia_cl_u32;
typedef signed long long xmedia_cl_s64;
typedef unsigned long long xmedia_cl_u64;
typedef signed short xmedia_cl_s16;
typedef unsigned short xmedia_cl_u16;
typedef float xmedia_cl_float;
typedef double xmedia_cl_double;

typedef enum xmedia_cl_device_type {
    XMEDIA_CL_DEVICE_CPU = 0,
    XMEDIA_CL_DEVICE_NPU = 1,
    XMEDIA_CL_DEVICE_ALL = 2,
} xmedia_cl_device_type_e;

typedef enum tensor_type {
    XMEDIA_CL_TENSOR_INPUT = 0,
    XMEDIA_CL_TENSOR_CONST,
    XMEDIA_CL_TENSOR_OUTPUT,
    XMEDIA_CL_TENSOR_WORKSPACE,
    UNKNOWN
} xmedia_cl_tensor_type_e;

typedef enum xmedia_cl_status {
    XMEDIA_CL_QUEUED = 0,
    XMEDIA_CL_SUBMITTED = 1,
    XMEDIA_CL_RUNNING = 2,
    XMEDIA_CL_COMPLETED = 3,
    XMEDIA_CL_FAILED = 4,
} xmedia_cl_status_e;

typedef enum {
    XMEDIA_CL_INT8 = 0,
    XMEDIA_CL_UINT8 = 1,
    XMEDIA_CL_INT16 = 2,
    XMEDIA_CL_UINT16 = 3,
    XMEDIA_CL_FP16 = 4,
    XMEDIA_CL_INT32 = 5,
    XMEDIA_CL_FP32 = 6,
    XMEDIA_CL_INT4 = 7,
    XMEDIA_CL_UINT12 = 8,
    XMEDIA_CL_UINT10 = 9,
    XMEDIA_CL_DATA_TYPE_END
} xmedia_cl_data_type;

typedef enum {
    XMEDIA_CL_FORMAT_RGB = 0,
    XMEDIA_CL_FORMAT_RGrGbB,
    XMEDIA_CL_FORMAT_BGbGrR,
    XMEDIA_CL_FORMAT_GrRBGb = 3,
    XMEDIA_CL_FORMAT_GbBRGr,
    XMEDIA_CL_FORMAT_YUV,
    XMEDIA_CL_FORMAT_YVU = 6,
} xmedia_cl_data_format;

typedef enum {
    XMEDIA_CL_WORKSPACE = 0,
    XMEDIA_CL_WORKSPACE_INPUT = 1,
    XMEDIA_CL_WORKSPACE_OUTPUT = 2,
    XMEDIA_CL_WORKSPACE_INPUT_OUTPUT = 3,
} xmedia_cl_memory_reuse_type;

typedef enum {
    XMEDIA_CL_PROFILING_TYPE_NORMAL = 0,
    XMEDIA_CL_PROFILING_TYPE_SIMPLE = 1,
} xmedia_cl_profiling_type_e;

typedef enum {
    XMEDIA_CL_PROFILING_OFF = 0,
    XMEDIA_CL_PROFILING_ON = 1,
} xmedia_cl_profiling_flag_e;

typedef struct _xmedia_cl_profiling_params {
    xmedia_cl_profiling_type_e profiling_type;
    xmedia_cl_profiling_flag_e profiling_flag;
} xmedia_cl_profiling_params;

typedef struct _xmedia_cl_tensor_shape {
    xmedia_cl_u32 ndims;
    xmedia_cl_u32 dims[XMEDIA_CL_MAX_DIMS_NUM];
    xmedia_cl_u32 pch[XMEDIA_CL_MAX_DIMS_NUM];
    xmedia_cl_data_type type;
} xmedia_cl_tensor_shape;

typedef struct _xmedia_cl_tensor_quant {
    xmedia_cl_float scale;
    xmedia_cl_s32 zp;
} xmedia_cl_tensor_quant;

typedef struct _xmedia_cl_tensor_list {
    xmedia_cl_u32 num;
    xmedia_cl_u32 *id;
} xmedia_cl_tensor_list;

typedef struct _xmedia_cl_tensor_batch {
    xmedia_cl_u32 batch_count;
    xmedia_cl_u32 batch[XMEDIA_CL_BATCH_NUM];
} xmedia_cl_tensor_batch;

typedef struct _xmedia_cl_tensor {
    xmedia_cl_u32 tensor_id;
    void *addr;
    xmedia_cl_tensor_shape shape;
    xmedia_cl_tensor_quant quant;
    xmedia_cl_u32 size;
    xmedia_cl_s8 *name;
} xmedia_cl_tensor;

typedef struct _xmedia_cl_tensor_info_inout {
    xmedia_cl_u32 num;
    xmedia_cl_tensor *tensor;
    xmedia_cl_tensor_batch *tensor_batch;
    xmedia_cl_u32 *current_batch;
} xmedia_cl_tensor_info_inout;

typedef struct _xmedia_cl_mem_info {
    xmedia_cl_u32 worksize;
    xmedia_cl_u32 weightsize;
    xmedia_cl_u32 inputsize;
    xmedia_cl_u32 outputsize;
    xmedia_cl_u32 codesize;
    xmedia_cl_u32 memory_reuse_type;
    xmedia_cl_u32 private_data_size;
} xmedia_cl_mem_info;

typedef enum {
    XMEDIA_CL_MEM_INFO = 0,
} xmedia_cl_model_info_type;

typedef struct _xmedia_cl_device_id *xmedia_cl_device_id;
typedef struct _xmedia_cl_event *xmedia_cl_event;
typedef struct _xmedia_cl_context *xmedia_cl_context;

typedef void *xmedia_cl_graph;

#ifdef __cplusplus
}
#endif

#endif
