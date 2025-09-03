#include <Core/PCH.h>
#include <Math/Transform.h>

vec3f Transform::gAxis[] = {
	vec3f(1.0f, 0.0f, 0.0f),
	vec3f(0.0f, 1.0f, 0.0f),
	vec3f(0.0f, 0.0f, 1.0f) };

Transform::Transform(void)
	: mParent(NULL), mFirstChild(NULL), mNext(NULL), mIsAutoUpdate(true)
{

	SetToIdentity();

	//Update();
}

Transform::~Transform(void)
{
}

void Transform::SetAutoUpdate(bool isAuto)
{
	this->mIsAutoUpdate = isAuto;
}

void Transform::SetToIdentity()
{
	this->mPosition.x = 0.f;
	this->mPosition.y = 0.f;
	this->mPosition.z = 0.f;

	this->mRight = vec3f(1, 0, 0);
	this->mUp = vec3f(0, 1, 0);
	this->mForward = vec3f(0, 0, 1);

	this->mScale = vec3f(1, 1, 1);
	this->Update();
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	this->mPosition.x = x;
	this->mPosition.y = y;
	this->mPosition.z = z;

	this->Update();
}
void Transform::SetLocalPosition(const vec3f &pos)
{
	this->SetLocalPosition(pos.x, pos.y, pos.z);
}

void Transform::SetWorldPosition(float x, float y, float z)
{
	vec3f pos(x, y, z);

	if (mParent)
	{
		Matrix4x4 parentInvFinal;
		parentInvFinal = mParent->mGlobal.Inverted();

		pos = parentInvFinal * pos;
	}

	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mPosition.z = pos.z;

	this->Update();
}
void Transform::SetWorldPosition(const vec3f &pos)
{
	this->SetWorldPosition(pos.x, pos.y, pos.z);
}

void Transform::MovePosition(float dx, float dy, float dz)
{
	vec3f delta(0, 0, 0);
	vec3f axis[3];
	GetUnitAxes(axis);
	delta += axis[0] * dx;
	delta += axis[1] * dy;
	delta += axis[2] * dz;

	vec3f world = GetWorldPosition();

	this->SetWorldPosition(world + delta);
}

void Transform::MovePosition(const vec3f &delta)
{
	this->MovePosition(delta.x, delta.y, delta.z);
}

void Transform::MoveLocalPosition(float dx, float dy, float dz)
{
	this->mPosition.x += dx;
	this->mPosition.y += dy;
	this->mPosition.z += dz;

	this->Update();
}

void Transform::MoveLocalPosition(const vec3f &delta)
{
	this->mPosition += delta;

	this->Update();
}

void Transform::MoveWorldPosition(float dx, float dy, float dz)
{
	vec3f world = this->GetWorldPosition();

	this->SetWorldPosition(world + vec3f(dx, dy, dz));

}
void Transform::MoveWorldPosition(const vec3f& delta)
{
	vec3f nowWorldPos = this->GetWorldPosition();

	this->SetWorldPosition(nowWorldPos + delta);
}

void Transform::RotateWorld(float angleX, float angleY, float angleZ)
{
	if (this->mParent)
	{
		vec3f worldAxis[3];
		this->GetUnitAxes(worldAxis);

		Matrix4x4 matRotateX = Matrix4x4::RotatedX(angleX);

		Matrix4x4 matRotateY = Matrix4x4::RotatedY(angleY);

		Matrix4x4 matRotateZ = Matrix4x4::RotatedZ(angleZ);


		Matrix4x4 matRotate = matRotateY * matRotateX * matRotateZ;

		Matrix4x4 matInvParentFinal;
		matInvParentFinal = this->mParent->mGlobal.Inverted();

		matRotate = matRotate * matInvParentFinal;

		for (int i = 0; i < 3; i++)
			this->mAxis[i] = matRotate.Normal(worldAxis[i]);

		this->Update();
	}

	else
	{
		RotateLocal(angleX, angleY, angleZ);
	}
}

