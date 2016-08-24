#pragma once

#include "../NewComponent.h"

#include "../../rendering/Voxels.h"
#include "../../rendering/Shader.h"
#include "../../Application.h"
#include "../../Utils.h"

struct VoxelModelComponent : Component
{
	COMPONENT(VoxelModelComponent, 0)

	std::string   model_path;
	Model		 model;

	virtual void Create(GameObject *object) 
	{
		Voxels voxel_loader(1);
		model = voxel_loader.loadModel(MODEL_PATH(model_path));
	}

	virtual void Update(GameObject *object) {(void)object;}
	virtual void Destroy() {}

	// This can be optimised using material batching
	void Render(GameObject *object, const glm::mat4& view, ShaderProgram *model_shader)
	{
		glm::mat4 model_matrix;
		glm::vec3 position = object->transform.position;
		model_matrix = glm::translate(model_matrix, position);
	/*	model_matrix = glm::rotate(model_matrix, object->rotation.x, glm::vec3(1, 0, 0));
		model_matrix = glm::rotate(model_matrix, object->rotation.y, glm::vec3(0, 1, 0));
		model_matrix = glm::rotate(model_matrix, object->rotation.z, glm::vec3(0, 0, 1));
		*/
		model_matrix = glm::scale(model_matrix, object->transform.scale);
		model_shader->UploadMatrix4f("model", model_matrix);
		DrawModel(model);
	}

	static void RegisterMembers()
	{
		REGISTER_MEMBER(VoxelModelComponent, model_path);
	}
};
FINISH_COMPONENT(VoxelModelComponent);