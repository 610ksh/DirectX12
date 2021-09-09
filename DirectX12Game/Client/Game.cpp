#include "pch.h" // precompiled header ������ ���ؼ� �ڵ����� �߰��� ���
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo & info)
{
	GEngine->Init(info);

	vector<Vertex> vec(3);
	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	// �޽� �ʱ�ȭ
	mesh->Init(vec);

	// shader �ʱ�ȭ
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	/// TODO!
	shader->Update(); // ���̴��� ���� ������Ʈ

	{
		// Vec4 ¥�� offset ���� 1���ִ� ����ü
		Transform t;
		// �ﰢ���� x�� ��ġ�� 0.75 Į�� red���� 0.75�� ����������
		t.offset = Vec4(0.75f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->Render();
	}

	{
		// Vec4 ¥�� offset ���� 1���ִ� ����ü
		Transform t;
		// �ﰢ���� y�� ��ġ�� 0.75 Į�� green���� 0.75�� ����������
		t.offset = Vec4(0.0f, 0.75f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->Render();
	}

	GEngine->RenderEnd();
}
