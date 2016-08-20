#pragma once

#include "../NewComponent.h"

#include "../../rendering/Voxels.h"
#include "../../rendering/Shader.h"
#include "../../Application.h"
#include "../../Utils.h"

struct VoxelModelComponent : Component
{
	COMPONENT
	
	Model		  *model;
	
	VoxelModelComponent(Model *model) : model(model) {}

	virtual void Create() {}
	virtual void Update(GameObject *object) {}
	virtual void Destroy() { MSVC_LOG("Destroying!"); }

	// This can be optimised using material batching
	void Render(GameObject *object, const glm::mat4& view, ShaderProgram *model_shader) 
	{
		if (model == NULL)
		{
			std::cout << "My model is NULL!" << std::endl;
			return;
		}

		glm::mat4 model_matrix;

		//model_matrix = glm::translate(model_matrix, glm::vec3(position.x(), position.y(), position.z()));
		//model_matrix = glm::rotate(model_matrix, object->rotation.x, glm::vec3(1, 0, 0));
		//model_matrix = glm::rotate(model_matrix, object->rotation.y, glm::vec3(0, 1, 0));
		//model_matrix = glm::rotate(model_matrix, object->rotation.z, glm::vec3(0, 0, 1));
		//model_matrix = glm::scale(model_matrix, object->scale);
		
		model_shader->UploadMatrix4f("model", model_matrix);
		DrawModel((*model));
	}

};