void Transform::RotateWorld(const vec3f &angle)
{
	if (this->mParent)
	{
		vec3f worldAxis[3];
		this->GetUnitAxes(worldAxis);

		Matrix4x4 rotX = Matrix4x4::RotatedX(angle.x);

		Matrix4x4 rotY = Matrix4x4::RotatedY(angle.y);

		Matrix4x4 rotZ = Matrix4x4::RotatedZ(angle.z);

		Matrix4x4 rotM = rotY * rotX * rotZ;

		Matrix4x4 parentInvM;
		parentInvM = this->mParent->mGlobal.Inverted();

		rotM = rotM * parentInvM;

		for (int i = 0; i < 3; i++)
			this->mAxis[i] = rotM.Normal(worldAxis[i]);

		this->Update();
	}
	else
	{
		RotateLocal(angle.x, angle.y, angle.z);
	}
}

void Transform::Rotate(float angleX, float angleY, float angleZ)
{
	Matrix4x4 rotX;
	rotX = Matrix4x4::RotationAxis(this->GetRight(), angleX);

	Matrix4x4 rotY;
	rotY = Matrix4x4::RotationAxis(this->GetUp(), angleY);

	Matrix4x4 rotZ;
	rotZ = Matrix4x4::RotationAxis(this->GetForward(), angleZ);

	Matrix4x4 rotM = rotY * rotX * rotZ;
	for (int i = 0; i < 3; i++)
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);

	this->Update();
}

void Transform::Rotate(const vec3f &angle)
{
	Matrix4x4 rotX;
	rotX = Matrix4x4::RotationAxis(this->GetRight(), angle.x);

	Matrix4x4 rotY;
	rotY = Matrix4x4::RotationAxis(this->GetUp(), angle.y);

	Matrix4x4 rotZ;
	rotZ = Matrix4x4::RotationAxis(this->GetForward(), angle.z);

	Matrix4x4 rotM = rotY * rotX * rotZ;
	for (int i = 0; i < 3; i++)
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);

	this->Update();
}

void Transform::RotateLocal(float angleX, float angleY, float angleZ)
{
	Matrix4x4 rotX = Matrix4x4::RotatedX(angleX);

	Matrix4x4 rotY = Matrix4x4::RotatedY(angleY);

	Matrix4x4 rotZ = Matrix4x4::RotatedZ(angleZ);

	Matrix4x4 rotM = rotY * rotX * rotZ;

	for (int i = 0; i < 3; i++)
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);

	this->Update();
}

void Transform::RotateLocal(const vec3f &angle)
{
	Matrix4x4 rotX = Matrix4x4::RotatedX(angle.x);

	Matrix4x4 rotY = Matrix4x4::RotatedY(angle.y);

	Matrix4x4 rotZ = Matrix4x4::RotatedZ(angle.z);

	Matrix4x4 rotM = rotY * rotX * rotZ;

	for (int i = 0; i < 3; i++)
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);

	this->Update();
}

void Transform::SetScale(float x, float y, float z)
{
	this->mScale.x = x;
	this->mScale.y = y;
	this->mScale.z = z;

	this->Update();
}
void Transform::SetScale(const vec3f &scale)
{
	this->mScale = mScale;
	this->Update();
}

void Transform::Scaling(float dx, float dy, float dz)
{
	this->mScale.x += dx;
	this->mScale.y += dy;
	this->mScale.z += dz;

	this->Update();
}
void Transform::Scaling(const vec3f &delta)
{
	this->mScale += delta;

	this->Update();
}

void Transform::lookDirection(const vec3f& dir, const vec3f& up)
{

	vec3f newForward = dir;
	vec3f newRight;
	newRight = vec3f::cross(up, newForward).normalized();

	vec3f newUp;
	newUp = vec3f::cross(newForward, newRight).normalized();

	if (this->mParent)
	{
		Matrix4x4 parentInvM;
		parentInvM = mParent->mGlobal.Inverted();

		mForward = parentInvM.Normal(newForward);
		mRight = parentInvM.Normal(newRight);
		mUp = parentInvM.Normal(newUp);
	}
	else
	{
		mForward = newForward;
		mRight = newRight;
		mUp = newUp;
	}

	this->Update();
}

