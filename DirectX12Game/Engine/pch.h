// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

// 보편적인 header guard 내용
#ifndef PCH_H // 만약 PCH_H가 define 되어 있지 않다면 아래 내용을 사용.

#define PCH_H // PCH_H 선언 -> 중복을 막는 헤더 가드

// 여기에 미리 컴파일하려는 헤더 추가
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "EnginePch.h" // 엔진 프로젝트 내에서 pch.h를 포함하는 곳에는 모두 EnginePch도 들어가게됨.

#endif //PCH_H
