#ifndef libalg_api_h
#define libalg_api_h

#ifdef __cplusplus
extern "C"
{
#endif

    // <summary>
    // 图像结构
    // </summary>
    typedef struct ALG_IMAGE_INFO
    {
        unsigned int image_width; //图像宽度
        unsigned int image_height; //图像高度
        unsigned int image_stride; //图像行宽
        unsigned int image_format; //预留，暂时只支持NV21
        unsigned char *image_buffer; //图像buff
    }C_ALG_IMAGE_INFO;


    // <summary>
    // 返回定义
    // </summary>
    typedef enum ALG_RESULT
    {
        // <summary>
        // 执行成功
        // </summary>
        ALG_SUCCESS = 0,
        // <summary>
        // 算法未定义
        // </summary>
        ERR_ALG_NULL = 1000,
        // <summary>
        // KEY校验失败
        // </summary>
        ERR_KEY = 1001,
        // <summary>
        // 算法初始化失败
        // </summary>
        ERR_ALG_INIT = 1002,
        // <summary>
        // 创建算法失败
        // </summary>
        ERR_ALG_CREAT = 1003,
        // <summary>
        // 算法使能模块错误。
        // </summary>
        ERR_ALG_CONFLICT = 1004,
        // <summary>
        // 检测算法执行错误
        // </summary>
        ERR_ALG_DETECT_EXEC = 1005,
        // <summary>
        // 口罩算法执行错误
        // </summary>
        ERR_ALG_MASK_EXEC = 1006,
        // <summary>
        // 提取特征执行错误
        // </summary>
        ERR_ALG_FEATURE_EXEC = 1007,
        // <summary>
        // 人脸比对执行错误
        // </summary>
        ERR_ALG_MATCH_EXEC = 1008,
        // <summary>
        // 人形检测执行错误
        // </summary>
        ERR_ALG_PROCESS_EXEC = 1009,
        // <summary>
        // 检测人脸为空
        // </summary>
        ERR_ALG_FACE_NULL = 1010,
        // <summary>
        // 不允许多个人脸
        // </summary>
        ERR_ALG_FACE_MULTI = 1010,
        // <summary>
        // 姿态分数过低
        // </summary>
        ERR_ALG_FACE_QUALITY = 1011,
        // <summary>
        // 人脸注册数量已满
        // </summary>
        ERR_ALG_FACE_FULL = 1012,
        // <summary>
        // 不存在的注册信息
        // </summary>
        ERR_ALG_FACE_NOTUSER = 1013,
    }C_ALG_RESULT;


    // @brief 从原图仿射变换得到人脸小图
    // @param src_img    原始YUV图像,注意：图像buff为虚拟地址
    // @param dst_img    输出仿射变换的YUV归一化图,注意：该地址请预先分配好。
    // @param targetpoints 进行仿射变换归一的5个标签点，长度为10。其内存排列为：xy xy xy xy xy
    // （targetpoints[0]为第一个标签点的x，targetpoints[9]为第五个标签点的y）
    // @param keypoints 人脸检测模块输出的5个关键点，长度为10。其内存排列为：xy xy xy xy xy
    // （keypoints[0]为第一个关键点的x，keypoints[9]为第五个关键点的y）
    // @return 成功返回ALG_RESULT,其他值参见C_ALG_RESULT的定义
    C_ALG_RESULT alg_api_face_feature_warpaffine( C_ALG_IMAGE_INFO *src_img,
        C_ALG_IMAGE_INFO *dst_img,float *targetpoints,float *keypoints);

    // @brief 从原图仿射变换得到车牌小图
    // @param src_img    原始YUV图像,注意：图像buff为虚拟地址
    // @param dst_img    输出仿射变换的YUV归一化图,注意：该地址请预先分配好。
    // @param targetpoints 进行仿射变换归一的4个标签点，长度为8。其内存排列为：xy xy xy xy
    // （targetpoints[0]为第一个标签点的x，targetpoints[8]为第4个标签点的y）
    // @param keypoints 车牌检测模块输出的4个关键点，长度为8。其内存排列为：xy xy xy xy
    // （keypoints[0]为第一个关键点的x，keypoints[8]为第4个关键点的y）
    // @return 成功返回ALG_RESULT,其他值参见C_ALG_RESULT的定义
    C_ALG_RESULT alg_api_plate_warpaffine( C_ALG_IMAGE_INFO *src_img, C_ALG_IMAGE_INFO *dst_img,
        float *targetpoints,float *keypoints);
#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif