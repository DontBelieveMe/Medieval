/*
 * @author Barney Wilks
 */

#pragma once

#include <string>

namespace AssetData
{
	namespace detail
	{
		static const std::string DefaultTexExtension = ".png";
		static const std::string DefaultModelExtension = ".obj";
		static const std::string DefaultShaderExtension = ".shader";
		static const std::string DefaultDataExtension = ".json";
	}

	static const std::string RootPath = "assets/";
	static const std::string ShaderPath = RootPath + "shaders/";
	static const std::string AudioPath = RootPath + "audio/";
	static const std::string ModelPath = RootPath + "models/";
	static const std::string TexturePath = RootPath + "images/";
	static const std::string DataPath = "data/";
}

#define SHADER_PATH(vertName, fragName) \
	AssetData::ShaderPath + vertName + AssetData::detail::DefaultShaderExtension, AssetData::ShaderPath + fragName + AssetData::detail::DefaultShaderExtension

// This only works when the folder containing the files and the files are the same name
// The texture and obj file must be the same name.
// On windows case does not matter - so different capitalisations are OK!
#define MODEL_PATH(modelName) \
	AssetData::ModelPath + modelName + "/" + modelName +  AssetData::detail::DefaultModelExtension, \
	AssetData::ModelPath + modelName + "/" + modelName + AssetData::detail::DefaultTexExtension

#define TEXTURE_PATH(imageName) \
	AssetData::TexturePath + imageName + AssetData::detail::DefaultTexExtension

// This new system... Explained
// For example:
//		When Creating a new ShaderProgram...
//			new ShaderProgram(SHADER_PATH(<vertexShaderName>, <fragmentShaderName>));	// Note - do not include the extension
//		When Creating a new Model...
//			new Model(MODEL_PATH(<modelName>));	// Note - just the name, no extension
//		When Creating a new texture
//			new Texture(TEXTURE_PATH(<textureName>));	// Note - no extensions