#include "../include/hf_mat.h"

#include <string.h>

void hf_mat1x2f_copy(hf_mat1x2f mat, hf_mat1x2f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 2);
}

void hf_mat1x2f_transpose(hf_mat1x2f mat, hf_mat2x1f out) {
	hf_mat2x1f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 2; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat1x2f_add(hf_mat1x2f a, hf_mat1x2f b, hf_mat1x2f out) {
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat1x2f_multiply(hf_mat1x2f mat, float scalar, hf_mat1x2f out) {
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat1x2f_multiply_mat2f(hf_mat1x2f a, hf_mat2f b, hf_mat1x2f out) {
	hf_mat1x2f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat1x2f_multiply_mat2x3f(hf_mat1x2f a, hf_mat2x3f b, hf_mat1x3f out) {
	hf_mat1x3f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat1x2f_multiply_mat2x4f(hf_mat1x2f a, hf_mat2x4f b, hf_mat1x4f out) {
	hf_mat1x4f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat1x3f_copy(hf_mat1x3f mat, hf_mat1x3f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 3);
}

void hf_mat1x3f_transpose(hf_mat1x3f mat, hf_mat3x1f out) {
	hf_mat3x1f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat1x3f_add(hf_mat1x3f a, hf_mat1x3f b, hf_mat1x3f out) {
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat1x3f_multiply(hf_mat1x3f mat, float scalar, hf_mat1x3f out) {
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat1x3f_multiply_mat3x2f(hf_mat1x3f a, hf_mat3x2f b, hf_mat1x2f out) {
	hf_mat1x2f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat1x3f_multiply_mat3f(hf_mat1x3f a, hf_mat3f b, hf_mat1x3f out) {
	hf_mat1x3f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat1x3f_multiply_mat3x4f(hf_mat1x3f a, hf_mat3x4f b, hf_mat1x4f out) {
	hf_mat1x4f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat1x4f_copy(hf_mat1x4f mat, hf_mat1x4f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 4);
}

void hf_mat1x4f_transpose(hf_mat1x4f mat, hf_mat4x1f out) {
	hf_mat4x1f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 4; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat1x4f_add(hf_mat1x4f a, hf_mat1x4f b, hf_mat1x4f out) {
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat1x4f_multiply(hf_mat1x4f mat, float scalar, hf_mat1x4f out) {
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat1x4f_multiply_mat4x2f(hf_mat1x4f a, hf_mat4x2f b, hf_mat1x2f out) {
	hf_mat1x2f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat1x4f_multiply_mat4x3f(hf_mat1x4f a, hf_mat4x3f b, hf_mat1x3f out) {
	hf_mat1x3f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat1x4f_multiply_mat4f(hf_mat1x4f a, hf_mat4f b, hf_mat1x4f out) {
	hf_mat1x4f tmp;
	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat2x1f_copy(hf_mat2x1f mat, hf_mat2x1f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 2);
}

void hf_mat2x1f_transpose(hf_mat2x1f mat, hf_mat1x2f out) {
	hf_mat1x2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat2x1f_add(hf_mat2x1f a, hf_mat2x1f b, hf_mat2x1f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat2x1f_multiply(hf_mat2x1f mat, float scalar, hf_mat2x1f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat2x1f_multiply_mat1x2f(hf_mat2x1f a, hf_mat1x2f b, hf_mat2f out) {
	hf_mat2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat2x1f_multiply_mat1x3f(hf_mat2x1f a, hf_mat1x3f b, hf_mat2x3f out) {
	hf_mat2x3f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat2x1f_multiply_mat1x4f(hf_mat2x1f a, hf_mat1x4f b, hf_mat2x4f out) {
	hf_mat2x4f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat2f_copy(hf_mat2f mat, hf_mat2f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 4);
}

void hf_mat2f_identity(hf_mat2f out) {
	float values[] = {
		1.f, 0.f,
		0.f, 1.f,
	};
	memcpy(out, values, sizeof(out[0][0]) * 4);
}

void hf_mat2f_transpose(hf_mat2f mat, hf_mat2f out) {
	hf_mat2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

float hf_mat2f_determinant(hf_mat2f mat) {

	return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}

float hf_mat2f_minor(hf_mat2f mat, int i, int j) {
	return mat[1 - i][1 - j];
}

void hf_mat2f_inverse(hf_mat2f mat, hf_mat2f out) {
	float det = hf_mat2f_determinant(mat);
	if(det == 0.0f) {
		return;
	}
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			out[j][i] = ((i + j) % 2 == 0 ? 1.f : -1.f) * hf_mat2f_minor(mat, i, j);
		}
	}
	hf_mat2f_multiply(out, 1.f / det, out);
}

void hf_mat2f_add(hf_mat2f a, hf_mat2f b, hf_mat2f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat2f_multiply(hf_mat2f mat, float scalar, hf_mat2f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat2f_multiply_mat2x1f(hf_mat2f a, hf_mat2x1f b, hf_mat2x1f out) {
	hf_mat2x1f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat2f_multiply_mat2f(hf_mat2f a, hf_mat2f b, hf_mat2f out) {
	hf_mat2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat2f_multiply_mat2x3f(hf_mat2f a, hf_mat2x3f b, hf_mat2x3f out) {
	hf_mat2x3f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat2f_multiply_mat2x4f(hf_mat2f a, hf_mat2x4f b, hf_mat2x4f out) {
	hf_mat2x4f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat2x3f_copy(hf_mat2x3f mat, hf_mat2x3f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 6);
}

void hf_mat2x3f_transpose(hf_mat2x3f mat, hf_mat3x2f out) {
	hf_mat3x2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat2x3f_add(hf_mat2x3f a, hf_mat2x3f b, hf_mat2x3f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat2x3f_multiply(hf_mat2x3f mat, float scalar, hf_mat2x3f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat2x3f_multiply_mat3x1f(hf_mat2x3f a, hf_mat3x1f b, hf_mat2x1f out) {
	hf_mat2x1f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat2x3f_multiply_mat3x2f(hf_mat2x3f a, hf_mat3x2f b, hf_mat2f out) {
	hf_mat2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat2x3f_multiply_mat3f(hf_mat2x3f a, hf_mat3f b, hf_mat2x3f out) {
	hf_mat2x3f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat2x3f_multiply_mat3x4f(hf_mat2x3f a, hf_mat3x4f b, hf_mat2x4f out) {
	hf_mat2x4f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat2x4f_copy(hf_mat2x4f mat, hf_mat2x4f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 8);
}

void hf_mat2x4f_transpose(hf_mat2x4f mat, hf_mat4x2f out) {
	hf_mat4x2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat2x4f_add(hf_mat2x4f a, hf_mat2x4f b, hf_mat2x4f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat2x4f_multiply(hf_mat2x4f mat, float scalar, hf_mat2x4f out) {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat2x4f_multiply_mat4x1f(hf_mat2x4f a, hf_mat4x1f b, hf_mat2x1f out) {
	hf_mat2x1f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 2);
}

void hf_mat2x4f_multiply_mat4x2f(hf_mat2x4f a, hf_mat4x2f b, hf_mat2f out) {
	hf_mat2f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat2x4f_multiply_mat4x3f(hf_mat2x4f a, hf_mat4x3f b, hf_mat2x3f out) {
	hf_mat2x3f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat2x4f_multiply_mat4f(hf_mat2x4f a, hf_mat4f b, hf_mat2x4f out) {
	hf_mat2x4f tmp;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat3x1f_copy(hf_mat3x1f mat, hf_mat3x1f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 3);
}

void hf_mat3x1f_transpose(hf_mat3x1f mat, hf_mat1x3f out) {
	hf_mat1x3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat3x1f_add(hf_mat3x1f a, hf_mat3x1f b, hf_mat3x1f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat3x1f_multiply(hf_mat3x1f mat, float scalar, hf_mat3x1f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat3x1f_multiply_mat1x2f(hf_mat3x1f a, hf_mat1x2f b, hf_mat3x2f out) {
	hf_mat3x2f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat3x1f_multiply_mat1x3f(hf_mat3x1f a, hf_mat1x3f b, hf_mat3f out) {
	hf_mat3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 9);
}

void hf_mat3x1f_multiply_mat1x4f(hf_mat3x1f a, hf_mat1x4f b, hf_mat3x4f out) {
	hf_mat3x4f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat3x2f_copy(hf_mat3x2f mat, hf_mat3x2f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 6);
}

void hf_mat3x2f_transpose(hf_mat3x2f mat, hf_mat2x3f out) {
	hf_mat2x3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat3x2f_add(hf_mat3x2f a, hf_mat3x2f b, hf_mat3x2f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat3x2f_multiply(hf_mat3x2f mat, float scalar, hf_mat3x2f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat3x2f_multiply_mat2x1f(hf_mat3x2f a, hf_mat2x1f b, hf_mat3x1f out) {
	hf_mat3x1f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat3x2f_multiply_mat2f(hf_mat3x2f a, hf_mat2f b, hf_mat3x2f out) {
	hf_mat3x2f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat3x2f_multiply_mat2x3f(hf_mat3x2f a, hf_mat2x3f b, hf_mat3f out) {
	hf_mat3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 9);
}

void hf_mat3x2f_multiply_mat2x4f(hf_mat3x2f a, hf_mat2x4f b, hf_mat3x4f out) {
	hf_mat3x4f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat3f_copy(hf_mat3f mat, hf_mat3f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 9);
}

void hf_mat3f_identity(hf_mat3f out) {
	float values[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
	};
	memcpy(out, values, sizeof(out[0][0]) * 9);
}

void hf_mat3f_transpose(hf_mat3f mat, hf_mat3f out) {
	hf_mat3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 9);
}

float hf_mat3f_determinant(hf_mat3f mat) {
	hf_mat2f mat_0, mat_1, mat_2;
	memcpy(&mat_0[0][0], &mat[1][1], sizeof(mat[0][0]) * 2);
	memcpy(&mat_0[1][0], &mat[2][1], sizeof(mat[0][0]) * 2);
	memcpy(&mat_1[0][0], &mat[0][1], sizeof(mat[0][0]) * 2);
	memcpy(&mat_1[1][0], &mat[2][1], sizeof(mat[0][0]) * 2);
	memcpy(&mat_2[0][0], &mat[0][1], sizeof(mat[0][0]) * 2);
	memcpy(&mat_2[1][0], &mat[1][1], sizeof(mat[0][0]) * 2);
	return
		+(mat[0][0] * hf_mat2f_determinant(mat_0))
		-(mat[1][0] * hf_mat2f_determinant(mat_1))
		+(mat[2][0] * hf_mat2f_determinant(mat_2))
	;
}

float hf_mat3f_minor(hf_mat3f mat, int i, int j) {
	hf_mat2f mat_sub;
	int row = 0;
	for(int k = 0; k < 3; k++) {
		if(k == i) {
			continue;
		}
		int col = 0;
		for(int l = 0; l < 3; l++) {
			if(l == j) {
				continue;
			}
			mat_sub[row][col] = mat[k][l];
			col++;
		}
		row++;
	}
	return hf_mat2f_determinant(mat_sub);
}

void hf_mat3f_inverse(hf_mat3f mat, hf_mat3f out) {
	float det = hf_mat3f_determinant(mat);
	if(det == 0.0f) {
		return;
	}
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			out[j][i] = ((i + j) % 2 == 0 ? 1.f : -1.f) * hf_mat3f_minor(mat, i, j);
		}
	}
	hf_mat3f_multiply(out, 1.f / det, out);
}

void hf_mat3f_add(hf_mat3f a, hf_mat3f b, hf_mat3f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat3f_multiply(hf_mat3f mat, float scalar, hf_mat3f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat3f_multiply_mat3x1f(hf_mat3f a, hf_mat3x1f b, hf_mat3x1f out) {
	hf_mat3x1f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat3f_multiply_mat3x2f(hf_mat3f a, hf_mat3x2f b, hf_mat3x2f out) {
	hf_mat3x2f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat3f_multiply_mat3f(hf_mat3f a, hf_mat3f b, hf_mat3f out) {
	hf_mat3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 9);
}

void hf_mat3f_multiply_mat3x4f(hf_mat3f a, hf_mat3x4f b, hf_mat3x4f out) {
	hf_mat3x4f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat3x4f_copy(hf_mat3x4f mat, hf_mat3x4f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 12);
}

void hf_mat3x4f_transpose(hf_mat3x4f mat, hf_mat4x3f out) {
	hf_mat4x3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat3x4f_add(hf_mat3x4f a, hf_mat3x4f b, hf_mat3x4f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat3x4f_multiply(hf_mat3x4f mat, float scalar, hf_mat3x4f out) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat3x4f_multiply_mat4x1f(hf_mat3x4f a, hf_mat4x1f b, hf_mat3x1f out) {
	hf_mat3x1f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 3);
}

void hf_mat3x4f_multiply_mat4x2f(hf_mat3x4f a, hf_mat4x2f b, hf_mat3x2f out) {
	hf_mat3x2f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 6);
}

void hf_mat3x4f_multiply_mat4x3f(hf_mat3x4f a, hf_mat4x3f b, hf_mat3f out) {
	hf_mat3f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 9);
}

void hf_mat3x4f_multiply_mat4f(hf_mat3x4f a, hf_mat4f b, hf_mat3x4f out) {
	hf_mat3x4f tmp;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat4x1f_copy(hf_mat4x1f mat, hf_mat4x1f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 4);
}

void hf_mat4x1f_transpose(hf_mat4x1f mat, hf_mat1x4f out) {
	hf_mat1x4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 1; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat4x1f_add(hf_mat4x1f a, hf_mat4x1f b, hf_mat4x1f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 1; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat4x1f_multiply(hf_mat4x1f mat, float scalar, hf_mat4x1f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 1; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat4x1f_multiply_mat1x2f(hf_mat4x1f a, hf_mat1x2f b, hf_mat4x2f out) {
	hf_mat4x2f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat4x1f_multiply_mat1x3f(hf_mat4x1f a, hf_mat1x3f b, hf_mat4x3f out) {
	hf_mat4x3f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat4x1f_multiply_mat1x4f(hf_mat4x1f a, hf_mat1x4f b, hf_mat4f out) {
	hf_mat4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 1; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 16);
}

void hf_mat4x2f_copy(hf_mat4x2f mat, hf_mat4x2f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 8);
}

void hf_mat4x2f_transpose(hf_mat4x2f mat, hf_mat2x4f out) {
	hf_mat2x4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat4x2f_add(hf_mat4x2f a, hf_mat4x2f b, hf_mat4x2f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat4x2f_multiply(hf_mat4x2f mat, float scalar, hf_mat4x2f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat4x2f_multiply_mat2x1f(hf_mat4x2f a, hf_mat2x1f b, hf_mat4x1f out) {
	hf_mat4x1f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat4x2f_multiply_mat2f(hf_mat4x2f a, hf_mat2f b, hf_mat4x2f out) {
	hf_mat4x2f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat4x2f_multiply_mat2x3f(hf_mat4x2f a, hf_mat2x3f b, hf_mat4x3f out) {
	hf_mat4x3f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat4x2f_multiply_mat2x4f(hf_mat4x2f a, hf_mat2x4f b, hf_mat4f out) {
	hf_mat4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 2; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 16);
}

void hf_mat4x3f_copy(hf_mat4x3f mat, hf_mat4x3f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 12);
}

void hf_mat4x3f_transpose(hf_mat4x3f mat, hf_mat3x4f out) {
	hf_mat3x4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat4x3f_add(hf_mat4x3f a, hf_mat4x3f b, hf_mat4x3f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat4x3f_multiply(hf_mat4x3f mat, float scalar, hf_mat4x3f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat4x3f_multiply_mat3x1f(hf_mat4x3f a, hf_mat3x1f b, hf_mat4x1f out) {
	hf_mat4x1f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat4x3f_multiply_mat3x2f(hf_mat4x3f a, hf_mat3x2f b, hf_mat4x2f out) {
	hf_mat4x2f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat4x3f_multiply_mat3f(hf_mat4x3f a, hf_mat3f b, hf_mat4x3f out) {
	hf_mat4x3f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat4x3f_multiply_mat3x4f(hf_mat4x3f a, hf_mat3x4f b, hf_mat4f out) {
	hf_mat4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 3; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 16);
}

void hf_mat4f_copy(hf_mat4f mat, hf_mat4f out) {
	memcpy(out, mat, sizeof(out[0][0]) * 16);
}

void hf_mat4f_identity(hf_mat4f out) {
	float values[] = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
	memcpy(out, values, sizeof(out[0][0]) * 16);
}

void hf_mat4f_transpose(hf_mat4f mat, hf_mat4f out) {
	hf_mat4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			tmp[i][j] = mat[j][i];
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 16);
}

float hf_mat4f_determinant(hf_mat4f mat) {
	hf_mat3f mat_0, mat_1, mat_2, mat_3;
	memcpy(&mat_0[0][0], &mat[1][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_0[1][0], &mat[2][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_0[2][0], &mat[3][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_1[0][0], &mat[0][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_1[1][0], &mat[2][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_1[2][0], &mat[3][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_2[0][0], &mat[0][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_2[1][0], &mat[1][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_2[2][0], &mat[3][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_3[0][0], &mat[0][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_3[1][0], &mat[1][1], sizeof(mat[0][0]) * 3);
	memcpy(&mat_3[2][0], &mat[2][1], sizeof(mat[0][0]) * 3);
	return
		+(mat[0][0] * hf_mat3f_determinant(mat_0))
		-(mat[1][0] * hf_mat3f_determinant(mat_1))
		+(mat[2][0] * hf_mat3f_determinant(mat_2))
		-(mat[3][0] * hf_mat3f_determinant(mat_3))
	;
}

float hf_mat4f_minor(hf_mat4f mat, int i, int j) {
	hf_mat3f mat_sub;
	int row = 0;
	for(int k = 0; k < 4; k++) {
		if(k == i) {
			continue;
		}
		int col = 0;
		for(int l = 0; l < 4; l++) {
			if(l == j) {
				continue;
			}
			mat_sub[row][col] = mat[k][l];
			col++;
		}
		row++;
	}
	return hf_mat3f_determinant(mat_sub);
}

void hf_mat4f_inverse(hf_mat4f mat, hf_mat4f out) {
	float det = hf_mat4f_determinant(mat);
	if(det == 0.0f) {
		return;
	}
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			out[j][i] = ((i + j) % 2 == 0 ? 1.f : -1.f) * hf_mat4f_minor(mat, i, j);
		}
	}
	hf_mat4f_multiply(out, 1.f / det, out);
}

void hf_mat4f_add(hf_mat4f a, hf_mat4f b, hf_mat4f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

void hf_mat4f_multiply(hf_mat4f mat, float scalar, hf_mat4f out) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			out[i][j] = mat[i][j] * scalar;
		}
	}
}

void hf_mat4f_multiply_mat4x1f(hf_mat4f a, hf_mat4x1f b, hf_mat4x1f out) {
	hf_mat4x1f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 1; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 4);
}

void hf_mat4f_multiply_mat4x2f(hf_mat4f a, hf_mat4x2f b, hf_mat4x2f out) {
	hf_mat4x2f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 2; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 8);
}

void hf_mat4f_multiply_mat4x3f(hf_mat4f a, hf_mat4x3f b, hf_mat4x3f out) {
	hf_mat4x3f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 12);
}

void hf_mat4f_multiply_mat4f(hf_mat4f a, hf_mat4f b, hf_mat4f out) {
	hf_mat4f tmp;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			float val = 0.f;
			for(int k = 0; k < 4; k++) {
				val += a[i][k] * b[k][j];
			}
			tmp[i][j] = val;
		}
	}
	memcpy(out, tmp, sizeof(out[0][0]) * 16);
}
