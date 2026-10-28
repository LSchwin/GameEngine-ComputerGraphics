#include "Engine/Engine.h"

#include "Renderer/Shader.h"
#include "Renderer/Pipeline.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#define CHECK_UNIFORM_BOUNDRY(T) static_assert((sizeof(T) % 16) == 0);



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

    auto model = Resources().Get<Model>("models/spot.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/spot_diffuse.png", Engine::Get().GetRenderer());

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
    CHECK_UNIFORM_BOUNDRY(vertexUniform)


    struct LightUniform
    {
        glm::vec3 position;
        uint32_t type;
        glm::vec3 direction;
        float range;
        glm::vec3 color;
        float intensity;
        glm::vec3 ambient;
        float innerCutoff;
        float outerCutoff;
        glm::vec3 padding;
    };
    CHECK_UNIFORM_BOUNDRY(LightUniform)

    LightUniform lightUniform{
        .position{ 10, 10, 10},
        .type{ (uint32_t)LightType::Point },
        .direction{0, -1, 0},
        .range{ 40 },
        .color{1, 1, 1},
        .intensity{ 1 },
        .ambient{ 0.2f },
        .innerCutoff{ 0.3f },
        .outerCutoff{ 0.6f }
    };

    struct MaterialUniform
    {
        glm::vec4 color;
        glm::vec2 tiling;
        glm::vec2 offset;
        float shininess;
        glm::vec3 padding;
    };
    CHECK_UNIFORM_BOUNDRY(MaterialUniform)

    MaterialUniform materialUniform
    {
        .color{ 1, 1, 1, 1},
        .tiling{ 1.0f, 1.0f },
        .offset{ 0.0f, 0.0f},
        .shininess{ 64.0f }
    };


    vertexUniform.cameraPosition = glm::vec3{ 0, 0, 5 };
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

        //tint.g = sin(time) * 0.5f + 0.5f;

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1.0f;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = 1.0f;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.y = -1.0f;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.y = 1.0f;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.z = -1.0f;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.z = 1.0f;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_R)) vertexUniform.cameraPosition = glm::vec3{ 0, 0, 5 };
        vertexUniform.cameraPosition += direction * 4.0f * dt;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1))
        {
            lightUniform.type = (uint32_t)LightType::Point;
            lightUniform.position = glm::vec3{ 10, 10, 10 };
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) 
        {
            lightUniform.type = (uint32_t)LightType::Directional;
            lightUniform.position = glm::vec3{ 10, 10, 10 };
        }
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3))
        {
            lightUniform.type = (uint32_t)LightType::Spot;
            lightUniform.position = glm::vec3{ 0, 0, 0 };
            lightUniform.innerCutoff = glm::cos(glm::radians(30.0f));
            lightUniform.outerCutoff = glm::cos(glm::radians(60.0f));
        }

        //materialUniform.offset.x = time;
        //materialUniform.tiling = glm::vec2{ 2.0f };

        
        // TRANSFORM
        transform.rotation.x = time * 90;

        // VIEW
        auto view = glm::lookAtRH(vertexUniform.cameraPosition, vertexUniform.cameraPosition + glm::vec3{ 0, 0, -1 }, glm::vec3{0, 1, 0});

        // PROJECTION
        float aspectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 20.0f);

        // MVP
        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;
  

        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentUniform(0, &vertexUniform, sizeof(vertexUniform)); //phong
        Engine::Get().GetRenderer().SetFragmentUniform(1, &lightUniform, sizeof(lightUniform)); //phong
        Engine::Get().GetRenderer().SetFragmentUniform(2, &materialUniform, sizeof(materialUniform)); //phong (for gouraud)
        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture); //cow

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    
    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}
