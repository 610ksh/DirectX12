#pragma once

// 각종 include
#include <windows.h>
#include <tchar.h>
#include <memory> // unique_ptr, make_unique 있음
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

/*
	d3dx12.h는 특별하다.
	공식적으로 만들어진 파일이 아니라,
	우리가 외부에서 받아온 파일을 사용하는것.
	그래서 외부 표시인 " "이 적혀있음.
	이 파일의 명칭을 보통 D3D12 Helper Library 라고 부름.
	
*/
#include "d3dx12.h" // 외부에서 다운받아 추가함.

#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// 각종 typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using Vec2 = XMFLOAT2;
using Vec3 = XMFLOAT3;
using Vec4 = XMFLOAT4;
using Matrix = XMMATRIX;


enum class CBV_REGISTER
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END // ★개수로 이용할 수 있음.
}; // MAXCOUNT, COUNT 등으로 이용한다고 함.


enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,

	// 일단은 CBV_REGISTER값으로 똑같이 사용하자. 달라질 수 있음.
	REGISTER_COUNT = CBV_REGISTER::END, // 총 레지스터 개수
};

struct WindowInfo
{
	HWND hwnd; // 출력 윈도우
	int32 width; // 너비
	int32 height; // 높이
	bool windowed; // 창모드 or 전체화면
};

struct Vertex
{
	Vec3 pos;
	Vec4 color;
};

// 물체의 정보
struct Transform
{
	// float 4개
	Vec4 offset;
};

// 자주 사용하는 함수 패턴을 매크로로 묶자
#define DEVICE GEngine->GetDevice()->GetDevice()
#define	CMD_LIST GEngine->GetCmdQueue()->GetCmdList()
#define ROOT_SIGNATURE GEngine->GetRootSignature()->GetSignature()

// Engine 클래스 전역변수. 일종의 싱글톤 형태. 전방선언까지 해주자.
extern unique_ptr<class Engine> GEngine; // 외부에서 접근가능(extern)