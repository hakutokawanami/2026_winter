#include "Transform.h"

Matrix4x4 Transform::GetWorldMatrix() const
{
    Matrix4x4 scaleMat = Matrix4x4::Scale(scale);

    Matrix4x4 rotateMat = rotation.ToMatrix();

    Matrix4x4 transMat = Matrix4x4::Translate(position);

    return transMat * rotateMat * scaleMat;
}

Matrix4x4 Transform::GetRotationMatrix() const
{
    return rotation.ToMatrix();
}

Vector3 Transform::Forward() const
{
    return rotation.Rotate(Vector3::Forward());
}

Vector3 Transform::Right() const
{
    return rotation.Rotate(Vector3::Right());
}

Vector3 Transform::Up() const
{
    return rotation.Rotate(Vector3::Up());
}

void Transform::Translate(const Vector3& move)
{
    position += move;
}

void Transform::SetPosition(const Vector3& pos)
{
    position = pos;
}

Vector3 Transform::GetPosition() const
{
    return position;
}

void Transform::SetRotate(const Quaternion& q)
{
    rotation = q.Normalize();
}

Quaternion Transform::GetRotation() const
{
    return rotation;
}

void Transform::SetScale(const Vector3& s)
{
    scale = s;
}

Transform Transform::FromMatrix(const Matrix4x4& mat)
{
    Transform t;
    t.SetPosition(mat.GetTranslation());
    t.SetRotate(Quaternion::FromMatrix(mat.GetRotationMatrix()));
    t.SetScale(mat.GetScale());
    return t;
}
