#include "Camera.h"

inline void Camera::SetCameraInfo(float Fov, float Aspect, float ZNear, float ZFar) {
    Camera::Fov = Fov;
    Camera::Aspect = Aspect;
    Camera::ZNear = ZNear;
    Camera::ZFar = ZFar;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(Fov, Aspect, ZNear, ZFar);
}
inline void Camera::GetCameraInfo(float* Fov, float* Aspect, float* ZNear, float* ZFar) {
    *Fov = Camera::Fov;
    *Aspect = Camera::Aspect;
    *ZNear = Camera::ZNear;
    *ZFar = Camera::ZFar;
}

inline float Camera::GetFov() {
    return Camera::Fov;
}
inline float Camera::GetAspect() {
    return Camera::Aspect;
}
inline float Camera::GetZNear() {
    return Camera::ZNear;
}
inline float Camera::GetZFar() {
    return Camera::ZFar;
}

inline Vector3 Camera::GetDirectionOfView() {
    DirectionOfView = GetParentObject()->GetRotation();

    DirectionOfView.Normilize();

    return DirectionOfView;
}