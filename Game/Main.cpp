#include "Engine/Engine.h"

#include <map>
#include <fmod.hpp>
#include <memory>
#include <random>
#include <fstream>

#include "Renderer/Shader.h"
#include "Renderer/Pipeline.h"
#include "Renderer/VertexBuffer.h"

#include "Resources/ResourceManager.h"

using namespace nu;

namespace nu
{
    struct Color4
    {
        float r, g, b, a;
    };
}

struct Vertex
{
    Vector3 position;
    Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ Vector3{-0.7f, -1.0f, 0.0f}, Color4{1.0f, 0.0f, 0.0f, 1.0f}}, // Bottom-Left
    Vertex{ Vector3{0.3f, -0.1f, 0.0f}, Color4{0.0f, 2.0f, 0.0f, 1.0f}},// Bottom-Right
    Vertex{ Vector3{0.0f,  0.6f, 0.0f}, Color4{0.0f, 0.0f, 1.0f, 1.0f}}// Top-Middle
};


int main()
{
    //NO TOUCHY
    SetWorkingDirectory("assets"); 
    //NO TOUCHY
    
    // INITIALIZATION
    Engine::Get().Initialize();


    
    //TESTING
    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer().GetGPUDevice());

    auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();
    pipeline->AddVertexBuffer(sizeof(Vertex));
    //position
    pipeline->AddVertexAttribute(
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, position));
    //color
    pipeline->AddVertexAttribute(
        1,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
        offsetof(Vertex, color));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());
    //TESTING

    float scale = 0.5f;
    Color4 color{ 0, 0, 0, 1 };
    

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
        Engine::Get().Update();

        float dt = Engine::Get().GetTime().GetDeltaTime();

        scale = sin(Engine::Get().GetTime().GetTime());
        color.r = sin(Engine::Get().GetTime().GetTime()) * 0.5f + 0.5f;
  
        //RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexBuffer(*vb);
        Engine::Get().GetRenderer().SetVertexUniform(0, &scale, sizeof(scale));
        Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));

        Engine::Get().GetRenderer().Draw(vb->GetVertexCount());

        Engine::Get().GetRenderer().EndFrame(); // Render the screen
    }

    
    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}
