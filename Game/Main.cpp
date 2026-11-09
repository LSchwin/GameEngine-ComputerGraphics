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


using namespace nu;


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

    

    auto& renderer = Engine::Get().GetRenderer();

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

        //RENDER
        renderer.BeginFrame();

        renderer.SetPipeline(*pipeline);
        renderer.SetFragmentTexture(0, *texture); //model

        scene.Draw(renderer);

        Engine::Get().GetRenderer().EndFrame();
    }

    
    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}
