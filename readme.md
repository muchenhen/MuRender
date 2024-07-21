1、 Texture类

这个类用于加载图片，保存图片信息，后续逐步扩展为各种纹理功能

2、 Mesh类

用于定义面片，目前保存了顶点信息和对应索引信息，拥有两个成员

std::vector<Vertex> Vertices;
std::vector<unsigned int> Indices;

其中Vertex的结构信息是
struct Vertex
{
    Eigen::Vector3f Position;
    Eigen::Vector2f UV;
};


3、 Material类

    Eigen::Vector3f BaseColor;
    std::shared_ptr<Texture> TexturePtr;

拥有两个成员 一个是默认输出的基本颜色，一个是纹理指针，指向一个Texture对象实例

4、 MeshObject类

    std::shared_ptr<Mesh> MeshPtr;
    std::shared_ptr<Material> MaterialPtr;

拥有两个成员，一个是Mesh指针，一个是Material指针，用于保存一个物体的信息。这个物体认为是拥有一个Mesh和一个Material的组合，类似虚幻引擎的UStaticMesh。

MeshObject继承自Object类，天生具备位置信息，提供了一个GetModelMatrix的接口，用于获取当前物体的模型矩阵。

5、 Scene类

    std::vector<std::unique_ptr<Object>> Objects;

    std::vector<std::unique_ptr<Camera>> Cameras;

场景类，定义了一个场景，场景中可以摆放多个物体，多个相机，类似虚幻引擎编辑器下Level的概念，主要是用于方便理解和管理当前要渲染的物体和相机。

6、 Camera类

    float Fov;
    float Aspect;
    float NearPlane;
    float FarPlane;
    Eigen::Vector3f Up;
    Eigen::Vector3f Target;

拥有常规摄像机类的参数，用于定义一个摄像机的视角，方便渲染时使用。提供获取ViewMatrix和ProjectionMatrix的接口。

7、 Renderer类

    int ScreenWidth;
    int ScreenHeight;
    std::vector<uint32_t> FrameBuffer;
    std::vector<float> DepthBuffer;

Renderer类保存了目标渲染的屏幕大小，提供了一个FrameBuffer和一个DepthBuffer，用于保存渲染的结果，提供了一个Render函数，用于渲染场景。

```c++
class Renderer
{
private:
    int ScreenWidth;
    int ScreenHeight;
    std::vector<uint32_t> FrameBuffer;
    std::vector<float> DepthBuffer;

public:
    Renderer(int Width, int Height);

    void Clear(uint32_t Color = 0);

    const std::vector<uint32_t>& GetFrameBuffer() const;

    int GetWidth() const;

    int GetHeight() const;

private:
    Eigen::Vector3f ComputeBarycentric(int X, int Y, int X0, int Y0, int X1, int Y1, int X2, int Y2);

    bool IsInsideTriangle(int X, int Y, int X0, int Y0, int X1, int Y1, int X2, int Y2);

    uint32_t ColorToUint32(const Eigen::Vector3f& Color);

public:
    void DrawPixel(int X, int Y, uint32_t Color);

    void DrawPixel(int X, int Y, float Depth, uint32_t Color);

    void DrawLine(int X1, int Y1, int X2, int Y2, uint32_t Color);

    void DrawTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, uint32_t Color);

    void FillTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, uint32_t Color);

    void FillTriangle(int X0, int Y0, float Z0, uint32_t Color0,
                      int X1, int Y1, float Z1, uint32_t Color1,
                      int X2, int Y2, float Z2, uint32_t Color2);

public:
    void RenderCamera(const Scene& Scene, const Camera& Camera);

    // void RenderMeshObject(const MeshObject& MeshObject, const Camera& Camera);
};
```

以上是我当前的主要类型和相关的定义，目前我进行渲染的流程如下：
```c++

G_Scene->AddCamera(std::move(CameraPtr));

std::shared_ptr<Texture> TexturePtr = std::make_shared<Texture>();
TexturePtr->LoadFromFile("..\\Resource\\NagisaKaworu.bmp");

std::shared_ptr<Material> MaterialPtr = std::make_shared<Material>();
MaterialPtr->SetTexture(TexturePtr);

std::unique_ptr<Cube> CubePtr = std::make_unique<Cube>(2.0f);
CubePtr->SetMaterial(MaterialPtr);

G_Scene->AddObject(std::move(CubePtr));

```

但是由于实际上还没有把贴图采样的相关逻辑加入到渲染器中，所以目前的渲染结果是一个没有贴图的立方体。

后续势必是要增加光照模型、贴图采样、阴影等功能，这些功能的实现会逐步加入到渲染器中，目前的目标是实现一个简单的光照模型，然后再逐步增加更多的功能。

基于以上信息，我现在需要如何调整我的项目结构来更好的支持我的目标呢？
