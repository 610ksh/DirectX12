#pragma once

// std::byte 사용하지 않음
#define _HAS_STD_BYTE 0 // 이거 넣어주면 됨.

/// 표준 C++ 각종 include
#include <windows.h>
#include <tchar.h>
#include <memory> // unique_ptr, make_unique 있음
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

// C++17의 파일시스템 추가. (미리 C++17 설정해야함)
#include <filesystem>
namespace fs = std::filesystem; // fs라고 이름을 줄이자

/// 외부 라이브러리
/*
	d3dx12.h는 특별하다.
	공식적으로 만들어진 파일이 아니라,
	우리가 외부에서 받아온 파일을 사용하는것.
	그래서 외부 표시인 " "이 적혀있음.
	이 파일의 명칭을 보통 D3D12 Helper Library 라고 부름.
	
*/
#include "d3dx12.h" // 외부에서 다운받아 추가함.
#include "SimpleMath.h"

/// 추가 기능 관련 헤더 추가
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

// Texture를 위한 외부 헤더파일 추가.
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// Texture를 위한 외부 lib 파일 추가.
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

// 각종 typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
// 새로운 벡터와 행렬로 수정함.
using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;

// Constant Buffer를 위한 레지스터 번호
enum class CBV_REGISTER : uint8
{
	// 사이즈를 uint8로 줄임. 기본은 int임.
	b0,
	b1,
	b2,
	b3,
	b4,

	END // ★개수로 이용할 수 있음.
}; // MAXCOUNT, COUNT 등으로 이용한다고 함.

// 텍스처를 위한 t0~t4
enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,

	END
}; 

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT, // 총 레지스터 개수
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
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
};

/*
	define은 한줄만 인식하는게 기본인데,
	여러줄을 인식하려면 \을 각줄 마지막에 적어줘야한다.
*/
/// 싱글톤 패턴을 자동화 (type = class 이름)
#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

// 싱글톤의 getter
#define GET_SINGLE(type)	type::GetInstance()


/// 자주 사용하는 함수 패턴을 매크로로 묶어둠.
#define DEVICE GEngine->GetDevice()->GetDevice()
#define	CMD_LIST GEngine->GetCmdQueue()->GetCmdList()
#define RESOURCE_CMD_LIST	GEngine->GetCmdQueue()->GetResourceCmdList()
#define ROOT_SIGNATURE GEngine->GetRootSignature()->GetSignature()

#define INPUT				GET_SINGLE(Input)
#define DELTA_TIME			GET_SINGLE(Timer)->GetDeltaTime()

#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)


struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP; // 최종
};


// Engine 클래스 전역변수. 일종의 싱글톤 형태. 전방선언까지 해주자.
extern unique_ptr<class Engine> GEngine; // 외부에서 접근가능(extern)