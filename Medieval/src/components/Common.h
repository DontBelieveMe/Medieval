#pragma once

#define REGISTER_PREFAB_COMPONENT_LOADER_(T, ID, it) \
	{ \
		if (ID == T::static_id) { \
		T *component = GetComponentFast<T>(); \
		DeserializeComponent(component, component_it); \
		} \
    }

#define REGISTER_PREFAB_COMPONENT_LOADER(T) \
	REGISTER_PREFAB_COMPONENT_LOADER_(T, ID, it)

// All components that are expected to be loaded by prefabs
// Need to be registered here
#define EXECUTE_ALL_SPECIALISED_COMPONENT_LOADING(id, it) \
	REGISTER_PREFAB_COMPONENT_LOADER(RigidBodyComponent) \
	REGISTER_PREFAB_COMPONENT_LOADER(VoxelModelComponent)
