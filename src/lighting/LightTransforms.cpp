#include "Light.h"
#include "Sphere.h"
void CLight::AttemptUpdate(float) {}
void CLight::CommitUpdate() {}
// Follow node from now on: keep the given offset, or use an identity offset and
// take the node's world matrix at once; then observe the node and move under it.
int CLight::Attach(ISceneNode &node, CMatrix *offset, unsigned int slot) {
    attachSlot = slot;
    if (offset) {
        attachTransform = *offset;
    } else {
        attachTransform.GetSlot(0);
        node.GetTMLocalToWorld(localToWorld);
    }
    Assign(&node);
    parent = &node;
    g_scene.Remove(*this);
    g_scene.Add(*this, &node);
    return 1;
}
// Stop observing and following the parent.
int CLight::Detach() {
    Assign(0);
    parent = 0;
    return 1;
}
// Test a sphere of the light's radius around its position.
int CLight::IsVisible(CDrawContext &context) const {
    CVector3 position;
    GetPosition(position);
    CVolSphere sphere(radius, position);
    return sphere.TestVisibility(context);
}
void CLight::Reset() { localToWorld.GetSlot(0); }
void CLight::PreTransform(const CMatrix &matrix) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(matrix, CMatrix::s_TempMat);
}
void CLight::Transform(const CMatrix &matrix) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(CMatrix::s_TempMat, matrix);
}
void CLight::SetTMLocalToWorld(const CMatrix &matrix) { localToWorld = matrix; }
void CLight::SetPosition(const CVector3 &position) { localToWorld.SetPos(position); }
void CLight::SetBasis(const CVector3 &right, const CVector3 &front, const CVector3 &up) {
    localToWorld.SetRight(right);
    localToWorld.SetFront(front);
    localToWorld.SetUp(up);
}
void CLight::Move(const CVector3 &offset) { localToWorld.Translate(offset); }
void CLight::Rotate(const CVector3 &axis, float angle) { localToWorld.Rotate(axis, angle); }
