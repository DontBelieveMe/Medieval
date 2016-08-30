#pragma once

#include <type_traits>
#include "IntrospectionManager.h"

#define REGISTER_TYPE(T) \
	IntrospectionManager::Get()->RegisterType<T>(#T, sizeof(T))

#define TYPE_INFO(T) \
	IntrospectionManager::Get()->StaticType<T>()

#define TYPE_OF(VAR) \
	IntrospectionManager::Get()->StaticType<decltype(VAR)>()

#define TYPE_OF_PTR(PTR) \
	IntrospectionManager::Get()->StaticType< std::remove_pointer_t< decltype(PTR) >>()

#define TYPE_INFO_STR(str) \
	IntrospectionManager::Get()->GetTypeByStr(str)

#define DECLTYPE_MEMBER(T, MEMBER) \
	detail::RemoveMemberDetail<decltype(&T::MEMBER)>::type

#define TYPE_OF_MEMBER(T, MEMBER) \
	TYPE_INFO(DECLTYPE_MEMBER(T, MEMBER))

#define OFFSET_OF(T, MEMBER) \
	detail::OffsetOf(&T::MEMBER)

#define REGISTER_MEMBER(T, MEMBER) \
	IntrospectionManager::Get()->RegisterMember(TYPE_INFO(T), TYPE_OF_MEMBER(T, MEMBER), #MEMBER, detail::OffsetOf(&T::MEMBER))

#define SET_MEMBER_VAR(OBJ, MEMBER, VALUE) \
	TYPE_OF(OBJ)->SetMember(&OBJ, &decltype(OBJ)::MEMBER, VALUE)

#define SET_MEMBER_STR(OBJ, MEMBER, VALUE) \
	TYPE_OF(OBJ)->SetMember(&OBJ, MEMBER, VALUE)

#define SET_MEMBER_STR_OBJ_PTR(OBJ, MEMBER, VALUE) \
	TYPE_OF_PTR(OBJ)->SetMember(OBJ, MEMBER, VALUE)
