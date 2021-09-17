#include "pch.h" // precompiled header ������ ���ؼ� �ڵ����� �߰��� ���
#include "Game.h"
#include "Engine.h"
#include "Material.h"

// 1���� �޽ø� ����. ��������. (�ϳ��� ������Ʈ ����)
shared_ptr<Mesh> mesh = make_shared<Mesh>();

void Game::Init(const WindowInfo & info)
{
	/// 1. ���� �ʱ�ȭ
	/*
		1. ��ü ���� ����� ���� ��� Ŭ���� ���� ���� �� �ʱ�ȭ
		2. 2���� Constant Buffer ����
		- Transform�� ���� b0 �������� ���. ����
		- Material�� ���� b1 �������� ���. ����
	*/
	GEngine->Init(info);

	/// 2. Vertex ���� ����
	// �޽� �ϳ��� ����� ���� ���� ����.
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

	/// 3. Index ���� ����
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

	/// 4. Mesh �ʱ�ȭ. (����, �ε��� ���� �ʿ�)
	mesh->Init(vec, indexVec);

	/// 5. Shader, Texture ���� �� �ʱ�ȭ
	// ���̴�, �ؽ�ó ���� ����
	shared_ptr<Shader> shader = make_shared<Shader>();
	shared_ptr<Texture> texture = make_shared<Texture>();
	// shader, texture �ʱ�ȭ, HLSL ���� �ν�. (VertexShader, PixelShader ����)
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
	
	/// 6. Material ����
	shared_ptr<Material> material = make_shared<Material>();
	material->SetShader(shader); // ����� ���̴� ����
	material->SetFloat(0, 0.3f); // b1 ������ float_0
	material->SetFloat(1, 0.4f); // b1 ������ float_1
	material->SetFloat(2, 0.3f); // b1 ������ float_2
	material->SetTexture(0, texture); // tex_0(t0)�� ���̰� �ؽ�ó�� �ѱ�

	/// 7. ���������� ������ ���� Material�� mesh�� ������
	mesh->SetMaterial(material); 

	/// 8. ���� ������ ���δ� �Ϸ�ɶ����� ��� ���. 
	// CPU�� GPU�� ������ ����ȭ ����
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	// �Ǿտ� �ϴ� ����.
	GEngine->Update();
	// ��ü ������ �ʱ�ȭ �� ���� �غ�ܰ�.
	// ������ ���� �κп��� �ʱ�ȭ�� �ʿ��� �۾��� RenderBegin���� �Ѵ�.
	// ����� _cmdQueue->RenderBegin(&_viewport, &_scissorRect) �ڵ� ���̴�.
	GEngine->RenderBegin();

	/// TODO!
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
		static Transform t = {}; // �ѹ� �����ϸ� ������ ������.
		
		/// Ű���� �Է°� ó�� (x,y ����)
		// Ű�� ������ �̵��ϰԵ�. 1��.
		// �ٵ� ���࿡ �츮�� W�� 1�ʰ� ������, 1�ʵ��� �������� 100�� ���ư���
		// ���ڱ� 100�� �̵��ϰ� �ȴ�.
		if (INPUT->GetButton(KEY_TYPE::W))
			t.offset.y += 1.f * DELTA_TIME;
		if (INPUT->GetButton(KEY_TYPE::S))
			t.offset.y -= 1.f * DELTA_TIME;
		if (INPUT->GetButton(KEY_TYPE::A))
			t.offset.x -= 1.f * DELTA_TIME;
		if (INPUT->GetButton(KEY_TYPE::D))
			t.offset.x += 1.f * DELTA_TIME;

		/// Transform ���� (���� ����)
		mesh->SetTransform(t);
		
		/// ���������� Render
		// Transform ������ Material ������ ��� ��.
		mesh->Render(); 
	}

	/*{
		/// ���� ���� �� ������ �߰�
		Transform t;
		t.offset = Vec4(0.25f, 0.25f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->SetTexture(texture); // texture ����

		/// ���������� Render
		mesh->Render();
	}*/

	// ��ü ������ �۾� �Ϸ�.
	// ���������� ���� ���������� �������� �����⸦ ��ٸ���. WaitSync();
	// �����_cmdQueue->RenderEnd() �ڵ� ���̴�.
	GEngine->RenderEnd();
}
