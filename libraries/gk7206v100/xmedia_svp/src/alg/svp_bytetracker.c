#include "svp_bytetracker.h"


static xmedia_float iou_with_negative(xmedia_svp_tracker_single_result *box1, xmedia_svp_tracker_single_result *box2)
{
    xmedia_float area1;
    xmedia_float area2;
    xmedia_float mix_w;
    xmedia_float mix_h;
    xmedia_float mix_area;

    if (box1->class_type != box2->class_type) {
        return 0.0f;
    }

    mix_w = STD_MIN(box1->rect.x2, box2->rect.x2) - STD_MAX(box1->rect.x1, box2->rect.x1);
    mix_h = STD_MIN(box1->rect.y2, box2->rect.y2) - STD_MAX(box1->rect.y1, box2->rect.y1);

    mix_area = abs(mix_w * mix_h);
    if (mix_w < 0 || mix_h < 0)
        mix_area = -mix_area;

    area1 = (box1->rect.x2 - box1->rect.x1) * (box1->rect.y2 - box1->rect.y1);
    area2 = (box2->rect.x2 - box2->rect.x1) * (box2->rect.y2 - box2->rect.y1);

    return (mix_area / (area1 + area2 - mix_area));
}

static xmedia_s32 matrix_inverse_dim4(xmedia_float matrix[4][4], xmedia_float matrix_inverse[4][4])
{
    xmedia_float det = 0; // 行列式
    xmedia_float adjoint_matrix[4][4];
    xmedia_u32 i, j;

    // 求矩阵行列式
    for (i = 0; i < 4; i++) {
        det += (matrix[0][i] * (matrix[1][(i + 1) % 4] * matrix[2][(i + 2) % 4] * matrix[3][(i + 3) % 4]
                - matrix[1][(i + 3) % 4] * matrix[2][(i + 2) % 4] * matrix[3][(i + 1) % 4]));
    }

    if (det == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "singular matrix\n");
        return XMEDIA_FAILURE;
    }

    // 求伴随矩阵
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            adjoint_matrix[i][j] = (matrix[(i + 1) % 4][(j + 1) % 4] * matrix[(i + 2) % 4][(j + 2) % 4] *
                                    matrix[(i + 3) % 4][(j + 3) % 4]) - (matrix[(i + 1) % 4][(j + 3) % 4] *
                                    matrix[(i + 2) % 4][(j + 2) % 4] * matrix[(i + 3) % 4][(j + 1) % 4]);
        }
    }

    // 矩阵求逆
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            matrix_inverse[i][j] = adjoint_matrix[j][i] / det;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 matrix_add(xmedia_float* matrix1, xmedia_float* matrix2, xmedia_float* matrix3, xmedia_u32 row,
                            xmedia_u32 col)
{
    xmedia_u32 i;
    xmedia_u32 size = row * col;

    if ((matrix1 == XMEDIA_NULL) || (matrix2 == XMEDIA_NULL) || (matrix3 == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    if ((row == XMEDIA_NULL) || (col == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "wrong matrix dim\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < size; i++) {
        *(matrix3 + i) = *(matrix1 + i) + *(matrix2 + i);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 matrix_subtract(xmedia_float* matrix1, xmedia_float* matrix2, xmedia_float* matrix3, xmedia_u32 row,
                                    xmedia_u32 col)
{
    xmedia_u32 i;
    xmedia_u32 size = row * col;

    if ((matrix1 == XMEDIA_NULL) || (matrix2 == XMEDIA_NULL) || (matrix3 == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    if ((row == XMEDIA_NULL) || (col == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "wrong matrix dim\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < size; i++) {
        *(matrix3 + i) = *(matrix1 + i) - *(matrix2 + i);
    }

    return XMEDIA_SUCCESS;
}


static xmedia_s32 matrix_transpose(xmedia_float* matrix, xmedia_float* matrix_transpose, xmedia_u32 row, xmedia_u32 col)
{
    xmedia_u32 i, j;

    if ((matrix == XMEDIA_NULL) || (matrix_transpose == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    if ((row == XMEDIA_NULL) || (col == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "wrong matrix dim\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            *(matrix_transpose + j * row + i) = *(matrix + i * col + j);
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 matrix_mul(xmedia_float* matrix1, xmedia_float* matrix2, xmedia_float* matrix3, xmedia_u32 row1,
                            xmedia_u32 row2, xmedia_u32 col1, xmedia_u32 col2)
{
    xmedia_u32 i;//行
    xmedia_u32 j;//列
    xmedia_u32 k;//行列中，第k个元素相乘

    if ((matrix1 == XMEDIA_NULL) || (matrix2 == XMEDIA_NULL) || (matrix3 == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    if ((col1 != row2) || (row1 == XMEDIA_NULL) || (row2 == XMEDIA_NULL) || (col1 == XMEDIA_NULL) || (col2 == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "wrong matrix dim\n");
        return XMEDIA_FAILURE;
    }

    memset(matrix3, 0, row1 * col2 * sizeof(xmedia_float));

    for (i = 0; i < row1; i++) {
        for (j = 0; j < col2; j++) {
            for (k = 0; k < col1; k++) {
                if (fabs(*(matrix1 + i * col1 + k)) < SVP_TRACK_EPSILON || fabs(*(matrix2 + k * col2 + j)) < SVP_TRACK_EPSILON) {
                    continue;
                } else {
                    *(matrix3 + i * col2 + j) += *(matrix1 + i * col1 + k) * *(matrix2 + k * col2 + j);
                }
            }
        }
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 compute_motion_nov(svp_kalman_measure measure, svp_kalman_filter* kalman_filter)
{
    xmedia_u32 i;

    if (kalman_filter == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    memset(kalman_filter->Q, 0, sizeof(kalman_filter->Q));
    kalman_filter->Q[0][0] = SVP_STD_POSITION * measure.h;
    kalman_filter->Q[1][1] = SVP_STD_POSITION * measure.h;
    kalman_filter->Q[2][2] = 1e-2;
    kalman_filter->Q[3][3] = SVP_STD_POSITION * measure.h;
    kalman_filter->Q[4][4] = SVP_STD_VELOCITY * measure.h;
    kalman_filter->Q[5][5] = SVP_STD_VELOCITY * measure.h;
    kalman_filter->Q[6][6] = 1e-5;
    kalman_filter->Q[7][7] = SVP_STD_VELOCITY * measure.h;

    for (i = 0; i < 8; i++) {
        kalman_filter->Q[i][i] *= kalman_filter->Q[i][i];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 compute_measure_nov(svp_kalman_measure measure, svp_kalman_filter* kalman_filter)
{
    xmedia_u32 i;

    if (kalman_filter == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    memset(kalman_filter->R, 0, sizeof(kalman_filter->R));
    kalman_filter->R[0][0] = SVP_STD_POSITION * measure.h;
    kalman_filter->R[1][1] = SVP_STD_POSITION * measure.h;
    kalman_filter->R[2][2] = 1e-1;
    kalman_filter->R[3][3] = SVP_STD_POSITION * measure.h;
    for (i = 0; i < 4; i++) {
        kalman_filter->R[i][i] *= kalman_filter->R[i][i];
    }
    return XMEDIA_SUCCESS;
}

static svp_kalman_measure rect2kalman_measure(xmedia_svp_rect rect)
{
    svp_kalman_measure measure;

    measure.x = (rect.x1 + rect.x2) / 2.0;
    measure.y = (rect.y1 + rect.y2) / 2.0;
    measure.h = (rect.y2 - rect.y1) < SVP_TRACK_EPSILON ? SVP_TRACK_EPSILON : (rect.y2 - rect.y1); //防止除以0
    measure.a = (rect.x2 - rect.x1) / measure.h;

    return measure;
}

static xmedia_svp_rect kalman_measure2rect(svp_kalman_measure measure)
{
    xmedia_svp_rect rect;
    xmedia_float weight;

    weight = measure.a * measure.h;

    rect.x1 = STD_MAX(0.0f, measure.x - weight / 2);
    rect.x2 = measure.x + weight / 2;
    rect.y1 = STD_MAX(0.0f, measure.y - measure.h / 2);
    rect.y2 = measure.y + measure.h / 2;

    return rect;
}

static xmedia_s32 kalman_filter_init(svp_kalman_filter* kalman_filter, xmedia_svp_rect init_rect)
{
    xmedia_u32 i;
    svp_kalman_measure init_state;

    init_state = rect2kalman_measure(init_rect);

    if (kalman_filter == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    xmedia_float temp[8][8] = {{1, 0, 0, 0, 1, 0, 0, 0},
                               {0, 1, 0, 0, 0, 1, 0, 0},
                               {0, 0, 1, 0, 0, 0, 1, 0},
                               {0, 0, 0, 1, 0, 0, 0, 1},
                               {0, 0, 0, 0, 1, 0, 0, 0},
                               {0, 0, 0, 0, 0, 1, 0, 0},
                               {0, 0, 0, 0, 0, 0, 1, 0},
                               {0, 0, 0, 0, 0, 0, 0, 1}};

    memcpy(&kalman_filter->F[0][0], &temp[0][0], sizeof(temp));

    memset(kalman_filter->H, 0, sizeof(kalman_filter->H));
    for (i = 0; i < 4; i++) {
        kalman_filter->H[i][i] = 1;
    }

    memset(kalman_filter->P, 0, sizeof(kalman_filter->P));
    kalman_filter->P[0][0] = 2 * SVP_STD_POSITION * init_state.h;
    kalman_filter->P[1][1] = 2 * SVP_STD_POSITION * init_state.h;
    kalman_filter->P[2][2] = 1e-2;
    kalman_filter->P[3][3] = 2 * SVP_STD_POSITION * init_state.h;
    kalman_filter->P[4][4] = 10 * SVP_STD_VELOCITY * init_state.h;
    kalman_filter->P[5][5] = 10 * SVP_STD_VELOCITY * init_state.h;
    kalman_filter->P[6][6] = 1e-5;
    kalman_filter->P[7][7] = 10 * SVP_STD_VELOCITY * init_state.h;
    for (i = 0; i < 8; i++) {
        kalman_filter->P[i][i] *= kalman_filter->P[i][i];
    }

    memset(kalman_filter->state, 0, sizeof(kalman_filter->state));
    kalman_filter->state[0][0] = init_state.x;
    kalman_filter->state[1][0] = init_state.y;
    kalman_filter->state[2][0] = init_state.a;
    kalman_filter->state[3][0] = init_state.h;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 kalman_filter_predict(svp_bytetracker* tracker, xmedia_svp_rect* predicted_rect)
{
    xmedia_s32 ret;
    svp_kalman_measure measure;
    xmedia_float temp_state[8][1];
    xmedia_float temp_P[8][8];
    xmedia_float F_transpose[8][8];

    tracker->lost_count++;

    if (tracker->age < SVP_TRACK_AGE_LIMIT) {
        tracker->age++;
    }

    if (tracker->lost_count != 1) {
        tracker->kalman_filter.state[7][0] = 0;
    };

    // 计算运动噪声
    measure.x = tracker->kalman_filter.state[0][0];
    measure.y = tracker->kalman_filter.state[1][0];
    measure.a = tracker->kalman_filter.state[2][0];
    measure.h = tracker->kalman_filter.state[3][0];

    ret = compute_motion_nov(measure, &tracker->kalman_filter);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "compute_motion_nov failed\n");
        return XMEDIA_FAILURE;
    }


    // 更新状态 x'(k) = F * x(k-1)
    // state_ = F_ * state_;
    ret = matrix_mul(&tracker->kalman_filter.F[0][0], &tracker->kalman_filter.state[0][0], &temp_state[0][0],
                        8, 8, 8, 1);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    memcpy(&tracker->kalman_filter.state[0][0], &temp_state[0][0], sizeof(xmedia_float) * 8 * 1);

    // 更新协方差矩阵 P = F * P * F_t + Q
    // P_ = F_ * P_ * F_.transpose() + Q;
    ret = matrix_mul(&tracker->kalman_filter.F[0][0], &tracker->kalman_filter.P[0][0], &temp_P[0][0], 8, 8, 8, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_transpose(&tracker->kalman_filter.F[0][0], &F_transpose[0][0], 8, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_transpose failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_mul(&temp_P[0][0], &F_transpose[0][0], &tracker->kalman_filter.P[0][0], 8, 8, 8, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_add(&tracker->kalman_filter.P[0][0], &tracker->kalman_filter.Q[0][0], &tracker->kalman_filter.P[0][0],
                        8, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_add failed\n");
        return XMEDIA_FAILURE;
    }

    measure.x = tracker->kalman_filter.state[0][0];
    measure.y = tracker->kalman_filter.state[1][0];
    measure.a = tracker->kalman_filter.state[2][0];
    measure.h = tracker->kalman_filter.state[3][0];

    *predicted_rect = kalman_measure2rect(measure);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 kalman_filter_update(svp_bytetracker* tracker,
                                       xmedia_svp_rect* result_rect,
                                       xmedia_svp_keypoint *keypoint)
{
    xmedia_s32 ret;
    svp_kalman_measure measure;
    xmedia_float H_transpose[8][4];
    xmedia_float P_Ht[8][4];
    xmedia_float H_P_Ht[4][4];
    xmedia_float matrix_4x4[4][4];
    xmedia_float matrix_inversed[4][4];
    xmedia_float matrix_4x1[4][1];
    xmedia_float state_temp[8][1];
    xmedia_float K_H[8][8];
    xmedia_float K_H_P[8][8];

    tracker->lost_count = 0;
    tracker->state = SVP_TRACKER_STATE_TRACKED;
    memcpy(tracker->kpt, keypoint , sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);

    measure = rect2kalman_measure(*result_rect);
    xmedia_float kf_measurement[4][1] = {{measure.x}, {measure.y}, {measure.a}, {measure.h}};

    // 计算测量噪声R
    measure.x = tracker->kalman_filter.state[0][0];
    measure.y = tracker->kalman_filter.state[1][0];
    measure.a = tracker->kalman_filter.state[2][0];
    measure.h = tracker->kalman_filter.state[3][0];

    ret = compute_measure_nov(measure, &tracker->kalman_filter);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "compute_measure_nov failed\n");
        return XMEDIA_FAILURE;
    }

    // 计算Kalman增益，K = P * H * inv(H * P * H_t + R)
    ret = matrix_transpose(&tracker->kalman_filter.H[0][0], &H_transpose[0][0], 4, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_transpose failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_mul(&tracker->kalman_filter.P[0][0], &H_transpose[0][0], &P_Ht[0][0], 8, 8, 8, 4);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_mul(&tracker->kalman_filter.H[0][0], &P_Ht[0][0], &H_P_Ht[0][0], 4, 8, 8, 4);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_add(&H_P_Ht[0][0], &tracker->kalman_filter.R[0][0], &matrix_4x4[0][0], 4, 4);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_add failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_inverse_dim4(matrix_4x4, matrix_inversed);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_inverse_dim4 failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_mul(&P_Ht[0][0], &matrix_inversed[0][0], &tracker->kalman_filter.K[0][0], 8, 4, 4, 4);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    //更新状态值 x(k) = x‘(k) + K * (z(k) - H * x‘(k))
    //state_ = state_ + K * (measure - H_ * state_);
    ret = matrix_mul(&tracker->kalman_filter.H[0][0], &tracker->kalman_filter.state[0][0], &matrix_4x1[0][0],
                        4, 8, 8, 1);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_subtract(&kf_measurement[0][0], &matrix_4x1[0][0], &matrix_4x1[0][0], 4, 1);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_subtract failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_mul(&tracker->kalman_filter.K[0][0], &matrix_4x1[0][0], &state_temp[0][0], 8, 4, 4, 1);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_add(&tracker->kalman_filter.state[0][0], &state_temp[0][0], &tracker->kalman_filter.state[0][0], 8, 1);

    //更新协方差矩阵 P = P - K * H * P
    //P_ = P_ - K * H_ * P_;
    ret = matrix_mul(&tracker->kalman_filter.K[0][0], &tracker->kalman_filter.H[0][0], &K_H[0][0], 8, 4, 4, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_mul(&K_H[0][0], &tracker->kalman_filter.P[0][0], &K_H_P[0][0], 8, 8, 8, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_mul failed\n");
        return XMEDIA_FAILURE;
    }

    ret = matrix_subtract(&tracker->kalman_filter.P[0][0], &K_H_P[0][0], &tracker->kalman_filter.P[0][0], 8, 8);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "matrix_subtract failed\n");
        return XMEDIA_FAILURE;
    }

    tracker->rect = *result_rect;

    return XMEDIA_SUCCESS;
}

xmedia_s32 build_assignment(xmedia_s32* assignment, xmedia_bool* star_matrix, xmedia_u32 row, xmedia_u32 col)
{
    xmedia_u32 i, j;

    if ((assignment == XMEDIA_NULL) || (star_matrix == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (star_matrix[i + j * row]) {
                assignment[i] = j;
                break;
            }
        }
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 hungarian_step2(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim);
xmedia_s32 hungarian_step3(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim);


xmedia_s32 hungarian_step1(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim)
{
    xmedia_s32 ret;
    xmedia_bool* star_matrix_temp;
    xmedia_bool* col_end;
    xmedia_u32 i;

    if ((assignment == XMEDIA_NULL) || (dist_matrix == XMEDIA_NULL) || (star_matrix == XMEDIA_NULL) ||
        (new_star_matrix == XMEDIA_NULL) || (prime_matrix == XMEDIA_NULL) || (covered_col == XMEDIA_NULL) ||
        (covered_row == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < col; i++) {
        star_matrix_temp = star_matrix + i * row;
        col_end = star_matrix_temp + row;
        while (star_matrix_temp < col_end) {
            if (*star_matrix_temp++) {
                covered_col[i] = XMEDIA_TRUE;
                break;
            }
        }
    }

    ret = hungarian_step2(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix, covered_col, covered_row,
                            row, col, min_dim);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "hungarian_step2 failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 hungarian_step5(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim)
{
    xmedia_s32 ret;
    xmedia_float min_value = 1;
    xmedia_float value;
    xmedia_u32 i, j;

    if ((assignment == XMEDIA_NULL) || (dist_matrix == XMEDIA_NULL) || (star_matrix == XMEDIA_NULL) ||
        (new_star_matrix == XMEDIA_NULL) || (prime_matrix == XMEDIA_NULL) || (covered_col == XMEDIA_NULL) ||
        (covered_row == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    // find smallest uncovered element min_value
    for (i = 0; i < row; i++) {
        if (!covered_row[i]) {
            for (j = 0; j < col; j++) {
                if (!covered_col[j]) {
                    value = dist_matrix[i + j * row];
                    if (value < min_value) {
                        min_value = value;
                    }
                }
            }
        }
    }

    // add min_value to each covered row
    for (i = 0; i < row; i++) {
        if (covered_row[i]) {
            for (j = 0; j < col; j++) {
                dist_matrix[i + j * row] += min_value;
            }
        }
    }

    // subtract min_value from each uncovered column
    for (j = 0; j < col; j++) {
        if (!covered_col[j]) {
            for (i = 0; i < row; i++) {
                dist_matrix[i + j * row] -= min_value;
            }
        }
    }

    ret = hungarian_step3(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix, covered_col, covered_row,
                            row, col, min_dim);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "hungarian_step3 failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 hungarian_step4(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim,
                                    xmedia_u32 col_index, xmedia_u32 row_index)
{
    xmedia_s32 ret;
    xmedia_u32 size = row * col;
    xmedia_u32 i, star_col, star_row, prime_row, prime_col;

    if ((assignment == XMEDIA_NULL) || (dist_matrix == XMEDIA_NULL) || (star_matrix == XMEDIA_NULL) ||
        (new_star_matrix == XMEDIA_NULL) || (prime_matrix == XMEDIA_NULL) || (covered_col == XMEDIA_NULL) ||
        (covered_row == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < size; i++) {
        new_star_matrix[i] = star_matrix[i];
    }

    new_star_matrix[row_index + col_index * row] = XMEDIA_TRUE;

    star_col = col_index;
    for (star_row = 0; star_row < row; star_row++) {
        if (star_matrix[star_row + star_col * row]) {
            break;
        }
    }

    while (star_row < row) {
        new_star_matrix[star_row + star_col * row] = XMEDIA_FALSE;
        prime_row = star_row;
        for (prime_col = 0;prime_col < col; prime_col++) {
            if (prime_matrix[prime_row + prime_col * row]) {
                break;
            }
        }
        new_star_matrix[prime_row + prime_col * row] = XMEDIA_TRUE;

        star_col = prime_col;
        for (star_row = 0; star_row < row; star_row++) {
            if (star_matrix[star_row + star_col * row]) {
                break;
            }
        }
    }

    // use temporary copy as new starMatrix
    // delete all primes, uncover all rows
    for (i = 0; i < size; i++) {
        prime_matrix[i] = XMEDIA_FALSE;
        star_matrix[i] = new_star_matrix[i];
    }
    for (i = 0; i < row; i++) {
        covered_row[i] = XMEDIA_FALSE;
    }

    ret = hungarian_step1(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix, covered_col, covered_row,
                            row, col, min_dim);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "hungarian_step1 failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 hungarian_step3(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim)
{
    xmedia_s32 ret;
    xmedia_u32 i, j, star_col;
    xmedia_bool zeros_found = XMEDIA_TRUE;

    if ((assignment == XMEDIA_NULL) || (dist_matrix == XMEDIA_NULL) || (star_matrix == XMEDIA_NULL) ||
        (new_star_matrix == XMEDIA_NULL) || (prime_matrix == XMEDIA_NULL) || (covered_col == XMEDIA_NULL) ||
        (covered_row == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    while (zeros_found) {
        zeros_found = XMEDIA_FALSE;
        for (i = 0; i < col; i++) {
            if (!covered_col[i]) {
                for (j = 0; j < row; j++) {
                    if ((!covered_row[j]) && (fabs(dist_matrix[j + i * row]) < SVP_TRACK_EPSILON)) {
                        prime_matrix[j + i * row] = XMEDIA_TRUE;
                        for (star_col = 0; star_col < col; star_col++) {
                            if (star_matrix[j + star_col * row]) {
                                break;
                            }
                        }
                        if (star_col == col) { // no starred zero found
                            ret = hungarian_step4(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix,
                                                    covered_col, covered_row, row, col, min_dim, i, j);
                            return XMEDIA_SUCCESS;
                        } else {
                            covered_row[j] = XMEDIA_TRUE;
                            covered_col[star_col] = XMEDIA_FALSE;
                            zeros_found = XMEDIA_TRUE;
                            break;
                        }
                    }
                }
            }
        }
    }

    ret = hungarian_step5(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix, covered_col, covered_row,
                            row, col, min_dim);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "hungarian_step5 failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 hungarian_step2(xmedia_s32* assignment, xmedia_float* dist_matrix, xmedia_bool* star_matrix,
                                    xmedia_bool* new_star_matrix, xmedia_bool* prime_matrix, xmedia_bool* covered_col,
                                    xmedia_bool* covered_row, xmedia_u32 row, xmedia_u32 col, xmedia_u32 min_dim)
{
    xmedia_u32 count = 0;
    xmedia_u32 i;
    xmedia_s32 ret;

    if ((assignment == XMEDIA_NULL) || (dist_matrix == XMEDIA_NULL) || (star_matrix == XMEDIA_NULL) ||
        (new_star_matrix == XMEDIA_NULL) || (prime_matrix == XMEDIA_NULL) || (covered_col == XMEDIA_NULL) ||
        (covered_row == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    // count covered columns
    for (i = 0; i < col; i++) {
        if (covered_col[i]) {
            count++;
        }
    }

    if (count == min_dim) {
        //finish
        ret = build_assignment(assignment, star_matrix, row, col);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "build_assignment failed\n");
            return XMEDIA_FAILURE;
        }
    } else {
        ret = hungarian_step3(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix, covered_col,
                                covered_row, row, col, min_dim);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "hungarian_step3 failed\n");
            return XMEDIA_FAILURE;
        }
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 assignment_optimal(xmedia_s32* assignment, xmedia_float* dist_matrix_in, xmedia_u32 row,
                                        xmedia_u32 col)
{
    xmedia_s32 ret;
    xmedia_u32 i, j, size, min_dim;
    xmedia_float* dist_matrix = XMEDIA_NULL;
    xmedia_float* dist_matrix_end = XMEDIA_NULL;
    xmedia_float value, min_value;
    xmedia_bool* covered_col = XMEDIA_NULL;
    xmedia_bool* covered_row = XMEDIA_NULL;
    xmedia_bool* star_matrix = XMEDIA_NULL;
    xmedia_bool* prime_matrix = XMEDIA_NULL;
    xmedia_bool* new_star_matrix = XMEDIA_NULL;
    xmedia_float* dist_matrix_temp;
    xmedia_float* column_end;

    if ((assignment == XMEDIA_NULL) || (dist_matrix_in == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    // generate working copy of distance Matrix
    // check if all matrix elements are positive
    size = row * col;
    dist_matrix = (xmedia_float*)malloc(size * sizeof(xmedia_float));
    dist_matrix_end = dist_matrix + size;

    for (i = 0; i < size; i++) {
            dist_matrix[i] = dist_matrix_in[i];
    }

    covered_col = (xmedia_bool*)calloc(col, sizeof(xmedia_bool));
    CHECK_CALLOC_RETURN_ERROR(covered_col);
    covered_row = (xmedia_bool*)calloc(row, sizeof(xmedia_bool));
    CHECK_CALLOC_RETURN_ERROR(covered_row);
    star_matrix = (xmedia_bool*)calloc(size, sizeof(xmedia_bool));
    CHECK_CALLOC_RETURN_ERROR(star_matrix);
    prime_matrix = (xmedia_bool*)calloc(size, sizeof(xmedia_bool));
    CHECK_CALLOC_RETURN_ERROR(prime_matrix);
    new_star_matrix = (xmedia_bool*)calloc(size, sizeof(xmedia_bool));
    CHECK_CALLOC_RETURN_ERROR(new_star_matrix);

    if (row <= col) {
        min_dim = row;

        for (i = 0; i < row; i++) {
            // find the smallest element in the row
            dist_matrix_temp = dist_matrix + i;
            min_value = *dist_matrix_temp;
            dist_matrix_temp += row;
            while (dist_matrix_temp < dist_matrix_end) {
                value = *dist_matrix_temp;
                if (value < min_value) {
                    min_value = value;
                }
                dist_matrix_temp += row;
            }

            // subtract the smallest element from each element of the row
            dist_matrix_temp = dist_matrix + i;
            while (dist_matrix_temp < dist_matrix_end) {
                *dist_matrix_temp -= min_value;
                dist_matrix_temp += row;
            }
        }

        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                if (fabs(dist_matrix[i + j * row]) < SVP_TRACK_EPSILON) {
                    if (!covered_col[j]) {
                        star_matrix[i + j * row] = XMEDIA_TRUE;
                        covered_col[j] = XMEDIA_TRUE;
                        break;
                    }
                }
            }
        }
    } else {
        min_dim = col;

        for (i = 0; i < col; i++) {
            //find the smallest element in the column
            dist_matrix_temp = dist_matrix + i * row;
            column_end = dist_matrix_temp + row;
            min_value = *dist_matrix_temp;
            dist_matrix_temp++;

            while (dist_matrix_temp < column_end) {
                value = *dist_matrix_temp;
                dist_matrix_temp++;
                if (value < min_value) {
                    min_value = value;
                }
            }

            // subtract the smallest element from each element of the column
            dist_matrix_temp = dist_matrix + i * row;
            while (dist_matrix_temp < column_end) {
                *dist_matrix_temp -= min_value;
                dist_matrix_temp++;
            }
        }

        for (i = 0; i < col; i++) {
            for (j = 0; j < row; j++) {
                if (fabs(dist_matrix[j + i * row]) < SVP_TRACK_EPSILON) {
                    if (!covered_row[j]) {
                        star_matrix[j + i * row] = XMEDIA_TRUE;
                        covered_col[i] = XMEDIA_TRUE;
                        break;
                    }
                }
            }
        }
    }

    ret = hungarian_step2(assignment, dist_matrix, star_matrix, new_star_matrix, prime_matrix, covered_col, covered_row,
                            row, col, min_dim);
    if (ret != XMEDIA_SUCCESS) {
        CFREE(covered_col);
        CFREE(covered_row);
        CFREE(star_matrix);
        CFREE(prime_matrix);
        CFREE(new_star_matrix);
        CFREE(dist_matrix);
        SVP_TRACE(MODULE_DBG_ERR, "hungarian_step2 failed\n");
        return XMEDIA_FAILURE;
    }

    CFREE(covered_col);
    CFREE(covered_row);
    CFREE(star_matrix);
    CFREE(prime_matrix);
    CFREE(new_star_matrix);
    CFREE(dist_matrix);

    return XMEDIA_SUCCESS;
}

xmedia_s32 hungarian_matching(xmedia_float* iou_marix, xmedia_u32 row, xmedia_u32 col, xmedia_s32* assignment)
{
    // 无tracklet
    if (row == 0) {
        return XMEDIA_SUCCESS;
    }

    xmedia_float dist_matrix_in[row * col];
    xmedia_u32 i, j;
    xmedia_s32 ret;

    if ((iou_marix == XMEDIA_NULL) || (assignment == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            dist_matrix_in[i + row * j] = *(iou_marix + i * col + j);
        }
    }

    ret = assignment_optimal(assignment, dist_matrix_in, row, col);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "assignment_optimal failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 new_tracker_init(svp_bytetracker *tracker, xmedia_svp_tracker_single_result *result, svp_tracker_state state)
{
    xmedia_s32 ret;
    if ((tracker == XMEDIA_NULL) || (result == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    if (state > SVP_TRACKER_STATE_TRACKED) {
        SVP_TRACE(MODULE_DBG_ERR, "new tracker init get wrong state\n");
        return XMEDIA_FAILURE;
    }

    tracker->state = state;
    tracker->class_type = result->class_type;
    tracker->rect = result->rect;
    tracker->score = result->detect_score;
    tracker->classfier_score = result->classfier_score;
    tracker->lost_count = 0;
    tracker->age = 1;
    memcpy(tracker->kpt, result->kpt, sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
    ret = kalman_filter_init(&tracker->kalman_filter, tracker->rect);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "kalman_filter_init failed, ret=0x%x\n", ret);
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 set_tracker_id(svp_bytetracker* tracker, xmedia_s32* tracker_idx, xmedia_bool* id_flag)
{
    xmedia_u32 i;
    xmedia_s32 idx = *tracker_idx;

    if (tracker == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT; i++) {
        idx++;
        idx %= XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT;
        if (id_flag[idx] == XMEDIA_FALSE) {
            tracker->id = idx;
            id_flag[idx] = XMEDIA_TRUE;
            *tracker_idx = idx;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

/*
xmedia_s32 svp_bytetracker_set_attr(detect_param *param, const xmedia_svp_bytetracker_attr attr)
{
    if (param == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "svp_set_bytetracker_attr get null param:%p \n", param);
        return XMEDIA_FAILURE;
    }

    param->tracklet.enable = attr.enable;

    if (attr.enable == XMEDIA_FALSE) {
        return XMEDIA_SUCCESS;
    }
    if ((attr.high_score_thres <= 0) || (attr.high_score_thres >= 1)) {
        SVP_TRACE(MODULE_DBG_ERR, "high_score_thres threshold abnormal,Use default!\n");
    } else {
        param->tracklet.high_score_thres = attr.high_score_thres;
    }
    if ((attr.activated_tracker_thres <= 0) || (attr.activated_tracker_thres >= 1)) {
        SVP_TRACE(MODULE_DBG_ERR, "activated_tracker_thres threshold abnormal,Use default!\n");
    } else {
        param->tracklet.activated_tracker_thres = attr.activated_tracker_thres;
    }
    if ((attr.tracked_tracker_thres <= 0) || (attr.tracked_tracker_thres >= 1)) {
        SVP_TRACE(MODULE_DBG_ERR, "tracked_tracker_thres threshold abnormal,Use default!\n");
    } else {
        param->tracklet.tracked_tracker_thres = attr.tracked_tracker_thres;
    }
    if ((attr.unactivated_tracker_thres <= 0) || (attr.unactivated_tracker_thres >= 1)) {
        SVP_TRACE(MODULE_DBG_ERR, "unactivated_tracker_thres threshold abnormal,Use default!\n");
    } else {
        param->tracklet.unactivated_tracker_thres = attr.unactivated_tracker_thres;
    }
    if (attr.track_age_thres < 1) {
        SVP_TRACE(MODULE_DBG_ERR, "track_age_thres threshold abnormal,Use default!\n");
    } else {
        param->tracklet.track_age_thres = attr.track_age_thres;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_bytetracker_get_attr(detect_param *param, xmedia_svp_bytetracker_attr *attr)
{
    if (param == XMEDIA_NULL || attr == NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "svp_set_bytetracker_attr get null param:%p attr:%p\n", param, attr);
        return XMEDIA_FAILURE;
    }

    attr->enable = param->tracklet.enable;
    attr->high_score_thres = param->tracklet.high_score_thres;
    attr->activated_tracker_thres = param->tracklet.activated_tracker_thres;
    attr->tracked_tracker_thres = param->tracklet.tracked_tracker_thres;
    attr->unactivated_tracker_thres = param->tracklet.unactivated_tracker_thres;
    attr->track_age_thres = param->tracklet.track_age_thres;

    return XMEDIA_SUCCESS;
}
*/

xmedia_s32 svp_detect_bytetracker(xmedia_svp_tracker_result* result,svp_tracklet* tracklet,
                                            xmedia_s32* tracker_idx, xmedia_bool* id_flag)
{
    xmedia_float low_score_thresh = 0.01f;
    xmedia_float high_score_thresh = STD_MAX(low_score_thresh, tracklet->high_score_thres);
    svp_tracklet* unactivated_tracks; //new tracks 低优先级轨迹
    svp_tracklet* activated_tracks; //tracked & lost tracks 高优先级轨迹
    xmedia_svp_tracker_result high_score_rect = {0};
    xmedia_svp_tracker_result* remain_rect;
    xmedia_svp_tracker_single_result temp_bbox;
    xmedia_s32 ret;
    xmedia_u32 i,j;

    if ((tracklet->num == 0) && (result->target_num == 0)) {
        return XMEDIA_SUCCESS;
    }

    if ((tracklet->num == 0) && (result->target_num != 0)) {
        // 第一帧 所有框都建立新轨迹
        tracklet->num = result->target_num;
        for (i = 0; i < result->target_num; i++) {
            ret = new_tracker_init(&tracklet->tracker[i],
                                   &result->tracker_single[i],
                                    SVP_TRACKER_STATE_TRACKED);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "new_tracker_init failed\n");
                return XMEDIA_FAILURE;
            }
            ret = set_tracker_id(&tracklet->tracker[i], tracker_idx, id_flag);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "set_tracker_id failed\n");
                return XMEDIA_FAILURE;
            }
            result->tracker_single[i].tracker_id = tracklet->tracker[i].id;
            result->tracker_single[i].tracker_age = tracklet->tracker[i].age;
        }
        return XMEDIA_SUCCESS;
    }

    // 1、kalman预测
    for (i = 0; i < tracklet->num; i++) {
        ret = kalman_filter_predict(&tracklet->tracker[i], &tracklet->tracker[i].predicted_rect);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "kalman filter predict failed\n");
            return XMEDIA_FAILURE;
        }
    }

    activated_tracks = (svp_tracklet*)calloc(1, sizeof(svp_tracklet));
    activated_tracks->tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) * tracklet->num);
    CHECK_CALLOC_RETURN_ERROR(activated_tracks->tracker);
    CHECK_CALLOC_RETURN_ERROR(activated_tracks);

    if ((tracklet->num != 0) && (result->target_num == 0)) {
        for (i = 0; i < tracklet->num; i++) {
            if (tracklet->tracker[i].state == SVP_TRACKER_STATE_TRACKED) {
                tracklet->tracker[i].state = SVP_TRACKER_STATE_LOST;
                activated_tracks->tracker[activated_tracks->num] = tracklet->tracker[i];
                activated_tracks->num++;
            } else if ((tracklet->tracker[i].state == SVP_TRACKER_STATE_LOST) &&
                       (tracklet->tracker[i].lost_count < SVP_MAX_LOST_COUNT)) {
                activated_tracks->tracker[activated_tracks->num] = tracklet->tracker[i];
                activated_tracks->num++;
            } else if ((tracklet->tracker[i].state == SVP_TRACKER_STATE_LOST) &&
                       (tracklet->tracker[i].lost_count >= SVP_MAX_LOST_COUNT)) {
                id_flag[tracklet->tracker[i].id] = XMEDIA_FALSE;
            }
        }
        memset(tracklet->tracker, 0, sizeof(svp_bytetracker) * XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
        memcpy(tracklet->tracker, activated_tracks->tracker, sizeof(svp_bytetracker) * tracklet->num);
        tracklet->num = activated_tracks->num;
        CFREE(activated_tracks->tracker);
        CFREE(activated_tracks);
        return XMEDIA_SUCCESS;
    }

    unactivated_tracks = (svp_tracklet*)calloc(1, sizeof(svp_tracklet));
    unactivated_tracks->tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) * tracklet->num);
    CHECK_CALLOC_RETURN_ERROR(unactivated_tracks->tracker);
    CHECK_CALLOC_RETURN_ERROR(unactivated_tracks);

    // 2、检测结果分组
    for (i = 0; i < result->target_num; i++) {
        if (result->tracker_single[i].detect_score >= high_score_thresh) {
            high_score_rect.tracker_single[high_score_rect.target_num] = result->tracker_single[i];
            memcpy(high_score_rect.tracker_single[high_score_rect.target_num].kpt,
                   result->tracker_single[i].kpt, sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
            high_score_rect.target_num++;
        }
    }

    //tracklet 分类存放
    for (i = 0; i < tracklet->num; i++) {
        if ((tracklet->tracker[i].state == SVP_TRACKER_STATE_TRACKED) ||
            (tracklet->tracker[i].state == SVP_TRACKER_STATE_LOST)) {
            activated_tracks->tracker[activated_tracks->num] = tracklet->tracker[i];
            activated_tracks->num++;
        } else if (tracklet->tracker[i].state == SVP_TRACKER_STATE_NEW) {
            unactivated_tracks->tracker[unactivated_tracks->num] = tracklet->tracker[i];
            unactivated_tracks->num++;
        }
    }

    // 清空存放本帧结果
    memset(tracklet->tracker, 0, sizeof(svp_bytetracker) * XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    tracklet->num = 0;

    // 3 匹配
    // 3-1 高优先级轨迹与高分框匹配
    // 计算iou相似矩阵 high priority       tracker high score rect
    xmedia_float hh_iou_matrix[activated_tracks->num][high_score_rect.target_num];
    xmedia_s32 hh_assignment[activated_tracks->num]; // 下角标是第n个tracks，value是第n个目标
    for (i = 0; i < activated_tracks->num; i++) {
        hh_assignment[i] = -1;
    }

    for (i = 0; i < activated_tracks->num; i++) {
        temp_bbox.class_type = activated_tracks->tracker[i].class_type;
        temp_bbox.detect_score = activated_tracks->tracker[i].score;
        temp_bbox.rect = activated_tracks->tracker[i].predicted_rect;
        for (j = 0; j < high_score_rect.target_num; j++) {
            hh_iou_matrix[i][j] = 1.0 - iou_with_negative(&temp_bbox, &high_score_rect.tracker_single[j]) *
                                                                       high_score_rect.tracker_single[j].detect_score;
        }
    }

    if ((activated_tracks->num > 0) && (high_score_rect.target_num > 0)) {
        ret = hungarian_matching(&hh_iou_matrix[0][0], activated_tracks->num,
                                  high_score_rect.target_num, hh_assignment);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "hungarian matching failed\n");
            CFREE(unactivated_tracks->tracker);
            CFREE(activated_tracks->tracker);
            CFREE(activated_tracks);
            CFREE(unactivated_tracks);
            return XMEDIA_FAILURE;
        }
    }

    remain_rect = (xmedia_svp_tracker_result*)calloc(1, sizeof(xmedia_svp_tracker_result));
    CHECK_CALLOC_RETURN_ERROR(remain_rect);

    //处理高分组成功的匹配
    for (i = 0; i < activated_tracks->num; i++) {
        if ((hh_assignment[i] != -1) && ((1 - hh_iou_matrix[i][hh_assignment[i]] >
                                                tracklet->activated_tracker_thres))) {
            ret = kalman_filter_update(&activated_tracks->tracker[i],
                                       &high_score_rect.tracker_single[hh_assignment[i]].rect,
                                        high_score_rect.tracker_single[hh_assignment[i]].kpt);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "kalman filter update failed\n");
                CFREE(unactivated_tracks->tracker);
                CFREE(activated_tracks->tracker);
                CFREE(activated_tracks);
                CFREE(unactivated_tracks);
                CFREE(remain_rect);
                return XMEDIA_FAILURE;
            }
            tracklet->tracker[tracklet->num] = activated_tracks->tracker[i]; // 匹配成功的tracker
            tracklet->tracker[tracklet->num].score = high_score_rect.tracker_single[hh_assignment[i]].detect_score;
            tracklet->tracker[tracklet->num].classfier_score = 
                high_score_rect.tracker_single[hh_assignment[i]].classfier_score;
            high_score_rect.tracker_single[hh_assignment[i]].detect_score = 0.0f;
            tracklet->tracker[tracklet->num].iou = (1 - hh_iou_matrix[i][hh_assignment[i]]) /
                                                    tracklet->tracker[tracklet->num].score;
            tracklet->num++;
        } else {
            if (activated_tracks->tracker[i].state == SVP_TRACKER_STATE_TRACKED) {
                activated_tracks->tracker[i].state = SVP_TRACKER_STATE_LOST;
            }
            if ((activated_tracks->tracker[i].state == SVP_TRACKER_STATE_LOST) &&
                (activated_tracks->tracker[i].lost_count < SVP_MAX_LOST_COUNT)) {
                tracklet->tracker[tracklet->num] = activated_tracks->tracker[i]; // 匹配不成功的lost
                tracklet->num++;
            } else {
                id_flag[activated_tracks->tracker[i].id] = XMEDIA_FALSE;
            }
        }
    }

    // 记录未匹配到的高分框
    for (i = 0; i < high_score_rect.target_num; i++) {
        if (high_score_rect.tracker_single[i].detect_score >= high_score_thresh) {
            // 剩余高分框 第三次匹配用到
            remain_rect->tracker_single[remain_rect->target_num] = high_score_rect.tracker_single[i];
            remain_rect->target_num++;
        }
    }

    // 3-2 低优先级轨迹与高分框
    // low priority tracker & high score rect
    xmedia_float lh_iou_matrix[unactivated_tracks->num][remain_rect->target_num];
    xmedia_s32 lh_assignment[unactivated_tracks->num];
    for (i = 0; i < unactivated_tracks->num; i++) {
        lh_assignment[i] = -1;
    }
    for (i = 0; i < unactivated_tracks->num; i++) {
        temp_bbox.class_type = unactivated_tracks->tracker[i].class_type;
        temp_bbox.detect_score = unactivated_tracks->tracker[i].score;
        temp_bbox.rect = unactivated_tracks->tracker[i].predicted_rect;
        for (j = 0; j < remain_rect->target_num; j++) {
            lh_iou_matrix[i][j] = 1.0 - iou_with_negative(&temp_bbox, &remain_rect->tracker_single[j]) *
                                                          remain_rect->tracker_single[j].detect_score;
        }
    }

    if ((unactivated_tracks->num > 0) && (remain_rect->target_num > 0)) {
        ret = hungarian_matching(&lh_iou_matrix[0][0], unactivated_tracks->num, remain_rect->target_num, lh_assignment);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "hungarian matching update failed\n");
            CFREE(unactivated_tracks->tracker);
            CFREE(activated_tracks->tracker);
            CFREE(activated_tracks);
            CFREE(unactivated_tracks);
            CFREE(remain_rect);
            return XMEDIA_FAILURE;
        }
    }

    //处理成功的匹配
    for (i = 0; i < unactivated_tracks->num; i++) {
        if ((lh_assignment[i] != -1) && ((1 - lh_iou_matrix[i][lh_assignment[i]] >
                                            tracklet->unactivated_tracker_thres))) {
            ret = kalman_filter_update(&unactivated_tracks->tracker[i],
                                       &remain_rect->tracker_single[lh_assignment[i]].rect,
                                        remain_rect->tracker_single[lh_assignment[i]].kpt);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "kalman filter update update failed\n");
                CFREE(unactivated_tracks->tracker);
                CFREE(activated_tracks->tracker);
                CFREE(activated_tracks);
                CFREE(unactivated_tracks);
                CFREE(remain_rect);
                return XMEDIA_FAILURE;
            }
            tracklet->tracker[tracklet->num] = unactivated_tracks->tracker[i]; // 匹配成功的tracklet 加进最终结果中
            tracklet->tracker[tracklet->num].score = remain_rect->tracker_single[lh_assignment[i]].detect_score;
            tracklet->tracker[tracklet->num].classfier_score =
                remain_rect->tracker_single[lh_assignment[i]].classfier_score;
            remain_rect->tracker_single[lh_assignment[i]].detect_score = 0.0f;
            tracklet->tracker[tracklet->num].iou = (1 - lh_iou_matrix[i][lh_assignment[i]]) /
                                                    tracklet->tracker[tracklet->num].score;
            ret = set_tracker_id(&tracklet->tracker[tracklet->num], tracker_idx, id_flag);
            if (ret != XMEDIA_SUCCESS) {
                CFREE(unactivated_tracks->tracker);
                CFREE(activated_tracks->tracker);
                CFREE(activated_tracks);
                CFREE(unactivated_tracks);
                CFREE(remain_rect);
                SVP_TRACE(MODULE_DBG_ERR, "set_tracker_id failed\n");
                return XMEDIA_FAILURE;
            }
            tracklet->num++;
        }
    }

    memset(&high_score_rect, 0, sizeof(xmedia_svp_tracker_result));
    for (i = 0; i < remain_rect->target_num; i++) {
        if (remain_rect->tracker_single[i].detect_score >= high_score_thresh) {
            // 剩余高分框 建立新轨迹用到
            high_score_rect.tracker_single[high_score_rect.target_num] = remain_rect->tracker_single[i];
            high_score_rect.target_num++;
        }
    }

    // 4 建立新轨迹
    for (i = 0; i < high_score_rect.target_num; i++) {
        ret = new_tracker_init(&tracklet->tracker[tracklet->num],
                               &high_score_rect.tracker_single[i],
                                SVP_TRACKER_STATE_NEW);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "new tracker init failed\n");
            CFREE(unactivated_tracks->tracker);
            CFREE(activated_tracks->tracker);
            CFREE(activated_tracks);
            CFREE(unactivated_tracks);
            CFREE(remain_rect);
            return XMEDIA_FAILURE;
        }
        tracklet->num++;
    }

    //最后 将追踪结果赋值给result
    memset(result, 0, sizeof(xmedia_svp_tracker_result));
    for (i = 0; i < tracklet->num; i++) {
        if ((tracklet->tracker[i].state == SVP_TRACKER_STATE_TRACKED) &&
            (tracklet->tracker[i].age >= tracklet->track_age_thres)) {
            result->tracker_single[result->target_num].class_type = tracklet->tracker[i].class_type;
            result->tracker_single[result->target_num].detect_score = tracklet->tracker[i].score;
            result->tracker_single[result->target_num].classfier_score = tracklet->tracker[i].classfier_score;
            result->tracker_single[result->target_num].rect = tracklet->tracker[i].rect;
            result->tracker_single[result->target_num].tracker_id = tracklet->tracker[i].id;
            result->tracker_single[result->target_num].tracker_age = tracklet->tracker[i].age;
            memcpy(result->tracker_single[result->target_num].kpt, tracklet->tracker[i].kpt,
                sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
            result->target_num++;
        }
    }

    CFREE(unactivated_tracks->tracker);
    CFREE(activated_tracks->tracker);
    CFREE(unactivated_tracks);
    CFREE(activated_tracks);
    CFREE(remain_rect);

    return XMEDIA_SUCCESS;
}

