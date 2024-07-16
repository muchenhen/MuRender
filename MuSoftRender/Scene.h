#include "Object.h"
#include "Render.h"

class Scene
{
private:
    std::vector<std::unique_ptr<Object>> objects;

public:
    void AddObject(std::unique_ptr<Object> obj)
    {
        objects.push_back(std::move(obj));
    }

    void Update(float deltaTime)
    {
        for (auto& obj : objects)
        {
            obj->Update(deltaTime);
        }
    }
};