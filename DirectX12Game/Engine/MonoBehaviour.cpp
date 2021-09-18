#include "pch.h"
#include "MonoBehaviour.h"

// 자신의 부모인 Component에 있는 Component type에 MONO_BEHAVIOUR를 지정함.
MonoBehaviour::MonoBehaviour() : Component(COMPONENT_TYPE::MONO_BEHAVIOUR)
{
	// 생성자에서 Component 생성자 호출해야함.

}

MonoBehaviour::~MonoBehaviour()
{

}