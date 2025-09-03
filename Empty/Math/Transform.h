#pragma once

#define _ALIGN_16 __declspec(align(16))

#include <Math/matrix4x4.h>
#include <Math/Quaternion.h>


#define AXIS_X			0
#define AXIS_Y			1
#define AXIS_Z			2
#define AXIS_NUM		3

_ALIGN_16
class Transform
{
public:
	Transform(void);
	~Transform(void);

	void SetAutoUpdate(bool isAuto);

	void SetToIdentity();

	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(const vec3f &pos);

	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(const vec3f &pos);

	void MovePosition(float dx, float dy, float dz);
	void MovePosition(const vec3f &delta);

	void MoveLocalPosition(float dx, float dy, float dz);
	void MoveLocalPosition(const vec3f &delta);

	void MoveWorldPosition(float dx, float dy, float dz);
	void MoveWorldPosition(const vec3f &delta);

	void Rotate(float angleX, float angleY, float angleZ);
	void Rotate(const vec3f &angle);

	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(const vec3f &angle);

	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(const vec3f &angle);

	void SetScale(float x, float y, float z);
	void SetScale(const vec3f &scale);

	void Scaling(float dx, float dy, float dz);
	void Scaling(const vec3f &delta);

	void lookDirection(const vec3f& dir, const vec3f& up = vec3f(0, 1, 0));
	void lookDirection(const vec3f& dir, float angle);

	void LookPosition(const vec3f& at, const vec3f& up = vec3f(0, 1, 0));
	void LookPosition(const vec3f& at, float angle);

	void SetLookAt(const vec3f &at, vec3f& up_ = vec3f(0, 1, 0));

	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	void SetRotateWorld(const Matrix4x4& matrix);
	void SetRotateLocal(const Matrix4x4& matrix);

	void SetRotateWorld(const Quaternion& quaternion);
	void SetRotateLocal(const Quaternion& quaternion);
	void SetYawPitchRoll(float x, float y, float z);

	void SetWorldMatrix(const Matrix4x4& matrix);

	virtual void Update();
	void AddChild(Transform* pChild);
	void AddParent(Transform* mParent);
	void RemoveParent();

	Matrix4x4 GetGlobal() const;
	Matrix4x4 GetWorldRotateMatrix() const;
	Quaternion GetWorldRotateQuaternion() const;

	vec3f GetWorldPosition() const;
	vec3f GetLocalPosition() const;

	void GetScaledAxes(vec3f* pAxis) const;
	void GetUnitAxes(vec3f* pAxis) const;
	vec3f GetScaledAxis(int index) const;
	vec3f GetUnitAxis(int index) const;
	vec3f GetForward(bool normalized = true) const;
	vec3f GetUp(bool normalized = true) const;
	vec3f GetRight(bool normalized = true) const;

	vec3f GetScale() const;
	friend class Camera;
protected:
	union {

		struct {
			vec3f		mAxis[3];
		};

		struct {
			vec3f		mRight;
			vec3f		mUp;
			vec3f		mForward;
		};
	};

	Matrix4x4		mLocalM;
	Matrix4x4		mGlobal;

	vec3f			mPosition;
	vec3f			mScale;

	Transform*		mParent;
	Transform*		mFirstChild;
	Transform*		mNext;

	bool			mIsAutoUpdate;
	static vec3f	gAxis[3];
};

