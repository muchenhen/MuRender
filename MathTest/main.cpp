#include <DirectXMath.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>

#include <Eigen/Core>

float dotProductEigen(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2)
{
    return v1.dot(v2);
}

float dotProductDirectXMath(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1);
    DirectX::XMVECTOR xmv2 = DirectX::XMLoadFloat3(&v2);
    DirectX::XMVECTOR result = DirectX::XMVector3Dot(xmv1, xmv2);
    float dotProduct;
    DirectX::XMStoreFloat(&dotProduct, result);
    return dotProduct;
}

Eigen::Vector3f transformPointEigen(const Eigen::Matrix4f& matrix, const Eigen::Vector3f& point)
{
    return (matrix * Eigen::Vector4f(point.x(), point.y(), point.z(), 1.0f)).head<3>();
}

DirectX::XMFLOAT3 transformPointDirectXMath(const DirectX::XMMATRIX& matrix, const DirectX::XMFLOAT3& point)
{
    DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&point);
    vPoint = DirectX::XMVector3Transform(vPoint, matrix);
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, vPoint);
    return result;
}

Eigen::Matrix4f matrixMultiplyEigen(const Eigen::Matrix4f& a, const Eigen::Matrix4f& b)
{
    return a * b;
}

DirectX::XMMATRIX matrixMultiplyDirectXMath(const DirectX::XMMATRIX& a, const DirectX::XMMATRIX& b)
{
    return DirectX::XMMatrixMultiply(a, b);
}

// Eigen-based vertex structure
struct VertexEigen
{
    Eigen::Vector3f Position;
    Eigen::Vector2f UV;
    Eigen::Vector3f Normal;
};

// DirectXMath-based vertex structure
struct VertexDirectX
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT2 UV;
    DirectX::XMFLOAT3 Normal;
};

template <typename VertexType>
class Mesh
{
public:
    std::vector<VertexType> Vertices;
    std::vector<unsigned int> Indices;
};

void TransformVerticesEigen(std::vector<VertexEigen>& vertices, const Eigen::Matrix4f& transform)
{
    for (auto& vertex : vertices)
    {
        Eigen::Vector4f homogeneous(vertex.Position.x(), vertex.Position.y(), vertex.Position.z(), 1.0f);
        vertex.Position = (transform * homogeneous).head<3>();
        vertex.Normal = (transform.block<3, 3>(0, 0) * vertex.Normal).normalized();
    }
}

// Function to transform vertices using DirectXMath
void TransformVerticesDirectX(std::vector<VertexDirectX>& vertices, const DirectX::XMMATRIX& transform)
{
    for (auto& vertex : vertices)
    {
        DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&vertex.Position);
        position = DirectX::XMVector3Transform(position, transform);
        DirectX::XMStoreFloat3(&vertex.Position, position);

        DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&vertex.Normal);
        normal = DirectX::XMVector3TransformNormal(normal, transform);
        normal = DirectX::XMVector3Normalize(normal);
        DirectX::XMStoreFloat3(&vertex.Normal, normal);
    }
}