void Transform::lookDirection(const vec3f& dir, float angle)
{
	vec3f worldAxis[3];
	this->GetUnitAxes(worldAxis);

	//get degree btw front and look dir
	float distRadian = acos(
		math::ClampMinusOneToPlusOne(vec3f::dot(worldAxis[AXIS_Z], dir)));


	//if degree is almost 0
	if (FLOAT_ZERO(distRadian)) return;

	vec3f cross;
	cross = vec3f::cross(worldAxis[AXIS_Z], dir).normalized();

	//rotate each axis by cross
	Matrix4x4 rotM;
	rotM = Matrix4x4::RotationAxis(cross, std::min(angle, distRadian));

	if (this->mParent)
	{
		Matrix4x4 parentInvM;
		parentInvM = this->mParent->mGlobal.Inverted();
		rotM = rotM * parentInvM;
	}

	for (int i = 0; i < 3; i++)
		this->mAxis[i], rotM.Normal(worldAxis[i]);

	this->Update();
}

void Transform::LookPosition(const vec3f& at, const vec3f& up)
{
	vec3f world = this->GetWorldPosition();
	vec3f dir = (world - at).normalized();

	this->lookDirection(dir, up);
}

void Transform::LookPosition(const vec3f& at, float angle)
{
	vec3f world = this->GetWorldPosition();
	vec3f dir = (world - at).normalized();

	this->lookDirection(dir, angle);
}
/*
*	-Z is front for GL view matrtix
*	WorldP <---- Z(forward) ----- Target (GL Right Hand)
*	WorldP ----- Z(forward) ----> Traget (DirectX Left Hand)
*/
void Transform::SetLookAt(const vec3f &at, vec3f& up_)
{
	vec3f forward = (GetWorldPosition() - at).normalized();
	vec3f right = vec3f::cross(up_, forward).normalized();
	vec3f up = vec3f::cross(forward, right).normalized();

	if (this->mParent)
	{
		Matrix4x4 parentInvM;
		parentInvM = mParent->mGlobal.Inverted();

		mForward = parentInvM.Normal(forward);
		mRight = parentInvM.Normal(right);
		mUp = parentInvM.Normal(up);
	}
	else
	{
		mForward = forward;
		mRight = right;
		mUp = up;
	}

	this->Update();
}

void Transform::SetRotateWorld(float eAngleX, float eAngleY, float eAngleZ)
{
	Quaternion rotQ;
	rotQ = Quaternion::RotationYawPitchRoll(eAngleX, eAngleY, eAngleZ);

	Matrix4x4 rotM;
	rotM = rotQ.GetMatrix();

	if (this->mParent)
	{
		Matrix4x4 parentInvM;
		parentInvM = mParent->mGlobal.Inverted();
		rotM = rotM * parentInvM;
	}

	//reset axis
	mRight = vec3f(1, 0, 0);
	mUp = vec3f(0, 1, 0);
	mForward = vec3f(0, 0, 1);

	for (int i = 0; i < 3; i++) {
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);
	}

	this->Update();
}

void Transform::SetRotateLocal(float eAngleX, float eAngleY, float eAngleZ)
{
	Quaternion rotQ;
	rotQ = Quaternion::RotationYawPitchRoll(eAngleX, eAngleY, eAngleZ);

	Matrix4x4 rotM;
	rotM = rotQ.GetMatrix();

	mRight = vec3f(1, 0, 0);
	mUp = vec3f(0, 1, 0);
	mForward = vec3f(0, 0, 1);

	for (int i = 0; i < 3; i++) {
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);
	}

	this->Update();
}

