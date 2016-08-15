#pragma once

#include "../NewComponent.h"

#include "../../rendering/Voxels.h"
#include "../../rendering/Shader.h"
#include "../../Application.h"

struct VoxelModelComponent : Component
{
	Model		  *model;
	ShaderProgram *object_shader;

	virtual void Create() {}
	virtual void Update(GameObject *object) {}

	// This can be optimised using material batching
	void Render(GameObject *object, const glm::mat4& view) 
	{
		if (model == NULL)
		{
			std::cout << "My model is NULL!" << std::endl;
			return;
		}

		object_shader->Use();
		object_shader->UploadMatrix4f("projection", Application::projection_matrix);
		object_shader->UploadMatrix4f("view", view);
		
		glm::mat4 model_matrix;
		model_matrix = glm::translate(model_matrix, object->position);
		// Rotate here
		model_matrix = glm::scale(model_matrix, object->scale);
		
		object_shader->UploadMatrix4f("model", model_matrix);
		DrawModel((*model));
	}
};