#include "CameraGameView.h"

#include <iostream>

CameraGameView::CameraGameView(int width, int height) :
    m_cameraPos{ 0.0f, 0.0f, 0.0f },
    m_cameraFront{ 0.0f, 0.0f, 1.0f },
    m_cameraUp{ 0.0, 1.0, 0.0 },
    m_cameraRight{ 1.0, 0.0, 0.0 },
    m_lastX{ width / 2.0f },
    m_lastY{ height / 2.0f },
    m_yaw{ 0.0f },
    m_pitch{ 0.0f },
    isFirst{ true }
{

}

void CameraGameView::Update(float deltaTime, MasterUI* masterUI, glm::mat4& view)
{
    float xoffset = masterUI->GetOffset().x - m_lastX;
    float yoffset = m_lastY - masterUI->GetOffset().y;
    m_lastX = masterUI->GetOffset().x;
    m_lastY = masterUI->GetOffset().y;

    float sensitivity = 0.4f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f) { m_pitch = 89.0f; }
    if (m_pitch < -89.0f) { m_pitch = -89.0f; }

    if (isFirst) { m_yaw = -90.0f; m_pitch = 0.0f; isFirst = false; }

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(front);

    glm::vec3 right;
    float rightYaw = m_yaw + 90.0f;
    float rightPitch = 0;
    right.x = cos(glm::radians(rightYaw)) * cos(glm::radians(rightPitch));
    right.y = sin(glm::radians(rightPitch));
    right.z = sin(glm::radians(rightYaw)) * cos(glm::radians(rightPitch));
    m_cameraRight = glm::normalize(right);

    float scrollSpeed = 240.0f * deltaTime;
    m_cameraPos += masterUI->GetMouseWheel() * scrollSpeed * m_cameraFront;

    float cameraSpeed = 30.0f * deltaTime;
    Motion camMotion = masterUI->GetCamMotion();
    if (camMotion.Up) { m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraRight)) * cameraSpeed; }
    if (camMotion.Down) { m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraRight)) * cameraSpeed; }
    if (camMotion.Left) { m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed; }
    if (camMotion.Right) { m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed; }

    view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}
