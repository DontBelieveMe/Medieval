#include "UIRenderer.h"
#include <vector>

const glm::vec2 UIRenderer::SCREENSPACE_INCREMENT = glm::vec2((2.0f / WIDTH), (2.0f / HEIGHT)), UIRenderer::SCREENSPACE_OFFSET = glm::vec2(-1.0f, 1.0f);
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

UIRenderer::UIRenderer(const float &width, const float &height, const glm::vec2 &topLeftPos, ShaderProgram *shader)
	: uiQuad(Primitives::GetUIQuad())
{
	_topLeftPosition = glm::vec3((topLeftPos.x) * SCREENSPACE_INCREMENT.x, -topLeftPos.y * SCREENSPACE_INCREMENT.y, 0);
	_scale = glm::vec3(width * SCREENSPACE_INCREMENT.x, height * SCREENSPACE_INCREMENT.y, 0);

	if (shader == nullptr)
		_uiShader = new ShaderProgram("res/shaders/vertUI.shader", "res/shaders/fragUI.shader");
	else
		_uiShader = shader;
	transScaleLocation = _uiShader->GetUniformLoc("translationScale");
}

UIRenderer::UIRenderer(const glm::vec2 &topLeftPos, const glm::vec2 &bottomRightPos, ShaderProgram *shader)
	: uiQuad(Primitives::GetUIQuad())
{
	float width = bottomRightPos.x - topLeftPos.x, height = topLeftPos.y - bottomRightPos.y;
	_topLeftPosition = glm::vec3((topLeftPos.x) * SCREENSPACE_INCREMENT.x, -topLeftPos.y * SCREENSPACE_INCREMENT.y, 0);
	_scale = glm::vec3(width * SCREENSPACE_INCREMENT.x, height * SCREENSPACE_INCREMENT.y, 0);

	if (shader == nullptr)
		_uiShader = new ShaderProgram("res/shaders/vertUI.shader", "res/shaders/fragUI.shader");
	else
		_uiShader = shader;
	transScaleLocation = _uiShader->GetUniformLoc("translationScale");
}

UIRenderer::UIRenderer(const Texture &UITexture, const glm::vec2 &topLeftPos, const float &scaleValue, ShaderProgram *shader)
	: uiQuad(Primitives::GetUIQuad())
{
	float width = (float)UITexture.GetWidth(), height = (float)UITexture.GetHeight();
	_topLeftPosition = glm::vec3((topLeftPos.x) * SCREENSPACE_INCREMENT.x, -topLeftPos.y * SCREENSPACE_INCREMENT.y, 0);
	_scale = glm::vec3(width * SCREENSPACE_INCREMENT.x * scaleValue, height * SCREENSPACE_INCREMENT.y * scaleValue, 0);

	if (shader == nullptr)
		_uiShader = new ShaderProgram("res/shaders/vertUI.shader", "res/shaders/fragUI.shader");
	else
		_uiShader = shader;
	transScaleLocation = _uiShader->GetUniformLoc("translationScale");
}

UIRenderer::~UIRenderer()
{
	delete _uiShader;
}

void UIRenderer::Render()
{
	_uiShader->Use();
	_uiShader->UploadMatrix4f(transScaleLocation, GetMatrix());

	uiQuad.Render();
}

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;

	Vertex(glm::vec3 position, glm::vec2 uv)
		: position(position), uv(uv) {}
};