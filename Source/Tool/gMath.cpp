
//===== �C���N���[�h�� =====
#include <Tool/gMath.h>

//===== �N���X���� =====

//***** �x�N�g�� *****
VECTOR2::VECTOR2() noexcept : x(0.0f), y(0.0f)
{
}
VECTOR2::VECTOR2(float xIn, float yIn) noexcept : x(xIn), y(yIn)
{
}
VECTOR2::~VECTOR2() noexcept
{
}

//���Z�q�I�[�o�[���[�h
bool VECTOR2::operator==(const VECTOR2 Data) const		//��r
{
	if (this->x == Data.x && this->y == Data.y)
		return true;
	return false;
}

VECTOR2 VECTOR2::operator+(const VECTOR2 Data) const	//���Z
{
	//�ϐ��錾
	VECTOR2 f2{};

	//�v�Z
	f2.x = this->x + Data.x;
	f2.y = this->y + Data.y;

	//�߂�l
	return f2;
}
VECTOR2& VECTOR2::operator+=(const VECTOR2 Data)
{
	//�v�Z
	this->x += Data.x;
	this->y += Data.y;

	//�߂�l
	return *this;
}

VECTOR2 VECTOR2::operator-(const VECTOR2 Data) const	//���Z
{
	//�ϐ��錾
	VECTOR2 f2{};

	//�v�Z
	f2.x = this->x - Data.x;
	f2.y = this->y - Data.y;

	//�߂�l
	return f2;
}
VECTOR2& VECTOR2::operator-=(const VECTOR2 Data)
{
	//�v�Z
	this->x -= Data.x;
	this->y -= Data.y;

	//�߂�l
	return *this;
}

VECTOR2 VECTOR2::operator*(const float Data) const		//��Z
{
	//�ϐ��錾
	VECTOR2 f2{};

	//�v�Z
	f2.x = this->x * Data;
	f2.y = this->y * Data;

	//�߂�l
	return f2;
}
VECTOR2& VECTOR2::operator*=(const float Data)
{
	//�v�Z
	this->x *= Data;
	this->y *= Data;

	//�߂�l
	return *this;
}

VECTOR3::VECTOR3() noexcept : x(0.0f), y(0.0f), z(0.0f)
{
}
VECTOR3::VECTOR3(float xIn, float yIn, float zIn) noexcept : x(xIn), y(yIn), z(zIn)
{
}
VECTOR3::~VECTOR3() noexcept
{
}

//���Z�q�I�[�o�[���[�h
bool VECTOR3::operator==(const VECTOR3 Data) const		//��r
{
	if (this->x == Data.x && this->y == Data.y && this->z == Data.z)
		return true;
	return false;
}

VECTOR3 VECTOR3::operator+(const VECTOR3 Data) const	//���Z
{
	//�ϐ��錾
	VECTOR3 f3{};

	//�v�Z
	f3.x = this->x + Data.x;
	f3.y = this->y + Data.y;
	f3.z = this->z + Data.z;

	//�߂�l
	return f3;
}
VECTOR3& VECTOR3::operator+=(const VECTOR3 Data)
{
	//�v�Z
	this->x += Data.x;
	this->y += Data.y;
	this->z += Data.z;

	//�߂�l
	return *this;
}

VECTOR3 VECTOR3::operator-(const VECTOR3 Data) const	//���Z
{
	//�ϐ��錾
	VECTOR3 f3{};

	//�v�Z
	f3.x = this->x - Data.x;
	f3.y = this->y - Data.y;
	f3.z = this->z - Data.z;

	//�߂�l
	return f3;
}
VECTOR3& VECTOR3::operator-=(const VECTOR3 Data)
{
	//�v�Z
	this->x -= Data.x;
	this->y -= Data.y;
	this->z -= Data.z;

	//�߂�l
	return *this;
}

VECTOR3 VECTOR3::operator*(const float Data) const		//��Z
{
	//�ϐ��錾
	VECTOR3 f3{};

	//�v�Z
	f3.x = this->x * Data;
	f3.y = this->y * Data;
	f3.z = this->z * Data;

	//�߂�l
	return f3;
}
VECTOR3& VECTOR3::operator*=(const float Data)
{
	//�v�Z
	this->x *= Data;
	this->y *= Data;
	this->z *= Data;

	//�߂�l
	return *this;
}
