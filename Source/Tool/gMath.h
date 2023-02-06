/**
 * @file gMath.h
 * @brief �v�Z���C�u����
 * @author ���J�C�A��
 * @date 2022/07/06
 * @���� 2022/07/06�F�t�@�C���쐬
 *		 2022/07/17�FSIMD���Z�����E����
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <cmath>
#include <numbers>
#include <DirectXMath.h>
#include <WinApp/WinEO.h>

//===== �萔�E�}�N����` =====
constexpr float fPI = std::numbers::pi_v<float>;
constexpr double dPI = std::numbers::pi;			//�~����
constexpr float f2PI = fPI * 2.0f;
constexpr double d2PI = dPI * 2.0;					//360�x���W�A��
constexpr float fGravity = 9.80665f;
constexpr double dGravity = 9.80665;				//�d�͉����x

//===== �N���X�錾 =====

//***** �x�N�g�� *****
class VECTOR2
{
public:

	//�ϐ��錾
	float x;
	float y;

	//���Z�q�I�[�o�[���[�h
	bool operator==(const VECTOR2 Data) const;		//��r
	VECTOR2 operator+(const VECTOR2 Data) const;	//���Z
	VECTOR2& operator+=(const VECTOR2 Data);
	VECTOR2 operator-(const VECTOR2 Data) const;	//���Z
	VECTOR2& operator-=(const VECTOR2 Data);
	VECTOR2 operator*(const float Data) const;		//��Z
	VECTOR2& operator*=(const float Data);

	//�v���g�^�C�v�錾
	explicit VECTOR2() noexcept;
	explicit VECTOR2(float xIn, float yIn) noexcept;
	~VECTOR2() noexcept;
};

class VECTOR3
{
public:

	//�ϐ��錾
	float x;
	float y;
	float z;

	//���Z�q�I�[�o�[���[�h
	bool operator==(const VECTOR3 Data) const;		//��r
	VECTOR3 operator+(const VECTOR3 Data) const;	//���Z
	VECTOR3& operator+=(const VECTOR3 Data);
	VECTOR3 operator-(const VECTOR3 Data) const;	//���Z
	VECTOR3& operator-=(const VECTOR3 Data);
	VECTOR3 operator*(const float Data) const;		//��Z
	VECTOR3& operator*=(const float Data);

	//�v���g�^�C�v�錾
	explicit VECTOR3() noexcept;
	explicit VECTOR3(float xIn, float yIn, float zIn) noexcept;
	~VECTOR3() noexcept;
};

//===== �v���g�^�C�v�錾 =====
namespace gMath
{
	//��悷��
	inline float Square(const float x) noexcept
	{
		return std::powf(x, 2.0f);
	}

	//�p�x�ϊ�
	inline float GetRad(int Angle) noexcept
	{
		return (fPI * Angle / 180);
	}
	inline float GetRad(float Angle) noexcept
	{
		return (fPI * Angle / 180);
	}
	inline int GetDegree(float Radian) noexcept
	{
		return static_cast<int>(Radian * 180 / fPI);
	}
	inline float GetDegree_f(float Radian) noexcept
	{
		return Radian * 180 / fPI;
	}

	//�x�N�g�������擾
	inline float VecLength(VECTOR2 Vector) noexcept
	{
		return std::sqrtf(Square(Vector.x) + Square(Vector.y));
	}
	inline float VecLength(VECTOR3 Vector) noexcept
	{
		return std::sqrtf(Square(Vector.x) + Square(Vector.y) + Square(Vector.z));
	}

	//�x�N�g�����K��
	inline VECTOR2 VecNormalize(VECTOR2 Vector, bool bAlarm = true)
	{
		//�ϐ��錾
		VECTOR2 Vec = Vector;
		float VecLen = VecLength(Vec);

		//��O����
		if (VecLen == 0.0f && bAlarm)
			throw ERROR_EX2("�x�N�g���̒�����0�ł�!");

		//�v�Z
		if (VecLen != 0.0f) {
			Vec.x /= VecLen;
			Vec.y /= VecLen;
		}

		//�߂�l
		return Vec;
	}
	inline VECTOR3 VecNormalize(VECTOR3 Vector, bool bAlarm = true)
	{
		//�ϐ��錾
		VECTOR3 Vec = Vector;
		float VecLen = VecLength(Vec);

		//��O����
		if (VecLen == 0.0f && bAlarm)
			throw ERROR_EX2("�x�N�g���̒�����0�ł�!");

		//�v�Z
		if (VecLen != 0.0f) {
			Vec.x /= VecLen;
			Vec.y /= VecLen;
			Vec.z /= VecLen;
		}

		//�߂�l
		return Vec;
	}

	//����
	inline float VecDot(VECTOR2 Vector1, VECTOR2 Vector2) noexcept
	{
		return Vector1.x * Vector2.x + Vector1.y * Vector2.y;
	}
	inline float VecDot(VECTOR3 Vector1, VECTOR3 Vector2) noexcept
	{
		return Vector1.x * Vector2.x + Vector1.y * Vector2.y + Vector1.z * Vector2.z;
	}

	//�O��
	inline float VecCross(VECTOR2 Vector1, VECTOR2 Vector2) noexcept
	{
		return Vector1.x * Vector2.y - Vector1.y * Vector2.x;
	}
	inline VECTOR3 VecCross(VECTOR3 Vector1, VECTOR3 Vector2) noexcept
	{
		//�ϐ��錾
		VECTOR3 Vec{};

		//�v�Z
		Vec.x = Vector1.y * Vector2.z - Vector1.z * Vector2.y;
		Vec.y = Vector1.z * Vector2.x - Vector1.x * Vector2.z;
		Vec.z = Vector1.x * Vector2.y - Vector1.y * Vector2.x;

		//�߂�l
		return Vec;
	}

	//�]�u�s��
	inline void MtxTranspose4x4_SSE(float* mtxA, float* mtxB = nullptr) noexcept
	{
		__m128 row1 = _mm_load_ps(mtxA + 0 * 4);
		__m128 row2 = _mm_load_ps(mtxA + 1 * 4);
		__m128 row3 = _mm_load_ps(mtxA + 2 * 4);
		__m128 row4 = _mm_load_ps(mtxA + 3 * 4);
		_MM_TRANSPOSE4_PS(row1, row2, row3, row4);

		//���ʂ�Ԃ�
		if (mtxB != nullptr) {
			_mm_store_ps((mtxB + 0 * 4), row1);
			_mm_store_ps((mtxB + 1 * 4), row2);
			_mm_store_ps((mtxB + 2 * 4), row3);
			_mm_store_ps((mtxB + 3 * 4), row4);
		}
		else {
			_mm_store_ps((mtxA + 0 * 4), row1);
			_mm_store_ps((mtxA + 1 * 4), row2);
			_mm_store_ps((mtxA + 2 * 4), row3);
			_mm_store_ps((mtxA + 3 * 4), row4);
		}
	}

	//�x�N�g�� X �s��
	inline void Vec4Transform_AVX(float* pVec, float* pMtx) noexcept
	{
		//�s��Z�b�g
		__m256 row01 = _mm256_load_ps(pMtx + 0 * 8);
		__m256 row23 = _mm256_load_ps(pMtx + 1 * 8);

		//�x�N�g���Z�b�g�i�Z�b�g�֐��͋t���Ŋi�[�����j
		float* pVecTemp = pVec;
		__m256 multFactor01 = _mm256_set_ps(*(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);
		pVecTemp += 2;
		__m256 multFactor23 = _mm256_set_ps(*(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);

		//�s��ƃx�N�g���̊|���Z
		row01 = _mm256_mul_ps(row01, multFactor01);
		row23 = _mm256_mul_ps(row23, multFactor23);		//��Z
		__m256 result = _mm256_add_ps(row01, row23);	//���Z

		//���ʂ�Ԃ�
		pVecTemp = pVec;
		*pVecTemp = result.m256_f32[0] + result.m256_f32[4];
		pVecTemp++;
		*pVecTemp = result.m256_f32[1] + result.m256_f32[5];
		pVecTemp++;
		*pVecTemp = result.m256_f32[2] + result.m256_f32[6];
		pVecTemp++;
		*pVecTemp = result.m256_f32[3] + result.m256_f32[7];
	}
	inline void Vec3Transform_AVX(float* pVec, float* pMtx) noexcept
	{
		//�s��Z�b�g
		__m256 row01 = _mm256_load_ps(pMtx + 0 * 8);
		__m256 row23 = _mm256_load_ps(pMtx + 1 * 8);

		//�x�N�g���Z�b�g�i�Z�b�g�֐��͋t���Ŋi�[�����j
		float* pVecTemp = pVec;
		__m256 multFactor01 = _mm256_set_ps(*(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);
		pVecTemp += 2;
		__m256 multFactor23 = _mm256_set_ps(1.0f, 1.0f, 1.0f, 1.0f, *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);

		//�s��ƃx�N�g���̊|���Z
		row01 = _mm256_mul_ps(row01, multFactor01);
		row23 = _mm256_mul_ps(row23, multFactor23);		//��Z
		__m256 result = _mm256_add_ps(row01, row23);	//���Z

		//���ʂ�Ԃ�
		pVecTemp = pVec;
		*pVecTemp = result.m256_f32[0] + result.m256_f32[4];
		pVecTemp++;
		*pVecTemp = result.m256_f32[1] + result.m256_f32[5];
		pVecTemp++;
		*pVecTemp = result.m256_f32[2] + result.m256_f32[6];
	}

	//�s��̊|���Z
	inline void MtxMultiply4x4_AVX(float* mtxA, float* mtxB) noexcept
	{
		float* mtxATemp = mtxA;
		Vec4Transform_AVX(mtxATemp, mtxB);
		mtxATemp += 4;
		Vec4Transform_AVX(mtxATemp, mtxB);
		mtxATemp += 4;
		Vec4Transform_AVX(mtxATemp, mtxB);
		mtxATemp += 4;
		Vec4Transform_AVX(mtxATemp, mtxB);
	}

	//�s��̑����Z
	inline void mtxAdd4x4_AVX(float* pMtx1, float* pMtx2) noexcept
	{
		//�s��Z�b�g
		__m256 row01_1 = _mm256_load_ps(pMtx1 + 0 * 8);
		__m256 row23_1 = _mm256_load_ps(pMtx1 + 1 * 8);
		__m256 row01_2 = _mm256_load_ps(pMtx2 + 0 * 8);
		__m256 row23_2 = _mm256_load_ps(pMtx2 + 1 * 8);

		//�s�񓯎m�̑����Z
		__m256 result01 = _mm256_add_ps(row01_1, row01_2);
		__m256 result23 = _mm256_add_ps(row23_1, row23_2);

		//���ʂ�Ԃ�
		float* pMtxTemp = pMtx1;
		float* pResult1 = &result01.m256_f32[0];
		float* pResult2 = &result23.m256_f32[0];
		*pMtxTemp = *pResult1;
		for (size_t i = 0; i < 7; i++) {
			pMtxTemp++;
			pResult1++;
			*pMtxTemp = *pResult1;
		}
		pMtxTemp++;
		*pMtxTemp = *pResult2;
		for (size_t i = 0; i < 7; i++) {
			pMtxTemp++;
			pResult2++;
			*pMtxTemp = *pResult2;
		}
	}
}
