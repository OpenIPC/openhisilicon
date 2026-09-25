#ifndef __XMDEIA_API_PDM_H__
#define __XMDEIA_API_PDM_H__

/* Well-known descriptor tags.
 *
 * PDM_DESCRIPTOR_TAG_ISP: see |pdm_descriptor_isp| struct.
 * PDM_DESCRIPTOR_TAG_SENSOR: see |pdm_descriptor_sensor| struct.
 */
typedef enum {
    PDM_DESCRIPTOR_TAG_MIN = 0x1010,
    PDM_DESCRIPTOR_TAG_ISP,
    PDM_DESCRIPTOR_TAG_SENSOR,
    PDM_DESCRIPTOR_TAG_MAX,
} pdm_descriptor_tag;

/* The header for a serialized descriptor.
 *
 * A descriptor always have two fields, a |tag| (denoting its type,
 * see the |pdm_descriptor_tag| enumeration) and the size of the bytes
 * following, |num_bytes_following|.
 *
 * For padding, |num_bytes_following| is always a multiple of 4.
 */
typedef struct {
    unsigned int tag;
    unsigned int num_bytes_following;
} pdm_descriptor;

/* struct 4-bytes alignment */
typedef struct {
    unsigned int iso;
    unsigned int exposure; /* 曝光量 */
    unsigned int exp_time; /* 曝光时间 */
    unsigned int ir_status;
    unsigned int awb_rgain;
    unsigned int awb_ggain;
    unsigned int awb_bgain;
} pdm_descriptor_isp;

/* struct 4-bytes alignment */
typedef struct {
    unsigned char name[5];
    unsigned char reserve[3];
} pdm_descriptor_sensor;

#ifdef __UBOOT__

/* Called only in u-boot */
/* Save media configuration parameters to pdm */
/* in [tag]  : PDM_DESCRIPTOR_TAG_ISP or PDM_DESCRIPTOR_TAG_SENSOR or ... */
/*             see the |pdm_descriptor_tag| enumeration */
/* in [data] : if [tag] is PDM_DESCRIPTOR_TAG_ISP, [data] is pdm_descriptor_isp struct; */
/*             if [tag] is PDM_DESCRIPTOR_TAG_SENSOR, [data] is pdm_descriptor_sensor struct; */
/*             ...*/
/*                                 */
/* return : return 0 if successful */
/*          return -1 if failed    */
int xmedia_api_pdm_set(unsigned int tag, const void *data);

/* Called only in u-boot */
/* Save data in pdm */
void xmedia_api_pdm_save(void);

#else // for kernel

/* Called only in the Linux user layer */
/* Get media configuration parameters from pdm */
/* in  [tag]  : PDM_DESCRIPTOR_TAG_ISP or PDM_DESCRIPTOR_TAG_SENSOR or ... */
/*              see the |pdm_descriptor_tag| enumeration */
/* out [data] : if [tag] is PDM_DESCRIPTOR_TAG_ISP, [data] is pdm_descriptor_isp struct; */
/*              if [tag] is PDM_DESCRIPTOR_TAG_SENSOR, [data] is pdm_descriptor_sensor struct; */
/*              ...*/
/*                                 */
/* return : return 0 if successful */
/*          return -1 if failed    */
int xmedia_api_pdm_get(unsigned int tag, void *data);

#endif // __UBOOT__

#endif