//set rotate directly by rotation matix
void Transform::SetRotateWorld(const Matrix4x4& matrix)
{
	Matrix4x4 rotM = matrix;

	if (this->mParent)
	{
		Matrix4x4 parentInvM;
		parentInvM = mParent->mGlobal.Inverted();
		rotM = rotM * parentInvM;
	}

	mRight = vec3f(1, 0, 0);
	mUp = vec3f(0, 1, 0);
	mForward = vec3f(0, 0, 1);

	for (int i = 0; i < AXIS_NUM; i++) {
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);
	}

	this->Update();
}

void Transform::SetRotateLocal(const Matrix4x4& matrix)
{
	Matrix4x4 rotM = matrix;

	mRight = vec3f(1, 0, 0);
	mUp = vec3f(0, 1, 0);
	mForward = vec3f(0, 0, 1);

	for (int i = 0; i < 3; i++)
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);

	this->Update();
}

void Transform::SetRotateWorld(const Quaternion& quaternion)
{
	Quaternion rotQ = quaternion;

	Matrix4x4 rotM;
	rotM = rotQ.GetMatrix();

	if (this->mParent)
	{
		Matrix4x4 parentInvM;
		parentInvM = this->mParent->mGlobal.Inverted();
		rotM = rotM * parentInvM;
	}

	mRight = vec3f(1, 0, 0);
	mUp = vec3f(0, 1, 0);
	mForward = vec3f(0, 0, 1);

	for (int i = 0; i < AXIS_NUM; i++) {
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);
	}

	this->Update();
}

void Transform::SetRotateLocal(const Quaternion& matWorldRotate)
{
	Quaternion rotQ = matWorldRotate;
	Matrix4x4 rotM(rotQ.GetMatrix());

	mRight = vec3f(1, 0, 0);
	mUp = vec3f(0, 1, 0);
	mForward = vec3f(0, 0, 1);

	for (int i = 0; i < 3; i++) {
		this->mAxis[i] = rotM.Normal(this->mAxis[i]);
	}

	this->Update();
}

void Transform::SetYawPitchRoll(float x, float y, float z)
{
	Quaternion qRot = Quaternion::RotationYawPitchRoll(y, x, z);

	Matrix4x4 rot = qRot.GetMatrix();

	for (int i = 0; i < 3; ++i) {
		mAxis[i] = rot.Normal(gAxis[i]);
	}
	Update();
}

void Transform::SetWorldMatrix(const Matrix4x4& matrix)
{
	this->mGlobal = matrix;
	this->mLocalM = matrix;

	mPosition = vec3f(
		mGlobal[0][3],
		mGlobal[1][3],
		mGlobal[2][3]);

	vec3f front = vec3f(
		mGlobal[0][2],
		mGlobal[1][2],
		mGlobal[2][2]);

	vec3f up = vec3f(
		mGlobal[0][1],
		mGlobal[1][1],
		mGlobal[2][1]);

	vec3f right = vec3f(
		mGlobal[0][0],
		mGlobal[1][0],
		mGlobal[2][0]);

	mForward = front.normalized();
	mUp = up.normalized();
	mRight = right.normalized();

	mScale.x = right.length();
	mScale.y = up.length();
	mScale.z = front.length();

	if (mParent)
	{
		Matrix4x4 parentInvM = mParent->GetGlobal().Inverted();

		mLocalM = mGlobal * parentInvM;

		mPosition.x = mLocalM[0][3];
		mPosition.y = mLocalM[1][3];
		mPosition.z = mLocalM[2][3];

		vec3f front;
		front.x = mLocalM[0][2];
		front.y = mLocalM[1][2];
		front.z = mLocalM[2][2];

		vec3f up;
		up.x = mLocalM[0][1];
		up.y = mLocalM[1][1];
		up.z = mLocalM[2][1];

		vec3f right;
		right.x = mLocalM[0][0];
		right.y = mLocalM[1][0];
		right.z = mLocalM[2][0];

		float scaleX = right.length();
		float scaleY = up.length();
		float scaleZ = right.length();

		mScale = vec3f(scaleX, scaleY, scaleZ);

		front *= (1.0f) / scaleZ;
		up *= (1.0f) / scaleY;
		right *= (1.0f) / scaleX;

		mForward = front;
		mUp = up;
		mRight = right;

	}

	Transform* pChild = this->mFirstChild;
	while (pChild != NULL) {
		pChild->Update();
		pChild = pChild->mNext;
	}
}

