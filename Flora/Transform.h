#pragma once
#include "Core.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using glm::vec3;
using glm::mat4;
class FLORA_API FTransform
{
public:
    const static inline vec3 WorldRight  = { 1,0,0 };
    const static inline vec3 WorldUp     = { 0,1,0 };
    const static inline vec3 WorldFront  = { 0,0,1 };
    const static inline vec3 WorldOrigin = { 0,0,0 };
public:
    vec3 Position = { 0,0,0 };
    vec3 Rotation = { 0,0,0 };
    vec3 Scale    = { 0.01,0.01,0.01 };
private:
    vec3 Front;
    vec3 Up;
    vec3 Right;
public:
    FTransform();

    mat4 GetModelMatrix();
    mat4 GetViewMatrix();

    void MoveForward(float dS);
    void MoveBack(float dS);
    void MoveLeft(float dS);
    void MoveRight(float dS);
    void MoveUp(float dS);
    void MoveDown(float dS);

    void RotateX(float dAngle,bool constrain = true);
    void RotateY(float dAngle,bool constrain = false);
    void RotateZ(float dAngle, bool constrain = false);
private:
    void UpdateCoord();
};

