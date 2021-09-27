#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	// �θ� Component�� �Լ��� ȣ��
	GetTransform()->PushData();

	_material->PushData();
	_mesh->Render();
}