void Transform::AddChild(Transform* pNewChild)
{
	if (pNewChild->mParent == this)
		return;

	//release Parent first if we has
	pNewChild->RemoveParent();


	Matrix4x4 matInvFinal;
	matInvFinal = this->mGlobal.Inverted();


	pNewChild->mPosition = matInvFinal * pNewChild->mPosition;


	for (int i = 0; i < 3; i++) {
		pNewChild->mAxis[i] = matInvFinal.Normal(pNewChild->mAxis[i]);
	}

	pNewChild->mScale.x = pNewChild->mRight.length();
	pNewChild->mScale.y = pNewChild->mUp.length();
	pNewChild->mScale.z = pNewChild->mForward.length();

	pNewChild->mRight = pNewChild->mRight.normalized();
	pNewChild->mUp = pNewChild->mUp.normalized();
	pNewChild->mForward = pNewChild->mForward.normalized();

	pNewChild->mParent = this;

	Transform* pChild = this->mFirstChild;

	if (pChild == NULL) {
		this->mFirstChild = pNewChild;
		pNewChild->mParent = this;
	}


	else {

		while (pChild != NULL) {
			if (pChild->mNext == NULL)
			{
				pChild->mNext = pNewChild;
				pNewChild->mParent = this;
				break;
			}

			pChild = pChild->mNext;
		}
	}

	this->Update();
}

void Transform::AddParent(Transform* pNewParent)
{
	pNewParent->AddChild(this);
}

void Transform::RemoveParent()
{
	//no parent?
	if (this->mParent == NULL)
		return;

	//break off child first
	Transform* pChild = this->mParent->mFirstChild;

	//am i first child?
	if (pChild == this) {

		this->mParent->mFirstChild = this->mNext;
		this->mNext = NULL;
	}

	else
	{
		while (pChild != NULL) {

			//am i ur child?
			if (pChild->mNext == this) {

				pChild->mNext = this->mNext;

				//let's break off my bro and sis
				this->mNext = NULL;

				break;
			}

			//next child
			pChild = pChild->mNext;
		}
	}

	//remove parent
	this->mParent = NULL;

	//Update world mPosition
	this->mPosition.x = this->mGlobal[0][3];
	this->mPosition.y = this->mGlobal[1][3];
	this->mPosition.z = this->mGlobal[2][3];

	//get axis
	vec3f forwardScaled(this->mGlobal[0][2], this->mGlobal[1][2], this->mGlobal[2][2]);
	vec3f upScaled(this->mGlobal[0][1], this->mGlobal[1][1], this->mGlobal[2][1]);
	vec3f rightScaled(this->mGlobal[0][0], this->mGlobal[1][0], this->mGlobal[2][0]);

	//get each axis mScale from axis
	float scaleX = rightScaled.length();
	float scaleY = upScaled.length();
	float scaleZ = forwardScaled.length();

	//normalizing
	vec3f forwardUnit;
	vec3f upUnit;
	vec3f rightUnit;
	rightUnit = rightScaled.normalized();
	upUnit = upScaled.normalized();
	forwardUnit = forwardScaled.normalized();

	//normalize axis
	this->mForward = forwardUnit;
	this->mRight = rightUnit;
	this->mUp = upUnit;


	//mScale
	this->mScale.x = scaleX;
	this->mScale.y = scaleY;
	this->mScale.z = scaleZ;

	this->Update();
}

Matrix4x4 Transform::GetGlobal() const
{
	return this->mGlobal;
}

