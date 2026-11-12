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

  
    auto& renderer = Engine::Get().GetRenderer();

    // MAIN LOOP
    bool quit = false;
    while (!quit) {
        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Engine::Get().GetEditor().ProcessEvent(event);
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

        // EDITOR
        Engine::Get().GetEditor().BeginFrame();
        Engine::Get().GetEditor().Update();

        //RENDER
        renderer.BeginFrame();
        scene.Draw(renderer);

        Engine::Get().GetRenderer().EndRenderPass();
        Engine::Get().GetEditor().Draw(renderer);

        renderer.EndFrame();
    }

    
    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}
