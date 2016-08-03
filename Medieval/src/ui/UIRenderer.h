#pragma once
#include "../extern//glad/glad.h"
#include <glm\glm.hpp>
#include "includes.h"
#include "../rendering/Shader.h"
#include <glm\gtx\transform.hpp>
#include "../rendering/Texture.h"
#include "../rendering/Primitives.h"

/* Author: Morgan Frary
*  - All units (Position, scale) are in screen space coordinates (0 - WIDTH/HEIGHT)
*  - (0, 0) is the Top Left Corner
*/

class UIRenderer
{
public:
	static const glm::vec2 SCREENSPACE_INCREMENT, SCREENSPACE_OFFSET;

	UIRenderer(const float &width, const float &height, const glm::vec2 &topLeftPos, ShaderProgram *shader = nullptr);
	UIRenderer(const glm::vec2 &topLeftPos, const glm::vec2 &bottomRightPos, ShaderProgram *shader = nullptr);
	UIRenderer(const Texture &UITexture, const glm::vec2 &topLeftPos, const float &scaleValue = 1.0f, ShaderProgram *shader = nullptr);
	~UIRenderer();

	void Render();
	
	glm::mat4 GetMatrix()
	{
		glm::vec3 offset = ToVec3(SCREENSPACE_OFFSET);
		return glm::translate(_topLeftPosition + offset) * glm::scale(_scale);
	}

	void SetPosition(const glm::vec3 &topLeftPos)
	{
		_topLeftPosition = glm::vec3((topLeftPos.x) * SCREENSPACE_INCREMENT.x, -topLeftPos.y * SCREENSPACE_INCREMENT.y, 0);
	}

	void SetSize(const glm::vec2 &size) //I'd recommend against using this as it will change the entire size of the UI piece
	{
		_scale = glm::vec3(size.x * SCREENSPACE_INCREMENT.x, size.y * SCREENSPACE_INCREMENT.y, 0);
	}

	void SetSize(const Texture &UITexture) //This is much better <- I'd recommend using this mainly
	{
		float width = (float)UITexture.GetWidth(), height = (float)UITexture.GetHeight();
		_scale = glm::vec3(width * SCREENSPACE_INCREMENT.x, height * SCREENSPACE_INCREMENT.y, 0);
	}

	void Scale(const float &amount) //From 100% (100 being 100%)
	{
		float amountScale = amount / 100;
		_scale *= amountScale;
	}

private:
	ShaderProgram *_uiShader;
	glm::vec3 _topLeftPosition; //Don't touch this you filthy animal!
	glm::vec3 _scale; //Don't touch this either...

	GLint transScaleLocation;
	Primitive &uiQuad;

	glm::vec3 ToVec3(const glm::vec2 &vec2) const
	{
		return glm::vec3(vec2.x, vec2.y, 0);
	}
};

