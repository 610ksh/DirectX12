# ๐DirectX12
- DirectX12์ ๊ด๋ จ๋ ํ์ต ๋ด์ฉ์๋๋ค.

- ์ต์ข ๊ฒฐ๊ณผ (์ถํ์ ์ถ๊ฐํ  ์์ ์๋๋ค)ใ
![]()
#

## ๐ณ ๋ชฉ์ฐจ
1. DirectX12 Initialization
2. Component
3. Vector & Matrix
4. Camera & Lighting
5. Quaternion
6. Rendering
7. Particle
8. Shadow Mapping
9. Terrian
10. Mesh & Animation

## โ TODO
- [x] Start project (2021.06.07, 2021.09.06)
- [x] ์ต์ด ํ๋ก์ ํธ ์ค์  ๋ฐ ์ํ (2021.06.07) / https://blog.naver.com/610ksh/222388342098
- [x] ์ฅ์น ์ด๊ธฐํ (06.08) / https://blog.naver.com/610ksh/222389757864
- [x] ์ผ๊ฐํ ๋์ฐ๊ธฐ (2021.09.08) / https://blog.naver.com/610ksh/222392496872
- [x] Constant Buffer View(CBV) (09.09) / https://blog.naver.com/610ksh/222415510639
- [x] Descriptor Table(DescriptorHeap) (09.10) / https://blog.naver.com/610ksh/222417967521
- [x] Index Buffer (09.13) / https://blog.naver.com/610ksh/222418610083
- [x] Texture Mapping (09.14) / https://blog.naver.com/610ksh/222505298506
- [x] Depth Stencil View (09.15) / https://blog.naver.com/610ksh/222506519945
- [x] Input & Timer (09.16) / https://blog.naver.com/610ksh/222508029404
- [x] Material (09.17) / https://blog.naver.com/610ksh/222509237814
- [x] Component (09.18) / https://blog.naver.com/610ksh/222509738386
- [x] Scene (09.19) / https://blog.naver.com/610ksh/222510823338
- [x] Trigonometry & Vector & Matrix (09.21) / https://blog.naver.com/610ksh/222512188442
- [x] Coordinate Transformation(World, View Matrix) (09.24) / https://blog.naver.com/610ksh/222513116337
- [x] Projection, Screen Transformation (09.25) / https://blog.naver.com/610ksh/222516565902
- [x] Camera (09.27, 28) / https://blog.naver.com/610ksh/222518630108
- [x] Resource (10.05) / https://blog.naver.com/610ksh/222527858327
- [x] Lighting (10.06) / https://blog.naver.com/610ksh/222528751409
- [x] Normal Mapping (10.07) / https://blog.naver.com/610ksh/222530689058
- [x] Skybox (10.08) / https://blog.naver.com/610ksh/222531160911
- [x] Frustum Culling (10.09) / https://blog.naver.com/610ksh/222531953169
- [x] Quaternion (10.10) / https://blog.naver.com/610ksh/222532786353
- [x] Orthographic projection (11.02) / https://blog.naver.com/610ksh/222556372871
- [x] Render Target (Multiple Render Target, = MRT)) (11.04) / https://blog.naver.com/610ksh/222558947412
- [x] Deferred Rendering (11.05) / https://blog.naver.com/610ksh/222559311395

<!--
- [ ] Compute Shader ()
- [ ] Particle System ()
- [ ] Instancing ()
- [ ] Shadow Mapping ()
- [ ] Tessellation ()
- [ ] Terrain ()
- [ ] Picking ()
- [ ] Mesh ()
- [ ] Animation ()
-->


## :heavy_check_mark: Languages & Library
- Visual Studio 2017
- C++
- DirectX12
- d3dx12.h (Helper ํด๋์ค) / https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/Libraries/D3D12RaytracingFallback/Include/d3dx12.h
- DirectXTex (MS, ์ด๋ฏธ์ง ํ์ผ ์๋ก๋ ๋ผ์ด๋ธ๋ฌ๋ฆฌ) / https://github.com/microsoft/DirectXTex
- dx12 SimpleMath.h / https://github.com/microsoft/DirectXTK12
- gitignore : Visual Studio, C++