Matrix4x4 Transform::GetWorldRotateMatrix() const
{
	Matrix4x4 rotateM;

	//get normalize axis
	vec3f pAxis[3];
	this->GetUnitAxes(pAxis);

	rotateM[0][0] = pAxis[0].x;
	rotateM[1][0] = pAxis[0].y;
	rotateM[2][0] = pAxis[0].z;

	rotateM[0][1] = pAxis[1].x;
	rotateM[1][1] = pAxis[1].y;
	rotateM[2][1] = pAxis[1].z;

	rotateM[0][2] = pAxis[2].x;
	rotateM[1][2] = pAxis[2].y;
	rotateM[2][2] = pAxis[2].z;

	return rotateM;
}
Quaternion Transform::GetWorldRotateQuaternion() const
{
	Quaternion quat;

	Matrix4x4 matRotate = this->GetWorldRotateMatrix();

	quat = quat * matRotate;

	return quat;
}

vec3f Transform::GetWorldPosition() const
{
	vec3f pos = this->mPosition;

	if (this->mParent) {
		pos = this->mParent->mGlobal * pos;
	}

	return pos;
}
vec3f Transform::GetLocalPosition() const
{
	return this->mPosition;
}

//get scaled Axis's (world)
void Transform::GetScaledAxes(vec3f* pAxis) const
{
	for (int i = 0; i < 3; i++) {
		pAxis[i] = mAxis[i].normalized();
	}

	if (this->mParent) {
		Matrix4x4 parentFinalM = this->mParent->mGlobal;
		for (int i = 0; i < 3; i++) {
			pAxis[i] = parentFinalM.Normal(pAxis[i]);
		}
	}

}
void Transform::GetUnitAxes(vec3f* pAxis) const
{
	for (int i = 0; i < 3; i++) {
		pAxis[i] = mAxis[i].normalized();
	}
	if (this->mParent) {
		Matrix4x4 parentFinalM = this->mParent->mGlobal;
		for (int i = 0; i < 3; i++) {
			pAxis[i] = parentFinalM.Normal(pAxis[i]);
		}
	}
}
vec3f Transform::GetScaledAxis(int index) const
{
	vec3f result = mAxis[index];

	if (this->mParent) {
		Matrix4x4 parentFinalM = this->mParent->mGlobal;
		result = parentFinalM.Normal(result);
	}

	return result;
}
vec3f Transform::GetUnitAxis(int index) const
{
	vec3f result;
	result = mAxis[index].normalized();

	if (this->mParent) {
		Matrix4x4 parentFinalM = this->mParent->mGlobal;
		result = parentFinalM.Normal(result);
	}

	return result;
}
vec3f Transform::GetForward(bool normalized) const
{
	if (normalized)
		return GetUnitAxis(AXIS_Z);

	return GetScaledAxis(AXIS_Z);
}
vec3f Transform::GetUp(bool normalized) const
{
	if (normalized)
		return GetUnitAxis(AXIS_Y);

	return GetScaledAxis(AXIS_Y);
}
vec3f Transform::GetRight(bool normalized) const
{
	if (normalized)
		return GetUnitAxis(AXIS_X);

	return GetScaledAxis(AXIS_X);
}

vec3f Transform::GetScale() const
{
	return mScale;
}

void Transform::Update()
{
	if (!mIsAutoUpdate) return;

	mLocalM.SetToIdentity();

	vec3f right = mRight * mScale.x;
	vec3f up = mUp * mScale.y;
	vec3f forward = mForward * mScale.z;

	mLocalM = {
		right.x, up.x, forward.x, mPosition.x,
		right.y, up.y, forward.y, mPosition.y,
		right.z, up.z, forward.z, mPosition.z,
		0.0f,0.0f,0.0f,1.0f
	};

	if (this->mParent == NULL)
	{
		this->mGlobal = mLocalM;
	}
	else {
		this->mGlobal = mParent->mGlobal * mLocalM;
	}

	Transform* pChild = this->mFirstChild;

	while (pChild != NULL)
	{
		pChild->Update();
		pChild = pChild->mNext;
	}
}


