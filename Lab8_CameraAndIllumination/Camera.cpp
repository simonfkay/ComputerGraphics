#include "Camera.h"

#include <QtMath>

Camera::Camera() : position_(0.0, 0.0, 0.0), lookAt_(0.0, 0.0, 0.0), up_(0.0, 1.0, 0.0), fov_(70.0), yaw_(0.0), pitch_(0.0)
{
	projection_.setToIdentity();
}

Camera::~Camera()
{}

void Camera::calculatePerspective()
{
	projection_.setToIdentity();
	projection_.perspective(fov_, aspect_, 0.001, 1000.0);
}

void Camera::setPosition(const QVector3D& position)
{
	position_ = position;
}

QVector3D Camera::position() const
{
	return position_;
}

void Camera::translateCamera(const QVector3D& delta)
{
	position_ += delta;
	translateLookAt(delta);
}

void Camera::setGazeVector(const QVector3D& gaze)
{
	lookAt_ = gaze + position_;
}

QVector3D Camera::gazeVector() const
{
	QVector3D gaze = lookAt_ - position_;
	gaze.normalize();
	return gaze;
}

QVector3D Camera::lookAt() const
{
	return lookAt_;
}

QVector3D Camera::upVector() const
{
	return up_;
}

void Camera::setLookAt(const QVector3D& lookAt)
{
	lookAt_ = lookAt;
	QVector3D gaze = lookAt_ - position_;
	float pitch = qRadiansToDegrees(qAsin(gaze.y()));
	float yaw = qRadiansToDegrees(qAcos(gaze.x() / qCos(qDegreesToRadians(pitch))));

	yaw_ = yaw;
	pitch_ = pitch;
}

void Camera::translateLookAt(const QVector3D& delta)
{
	lookAt_ += delta;
}

QMatrix4x4 Camera::getViewMatrix() const
{
	QMatrix4x4 ret;
	ret.setToIdentity();
	ret.lookAt(position_, lookAt_, up_);

	return ret;
}

QMatrix4x4 Camera::getProjectionMatrix() const
{
	return projection_;
}

void Camera::modifyFov(float deltaFov) {
	fov_ -= deltaFov;

    if (fov_ <= 1.0f) {
      fov_ = 1.0f;
    } else if (fov_ >= 70.0f) {
      fov_ = 70.0f;
    }
}

void Camera::resetFov() {
	fov_ = 70.0;
}

void Camera::setAspect(float aspect) {
	aspect_ = aspect;
}

float Camera::modifyYaw(float modifyYaw) {
	yaw_ += modifyYaw;
	return yaw_;
}

float Camera::modifyPitch(float modifyPitch) {
	pitch_ += modifyPitch;
	if (pitch_ > 89.0f) {
      pitch_ = 89.0f;
    }
    if (pitch_ < -89.0f) {
      pitch_ = -89.0f;
    }
    return pitch_;
}
