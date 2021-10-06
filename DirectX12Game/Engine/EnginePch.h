#pragma once

// std::byte ������� ����
#define _HAS_STD_BYTE 0 // �̰� �־��ָ� ��.

/// ǥ�� C++ ���� include
#include <windows.h>
#include <tchar.h>
#include <memory> // unique_ptr, make_unique ����
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

// C++17�� ���Ͻý��� �߰�. (�̸� C++17 �����ؾ���)
#include <filesystem>
namespace fs = std::filesystem; // fs��� �̸��� ������

/// �ܺ� ���̺귯��
/*
	d3dx12.h�� Ư���ϴ�.
	���������� ������� ������ �ƴ϶�,
	�츮�� �ܺο��� �޾ƿ� ������ ����ϴ°�.
	�׷��� �ܺ� ǥ���� " "�� ��������.
	�� ������ ��Ī�� ���� D3D12 Helper Library ��� �θ�.
	
*/
#include "d3dx12.h" // �ܺο��� �ٿ�޾� �߰���.
#include "SimpleMath.h"

/// �߰� ��� ���� ��� �߰�
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

// Texture�� ���� �ܺ� ������� �߰�.
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

// ���� lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// Texture�� ���� �ܺ� lib ���� �߰�.
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

// ���� typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
// ���ο� ���Ϳ� ��ķ� ������.
using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;

// Constant Buffer�� ���� �������� ��ȣ
enum class CBV_REGISTER : uint8
{
	// ����� uint8�� ����. �⺻�� int��.
	b0,
	b1,
	b2,
	b3,
	b4,

	END // �ڰ����� �̿��� �� ����.
}; // MAXCOUNT, COUNT ������ �̿��Ѵٰ� ��.

// �ؽ�ó�� ���� t0~t4
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
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT, // �� �������� ����
};

struct WindowInfo
{
	HWND hwnd; // ��� ������
	int32 width; // �ʺ�
	int32 height; // ����
	bool windowed; // â��� or ��üȭ��
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
	define�� ���ٸ� �ν��ϴ°� �⺻�ε�,
	�������� �ν��Ϸ��� \�� ���� �������� ��������Ѵ�.
*/
/// �̱��� ������ �ڵ�ȭ (type = class �̸�)
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

// �̱����� getter
#define GET_SINGLE(type)	type::GetInstance()


/// ���� ����ϴ� �Լ� ������ ��ũ�η� �����.
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
	Matrix matWVP; // ����
};


// Engine Ŭ���� ��������. ������ �̱��� ����. ���漱����� ������.
extern unique_ptr<class Engine> GEngine; // �ܺο��� ���ٰ���(extern)