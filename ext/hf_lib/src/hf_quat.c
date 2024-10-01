#include "../include/hf_quat.h"

#include <string.h>
#include <math.h>

void hf_quat_copy(hf_quat quat, hf_quat out) {
    memcpy(out, quat, sizeof(float) * 4);
}

void hf_quat_normalize(hf_quat quat, hf_quat out) {
    const float len = sqrtf(
        quat[0] * quat[0] +
        quat[1] * quat[1] +
        quat[2] * quat[2] +
        quat[3] * quat[3]
    );

    out[0] = quat[0] / len;
    out[1] = quat[1] / len;
    out[2] = quat[2] / len;
    out[3] = quat[3] / len;
}
