#ifndef HF_QUAT_H
#define HF_QUAT_H

typedef float hf_quat[4];

void hf_quat_copy(hf_quat quat, hf_quat out);
void hf_quat_normalize(hf_quat quat, hf_quat out);

#endif//HF_QUAT_H