---
# ๐จChapter1. DirectX12 ์ด๊ธฐํ
## 1. ํ๋ก์ ํธ ์ํ & ์ฅ์น ์ด๊ธฐํ ์๋ฃ(์ด๊ธฐํ๋ฉด)
![Image](Git_Resources/Chapter1/์ด๊ธฐํ.png)

## 2. ์ผ๊ฐํ ๋์ฐ๊ธฐ (VertexBuffer, VertexBufferView = VBV)
![Image](Git_Resources/Chapter1/triangle.png)

## 3. Constant Buffer (ConstantBufferView)
![Image](Git_Resources/Chapter1/triangle2.png)
![Image](Git_Resources/Chapter1/triangle2-2.png)

## 4. Descriptor table
![Image](Git_Resources/Chapter1/TableDescriptorHeap.png)

## 5. Index Buffer (IndexBufferView)
![Image](Git_Resources/Chapter1/IndexBuffer.png)

## 6. Texture Mapping
![Image](Git_Resources/Chapter1/TextureMapping.png)

## 7. Depth Stencil View (Depth Buffer)
![Image](Git_Resources/Chapter1/DepthStencilView.png)


---
# ๐จChapter2. Component
## 2-1. Input & Timer
![Image](Git_Resources/Chapter2/Input&Timer.gif)

## 2-2. Material
![Image](Git_Resources/Chapter2/Material.png)

## 2-3. Component
![Image](Git_Resources/Chapter2/Component.png)

## 2-4. Scene
![Image](Git_Resources/Chapter2/Scene.png)

---
# ๐จChapter3. Vector & Matrix
### 3-1. ์ถ๊ฐ ์์ 
![]()

### 3-2. ์ถ๊ฐ ์์ 
![]()

### 3-3. ์ถ๊ฐ ์์ 
![]()

---
# ๐จChapter4. Camera & Lighting
## 4-1. Camera
![Image](Git_Resources/Chapter4/Camera.gif)

## 4-2. Resource
![Image](Git_Resources/Chapter4/Resources.gif)

## 4-3. Lighting
![Image](Git_Resources/Chapter4/Lighting.gif)

## 4-4. Normal Mapping
![Image](Git_Resources/Chapter4/Normal_Mapping.png)

## 4-5. Skybox
![Image](Git_Resources/Chapter4/Skybox.gif)

## 4-6. Frustum Culling
![Image](Git_Resources/Chapter4/Frustum_Culling.png)

---
# ๐จChapter5. Quaternion
## 1. ์ค์ผ๋ฌ๊ฐ๊ณผ ์ง๋ฒ๋ฝ ํ์
![Image](Git_Resources/Chapter5/์ง๋ฒ๋ฝ.png)

## 2. 2์ฐจ์ Quaternion
![Image](Git_Resources/Chapter5/Quaternion2.png)

## 3. 3์ฐจ์ Quaternion
![Image](Git_Resources/Chapter5/Quaternion.png)
![Image](Git_Resources/Chapter5/Quaternion3.png)

---
# ๐จChapter6. Rendering
## 1. Orthographic Projection (for UI)
![Image](Git_Resources/Chapter6/OrthographicProjection.gif)

## 2. Multiple Render Target(MRT)
![Image](Git_Resources/Chapter6/MultipleRenderTarget.gif)

## 3. Deferred Rendering 
![Image](Git_Resources/Chapter6/DeferredRendering.gif)

<!--
---
# ๐จChapter7. Particle
## Lecture1. ์์ํ๊ธฐ
### 1. x
![]()

### 2. x
![]()

### 3. x
![]()

---
# ๐จChapter8. Shadow Mapping
## Lecture1. ์์ํ๊ธฐ
### 1. x
![]()

### 2. x
![]()

### 3. x
![]()

---
# ๐จChapter9. Terrian
## Lecture1. ์์ํ๊ธฐ
### 1. x
![]()

### 2. x
![]()

### 3. x
![]()

---
# ๐จChapter10. Mesh & Animation
## Lecture1. ์์ํ๊ธฐ
### 1. x
![]()

### 2. x
![]()

### 3. x
![]()

---

-->


## MIT License
The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright 2021. [SungHoon](https://github.com/610ksh) all rights reserved.
