#include "Engine/Engine.h"

#include "Renderer/Shader.h"
#include "Renderer/Pipeline.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include "Framework/Scene.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "Components/CameraComponent.h"

#define CHECK_UNIFORM_BOUNDRY(T) static_assert((sizeof(T) % 16) == 0);

#define MAX_LIGHTS 8

using namespace nu;

enum class LightType : uint32_t
{
    Directional,
    Point,
    Spot
};

int main()
{
    //NO TOUCHY
    SetWorkingDirectory("assets"); 
    //NO TOUCHY
    
    // INITIALIZATION
    Engine::Get().Initialize();

    Scene scene;
    scene.Load("scenes/scene.json");

    // TESTING
    auto model = Resources().Get<Model>("models/ogre.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/ogre_diffuse.bmp", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/phong.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<nu::Shader>("shaders/phong.frag", Engine::Get().GetRenderer(), 3, 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    //TESTING
    //UNIFORMS
    struct vertexUniform
    {
        glm::mat4 mvp;
        glm::mat4 model;
        glm::vec3 cameraPosition;
        float padding1;
    };
    vertexUniform vertexUniform;
    CHECK_UNIFORM_BOUNDRY(vertexUniform);

    struct Light
    {
        glm::vec3 position;
        uint32_t type;
        glm::vec3 direction;
        float range;
        glm::vec3 color;
        float intensity;
        float innerCutoff;
        float outerCutoff;
        glm::vec2 padding;
    };

    struct LightUniform
    {
        Light lights[MAX_LIGHTS];
        uint32_t numLights;
        glm::vec3 ambient;
    };
    CHECK_UNIFORM_BOUNDRY(LightUniform);
    LightUniform lightUniform;

    Light light1{
        .position{ 10, 10, 10},
        .type{ (uint32_t)LightType::Point },
        .direction{0, -1, 0},
        .range{ 40 },
        .color{1, 0, 0},
        .intensity{ 1 },
        .innerCutoff{ 0.3f },
        .outerCutoff{ 0.6f }
    };

    Light light2{
        .position{ -10, 10, 10},
        .type{ (uint32_t)LightType::Point },
        .direction{0, -1, 0},
        .range{ 40 },
        .color{0, 1, 0},
        .intensity{ 1 },
        .innerCutoff{ 0.3f },
        .outerCutoff{ 0.6f }
    };

    Light light3{
        .position{ 0, 10, 10},
        .type{ (uint32_t)LightType::Directional },
        .direction{0, -1, 0},
        .range{ 40 },
        .color{0, 0, 1},
        .intensity{ 1 },
        .innerCutoff{ 0.3f },
        .outerCutoff{ 0.6f }
    };

    lightUniform.lights[0] = light1;
    lightUniform.lights[1] = light2;
    lightUniform.lights[2] = light3;
    lightUniform.numLights = 3;
    lightUniform.ambient = glm::vec3{ 0.2f };


    struct MaterialUniform
    {
        glm::vec4 color;
        glm::vec2 tiling;
        glm::vec2 offset;
        float shininess;
        glm::vec3 padding;
    };
    CHECK_UNIFORM_BOUNDRY(MaterialUniform);

    MaterialUniform materialUniform
    {
        .color{ 1, 1, 1, 1},
        .tiling{ 1.0f, 1.0f },
        .offset{ 0.0f, 0.0f},
        .shininess{ 32.0f }
    };
    //TESTING
    

    Transform transform;


    // MAIN LOOP
    bool quit = false;
    while (!quit) {
        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }
        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        // SCENE
        scene.Update(dt);


        //lightUniform.lights[0].position = glm::eulerAngleY(time) * glm::vec4{0, 0, 10, 1};
        //lightUniform.lights[1].position = glm::eulerAngleY(time * 0.5) * glm::vec4{0, 0, 10, 1};

        
        // TRANSFORM
        //transform.rotation.x = time * 90;
 
        // Camera
        auto camera = scene.GetActorByName("Camera");
        auto cameraComponent = camera->GetComponent<CameraComponent>();
        vertexUniform.cameraPosition = camera->GetTransform().position;

        // MVP
        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = cameraComponent->GetProjection() * cameraComponent->GetView() * vertexUniform.model;
  

        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Object, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Material, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Object, &vertexUniform, sizeof(vertexUniform)); //phong
        Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform)); //phong
        Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Material, &materialUniform, sizeof(materialUniform)); //phong (for gouraud) 0 for gouraud and 2 for phong

        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture); //model

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    
    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}
