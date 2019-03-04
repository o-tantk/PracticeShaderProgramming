#pragma once

#include "glm\glm.hpp"

class Raycaster
{
protected:
    glm::ivec2 viewport;
    glm::mat4 V, P;
    glm::mat4 m_inverseV, m_inverseP;

    Raycaster(const glm::ivec2 &viewport, const glm::mat4 &V, const glm::mat4 &P)
        : viewport(viewport), V(V), P(P), m_inverseV(glm::inverse(V)), m_inverseP(glm::inverse(P)) { }

public:
    static Raycaster* create(const glm::ivec2 &viewport, const glm::mat4 &V, const glm::mat4 &P)
    {
        return new Raycaster(viewport, V, P);
    }

    void updateMatrix(const glm::mat4 V, const glm::mat4 P)
    {
        this->V = V;
        this->P = P;
        m_inverseV = glm::inverse(V);
        m_inverseP = glm::inverse(P);
    }

    glm::vec2 getNormalizedDeviceCoordinates(int mouseX, int mouseY)
    {
        float ndcX = 2.0f * (static_cast<float>(mouseX) / static_cast<float>(viewport.x)) - 1.0f;
        float ndcY = 1.0f - 2.0f * (static_cast<float>(mouseY) / static_cast<float>(viewport.y));

        return glm::vec2(ndcX, ndcY);
    }

    glm::vec4 getHomogeneousClipCoordinates(const glm::vec2 &ndc)
    {
        return glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);
    }

    glm::vec4 getViewSpaceCoordinates(const glm::vec4 &hcc)
    {
        glm::vec4 vsc = m_inverseP * hcc;
        //printf("%f\n", vsc.z);

        //return glm::vec4(hcc.x / P[0][0], hcc.y / P[1][1], -1.0f, 0.0f);
        return glm::vec4(vsc.x, vsc.y, -1.0f, 0.0f);
    }

    glm::vec3 getWorldSpaceCoordinates(const glm::vec4 &vsc)
    {
        return glm::normalize(m_inverseV * vsc);
    }

    glm::vec3 getRay(int mouseX, int mouseY)
    {
        glm::vec2 ndc = getNormalizedDeviceCoordinates(mouseX, mouseY);
        glm::vec4 hcc = getHomogeneousClipCoordinates(ndc);
        glm::vec4 vsc = getViewSpaceCoordinates(hcc);
        glm::vec3 wsc = getWorldSpaceCoordinates(vsc);

        return wsc;
    }
};