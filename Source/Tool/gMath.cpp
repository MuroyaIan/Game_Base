
//===== インクルード部 =====
#include <Tool/gMath.h>

//===== クラス実装 =====

//***** ベクトル *****
VECTOR2::VECTOR2() noexcept : x(0.0f), y(0.0f)
{
}
VECTOR2::VECTOR2(float xIn, float yIn) noexcept : x(xIn), y(yIn)
{
}
VECTOR2::~VECTOR2() noexcept
{
}

//演算子オーバーロード
bool VECTOR2::operator==(const VECTOR2 Data) const		//比較
{
	if (this->x == Data.x && this->y == Data.y)
		return true;
	return false;
}

VECTOR2 VECTOR2::operator+(const VECTOR2 Data) const	//加算
{
	//変数宣言
	VECTOR2 f2{};

	//計算
	f2.x = this->x + Data.x;
	f2.y = this->y + Data.y;

	//戻り値
	return f2;
}
VECTOR2& VECTOR2::operator+=(const VECTOR2 Data)
{
	//計算
	this->x += Data.x;
	this->y += Data.y;

	//戻り値
	return *this;
}

VECTOR2 VECTOR2::operator-(const VECTOR2 Data) const	//減算
{
	//変数宣言
	VECTOR2 f2{};

	//計算
	f2.x = this->x - Data.x;
	f2.y = this->y - Data.y;

	//戻り値
	return f2;
}
VECTOR2& VECTOR2::operator-=(const VECTOR2 Data)
{
	//計算
	this->x -= Data.x;
	this->y -= Data.y;

	//戻り値
	return *this;
}

VECTOR2 VECTOR2::operator*(const float Data) const		//乗算
{
	//変数宣言
	VECTOR2 f2{};

	//計算
	f2.x = this->x * Data;
	f2.y = this->y * Data;

	//戻り値
	return f2;
}
VECTOR2& VECTOR2::operator*=(const float Data)
{
	//計算
	this->x *= Data;
	this->y *= Data;

	//戻り値
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

//演算子オーバーロード
bool VECTOR3::operator==(const VECTOR3 Data) const		//比較
{
	if (this->x == Data.x && this->y == Data.y && this->z == Data.z)
		return true;
	return false;
}

VECTOR3 VECTOR3::operator+(const VECTOR3 Data) const	//加算
{
	//変数宣言
	VECTOR3 f3{};

	//計算
	f3.x = this->x + Data.x;
	f3.y = this->y + Data.y;
	f3.z = this->z + Data.z;

	//戻り値
	return f3;
}
VECTOR3& VECTOR3::operator+=(const VECTOR3 Data)
{
	//計算
	this->x += Data.x;
	this->y += Data.y;
	this->z += Data.z;

	//戻り値
	return *this;
}

VECTOR3 VECTOR3::operator-(const VECTOR3 Data) const	//減算
{
	//変数宣言
	VECTOR3 f3{};

	//計算
	f3.x = this->x - Data.x;
	f3.y = this->y - Data.y;
	f3.z = this->z - Data.z;

	//戻り値
	return f3;
}
VECTOR3& VECTOR3::operator-=(const VECTOR3 Data)
{
	//計算
	this->x -= Data.x;
	this->y -= Data.y;
	this->z -= Data.z;

	//戻り値
	return *this;
}

VECTOR3 VECTOR3::operator*(const float Data) const		//乗算
{
	//変数宣言
	VECTOR3 f3{};

	//計算
	f3.x = this->x * Data;
	f3.y = this->y * Data;
	f3.z = this->z * Data;

	//戻り値
	return f3;
}
VECTOR3& VECTOR3::operator*=(const float Data)
{
	//計算
	this->x *= Data;
	this->y *= Data;
	this->z *= Data;

	//戻り値
	return *this;
}
