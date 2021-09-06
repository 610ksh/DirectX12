#pragma once

/*
	#pragma comment(lib, ) : 전처리 단계에서 코드를 읽어서 추가 종속성에 추가하는 역할.
	Engine.lib를 추가해준다.
	이 헤더를 포함, 참조하는 파일에서는 Engine.lib를 사용할 수 있게됨.
	자동으로 링커 단계에서 해당 외부 라이브러리를 참조함.
	이 방법을 안쓰면, 프로젝트 속성의 링커 -> 추가종속성 부분에서 일일이 라이브러리들을 추가해줘야함.
*/

#pragma comment(lib, "Engine.lib")

#include "EnginePch.h"