void Test1()
{
    const int numTests = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    std::vector<Eigen::Vector3f> eigenVectors1(numTests);
    std::vector<Eigen::Vector3f> eigenVectors2(numTests);
    std::vector<DirectX::XMFLOAT3> dxVectors1(numTests);
    std::vector<DirectX::XMFLOAT3> dxVectors2(numTests);

    for (int i = 0; i < numTests; ++i)
    {
        float x = dis(gen), y = dis(gen), z = dis(gen);
        eigenVectors1[i] = Eigen::Vector3f(x, y, z);
        eigenVectors2[i] = Eigen::Vector3f(dis(gen), dis(gen), dis(gen));
        dxVectors1[i] = DirectX::XMFLOAT3(x, y, z);
        dxVectors2[i] = DirectX::XMFLOAT3(dis(gen), dis(gen), dis(gen));
    }

    // 测试Eigen
    auto start = std::chrono::high_resolution_clock::now();
    float eigenSum = 0;
    for (int i = 0; i < numTests; ++i)
    {
        eigenSum += dotProductEigen(eigenVectors1[i], eigenVectors2[i]);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto eigenDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 测试DirectXMath
    start = std::chrono::high_resolution_clock::now();
    float dxSum = 0;
    for (int i = 0; i < numTests; ++i)
    {
        dxSum += dotProductDirectXMath(dxVectors1[i], dxVectors2[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    auto dxDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 计算最长的 duration 值的字符长度
    int maxWidth = std::max(
        std::to_string(eigenDuration.count()).length(),
        std::to_string(dxDuration.count()).length());

    // 输出结果，使用 setw 和 left 来对齐
    std::cout << "Eigen time      : " << std::left << std::setw(maxWidth)
              << eigenDuration.count() << " microseconds\n";
    std::cout << "DirectXMath time: " << std::left << std::setw(maxWidth)
              << dxDuration.count() << " microseconds\n";

    // 计算DirectXMath消耗时间相对Eigen的消耗时间的比例
    std::cout << "DirectXMath is " << static_cast<float>(dxDuration.count()) / eigenDuration.count() << " times faster than Eigen\n";

    // 矩阵乘法测试
    std::vector<Eigen::Matrix4f> eigenMatrices1(numTests);
    std::vector<Eigen::Matrix4f> eigenMatrices2(numTests);
    std::vector<DirectX::XMMATRIX> dxMatrices1(numTests);
    std::vector<DirectX::XMMATRIX> dxMatrices2(numTests);

    for (int i = 0; i < numTests; ++i)
    {
        eigenMatrices1[i] = Eigen::Matrix4f::Random();
        eigenMatrices2[i] = Eigen::Matrix4f::Random();
        dxMatrices1[i] = DirectX::XMMatrixSet(
            dis(gen), dis(gen), dis(gen), dis(gen),
            dis(gen), dis(gen), dis(gen), dis(gen),
            dis(gen), dis(gen), dis(gen), dis(gen),
            dis(gen), dis(gen), dis(gen), dis(gen));
        dxMatrices2[i] = DirectX::XMMatrixSet(
            dis(gen), dis(gen), dis(gen), dis(gen),
            dis(gen), dis(gen), dis(gen), dis(gen),
            dis(gen), dis(gen), dis(gen), dis(gen),
            dis(gen), dis(gen), dis(gen), dis(gen));
    }

    // 测试Eigen矩阵乘法
    start = std::chrono::high_resolution_clock::now();
    Eigen::Matrix4f eigenResult;
    for (int i = 0; i < numTests; ++i)
    {
        eigenResult = matrixMultiplyEigen(eigenMatrices1[i], eigenMatrices2[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    auto eigenMatrixDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 测试DirectXMath矩阵乘法
    start = std::chrono::high_resolution_clock::now();
    DirectX::XMMATRIX dxResult;
    for (int i = 0; i < numTests; ++i)
    {
        dxResult = matrixMultiplyDirectXMath(dxMatrices1[i], dxMatrices2[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    auto dxMatrixDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 空间转换测试
    std::vector<Eigen::Vector3f> eigenPoints(numTests);
    std::vector<DirectX::XMFLOAT3> dxPoints(numTests);
    Eigen::Matrix4f eigenTransform = Eigen::Matrix4f::Random();
    DirectX::XMMATRIX dxTransform = DirectX::XMMatrixSet(
        dis(gen), dis(gen), dis(gen), dis(gen),
        dis(gen), dis(gen), dis(gen), dis(gen),
        dis(gen), dis(gen), dis(gen), dis(gen),
        dis(gen), dis(gen), dis(gen), dis(gen));

    for (int i = 0; i < numTests; ++i)
    {
        eigenPoints[i] = Eigen::Vector3f(dis(gen), dis(gen), dis(gen));
        dxPoints[i] = DirectX::XMFLOAT3(dis(gen), dis(gen), dis(gen));
    }

    // 测试Eigen空间转换
    start = std::chrono::high_resolution_clock::now();
    Eigen::Vector3f eigenTransformResult;
    for (int i = 0; i < numTests; ++i)
    {
        eigenTransformResult = transformPointEigen(eigenTransform, eigenPoints[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    auto eigenTransformDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 测试DirectXMath空间转换
    start = std::chrono::high_resolution_clock::now();
    DirectX::XMFLOAT3 dxTransformResult;
    for (int i = 0; i < numTests; ++i)
    {
        dxTransformResult = transformPointDirectXMath(dxTransform, dxPoints[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    auto dxTransformDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 输出结果
    maxWidth = std::max({std::to_string(eigenDuration.count()).length(),
                         std::to_string(dxDuration.count()).length(),
                         std::to_string(eigenMatrixDuration.count()).length(),
                         std::to_string(dxMatrixDuration.count()).length(),
                         std::to_string(eigenTransformDuration.count()).length(),
                         std::to_string(dxTransformDuration.count()).length()});

    std::cout << "Dot Product:\n";
    std::cout << "Eigen time      : " << std::left << std::setw(maxWidth) << eigenDuration.count() << " microseconds\n";
    std::cout << "DirectXMath time: " << std::left << std::setw(maxWidth) << dxDuration.count() << " microseconds\n";
    std::cout << "DirectXMath is " << static_cast<float>(dxDuration.count()) / eigenDuration.count() << " times faster than Eigen\n\n";

    std::cout << "Matrix Multiplication:\n";
    std::cout << "Eigen time      : " << std::left << std::setw(maxWidth) << eigenMatrixDuration.count() << " microseconds\n";
    std::cout << "DirectXMath time: " << std::left << std::setw(maxWidth) << dxMatrixDuration.count() << " microseconds\n";
    std::cout << "DirectXMath is " << static_cast<float>(dxMatrixDuration.count()) / eigenMatrixDuration.count() << " times faster than Eigen\n\n";

    std::cout << "Point Transformation:\n";
    std::cout << "Eigen time      : " << std::left << std::setw(maxWidth) << eigenTransformDuration.count() << " microseconds\n";
    std::cout << "DirectXMath time: " << std::left << std::setw(maxWidth) << dxTransformDuration.count() << " microseconds\n";
    std::cout << "DirectXMath is " << static_cast<float>(dxTransformDuration.count()) / eigenTransformDuration.count() << " times faster than Eigen\n";
}

void Test2()
{
    const int numVertices = 1000000;
    const int numTests = 100;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Initialize Eigen mesh
    Mesh<VertexEigen> eigenMesh;
    eigenMesh.Vertices.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        eigenMesh.Vertices.push_back({Eigen::Vector3f(dis(gen), dis(gen), dis(gen)),
                                      Eigen::Vector2f(dis(gen), dis(gen)),
                                      Eigen::Vector3f(dis(gen), dis(gen), dis(gen)).normalized()});
    }

    // Initialize DirectXMath mesh
    Mesh<VertexDirectX> dxMesh;
    dxMesh.Vertices.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        dxMesh.Vertices.push_back({
            DirectX::XMFLOAT3(dis(gen), dis(gen), dis(gen)),
            DirectX::XMFLOAT2(dis(gen), dis(gen)),
            DirectX::XMFLOAT3(dis(gen), dis(gen), dis(gen)) // Will be normalized in the transform function
        });
    }

    // Create transform matrices
    Eigen::Matrix4f eigenTransform = Eigen::Matrix4f::Random();
    DirectX::XMMATRIX dxTransform = DirectX::XMMatrixSet(
        eigenTransform(0, 0), eigenTransform(0, 1), eigenTransform(0, 2), eigenTransform(0, 3),
        eigenTransform(1, 0), eigenTransform(1, 1), eigenTransform(1, 2), eigenTransform(1, 3),
        eigenTransform(2, 0), eigenTransform(2, 1), eigenTransform(2, 2), eigenTransform(2, 3),
        eigenTransform(3, 0), eigenTransform(3, 1), eigenTransform(3, 2), eigenTransform(3, 3));

    // Test Eigen performance
    std::cout << "Testing Eigen performance:\n";
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numTests; ++i)
    {
        TransformVerticesEigen(eigenMesh.Vertices, eigenTransform);
        // 显示进度
        std::cout << "\rProgress: " << std::setw(3) << (i + 1) * 100 / numTests << "%" << std::flush;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto eigenDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::endl; // 换行

    // Test DirectXMath performance
    std::cout << "Testing DirectXMath performance:\n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numTests; ++i)
    {
        TransformVerticesDirectX(dxMesh.Vertices, dxTransform);
        // 显示进度
        std::cout << "\rProgress: " << std::setw(3) << (i + 1) * 100 / numTests << "%" << std::flush;
    }
    end = std::chrono::high_resolution_clock::now();
    auto dxDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::endl; // 换行

    // Output results
    std::cout << "Eigen time      : " << eigenDuration.count() << " microseconds\n";
    std::cout << "DirectXMath time: " << dxDuration.count() << " microseconds\n";
    std::cout << "DirectXMath is " << static_cast<float>(eigenDuration.count()) / dxDuration.count() << " times faster than Eigen\n";
}

int main()
{
    Test2();

    system("pause");

    return 0;
}