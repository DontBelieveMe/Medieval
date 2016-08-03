#include "Renderer2D.h"

Renderer2D::Renderer2D(ShaderProgram *shader, const std::string & tex_path, glm::ivec2 size_in_tiles, glm::mat4 &ortho)
    : size_in_tiles(size_in_tiles), shader_ptr(shader)
{
    GLint previous_shader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_shader);
	shader->Use();
	shader->UploadMatrix4f("ortho", ortho);		// Probably shouldn't hardcode this name/location: TODO change
    glUseProgram(previous_shader);
    tex = new Texture(tex_path);
}

Renderer2D::Renderer2D(ShaderProgram *shader, const std::string& texPath, glm::ivec2 sizeInTiles)
	: size_in_tiles(sizeInTiles), shader_ptr(shader)
{
    GLint previousShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);
	shader->Use();
	shader->UploadMatrix4f("ortho", glm::ortho(0.f, (float)WIDTH, (float)HEIGHT, 0.0f, 1.0f, -1.0f));
    glUseProgram(previousShader);
    tex = new Texture(texPath);
}

void Renderer2D::drawTile(int uv_x, int uv_y, int uv_w, int uv_h, int x_offset, int y_offset, int width, int height)
{
	shader_ptr->UploadVector2f("tileWH", size_in_tiles);
	GLint tileLoc = shader_ptr->GetUniformLoc("tile");
    glUniform4i(tileLoc, uv_x, uv_y, uv_w, uv_h);

	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(x_offset, y_offset, 0.0f));
    model           = glm::scale(model, glm::vec3(width, height, 1.0f));
    shader_ptr->UploadMatrix4f("model", model);
	glBindVertexArray(AdditionalVAO::vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AdditionalVAO::ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Renderer2D::~Renderer2D()
{
    delete tex;
}

void Renderer2D::destroy()
{
    //tex->Destroy();
}