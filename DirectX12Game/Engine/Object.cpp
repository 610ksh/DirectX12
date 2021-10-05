#include "pch.h"
#include "Object.h"

// 기본 생성자는 없음. 반드시 오브젝트 타입을 받도록 함.
Object::Object(OBJECT_TYPE type) : _objectType(type)
{

}

Object::~Object()
{

}