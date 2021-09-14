#include "pch.h" // precompiled header ������ ���ؼ� �ڵ����� �߰��� ���
#include "Game.h"
#include "Engine.h"

// �ϳ��� �޽ÿ� ���̴� ��ü�� �ϳ� ������
shared_ptr<Mesh> mesh = make_shared<Mesh>(); // �޽ø� �� 1���� ������
shared_ptr<Shader> shader = make_shared<Shader>();

// Texture�� 1���� �����غ�.
shared_ptr<Texture> texture = make_shared<Texture>();


void Game::Init(const WindowInfo & info)
{
	// ���� �ʱ�ȭ
	GEngine->Init(info);

	/// ���� ����
	// ������ ������ �ϳ��� �ѱ�� ����. �޽ÿ� �ѱ�� ������
	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.f, 0.f); // (0,0)
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.f, 0.f); // (1,0)
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.f, 1.f); // (1,1)
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.f, 1.f); // (0,1)

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	// �޽� �ʱ�ȭ
	mesh->Init(vec, indexVec);

	// shader �ʱ�ȭ -> HLSL ���� �ν�. (VertexShader, PixelShader ����)
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	// texture �ʱ�ȭ
	texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
	
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	/// TODO!
	
	// ���̴��� ���� ������Ʈ
	// �� �����ӿ� �ϳ��� ������������������ ����µ�.
	// �� �������� ������ �ٽ� �ϳ��� ������ ������������ �����.
	shader->Update();
	
	/*
		<Notice>
		���� ��ġ���� ������ �ٸ��� ����Ǳ� �ٶ��ٸ�,
		Transform�� 2���� ���� ���� Mesh���� �ٸ��� �����ϰ�
		�� ���·� PushData�� �Ͽ� Render�Ͽ����� �����ϴ�.
		�Ʒ��� ������ �ϳ��� Transform�� �̿��Ͽ� ��ġ�� ����
		��θ� �ٲٴ°Ŷ�°� �����ϵ��� ����.
	*/
	{
		/// ���� ���� �� ������ �߰�
		Transform t;
		t.offset = Vec4(0.f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->SetTexture(texture); // texture ����
		/// ���������� Render
		mesh->Render();
	}

	GEngine->RenderEnd();
}
