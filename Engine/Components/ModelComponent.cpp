#include "pch.h"
#include "ModelComponent.h"
#include "Core/Factory.h"

#include "Renderer/Uniforms.h"

#include "Framework/Actor.h"

#include "Engine/Engine.h"

namespace nu
{
	FACTORY_REGISTER(ModelComponent)

	void ModelComponent::Draw(const Renderer& renderer)
	{
		if (!m_model)
		{
			std::cerr << "Could not draw model: " << m_owner->GetName() << "\n";
			return;
		}

		ObjectUniform objectUniform
		{
			.model = m_owner->GetTransform().GetMatrix(),
			.normalMatrix = m_owner->GetTransform().GetMatrix()
		};

		renderer.SetVertexUniform(UniformSlot::Object, &objectUniform, sizeof(objectUniform));
		renderer.SetFragmentUniform(UniformSlot::Object, &objectUniform, sizeof(objectUniform));

		m_model->Draw(renderer, m_materials);
		
		// To see if the model is positioned correctly
		//std::cout << m_owner->GetTransform().position.x << " " << m_owner->GetTransform().position.y << " " << m_owner->GetTransform().position.z << std::endl;
	}

	void ModelComponent::Read(const json::value_t& value)
	{
		RendererComponent::Read(value);

		std::string modelName;
		JSON_READ_NAME(value, "model", modelName);

		m_model = Resources().Get<Model>(modelName, Engine::Get().GetRenderer());

		// "materials": [ "materials/brick.instance.json", null ] - one entry per slot, null keeps the model's material
		if (JSON_HAS_NAME(value, "materials"))
		{
			for (const auto& entry : JSON_GET_NAME(value, "materials").GetArray())
			{
				if (entry.IsString())
				{
					auto material = Resources().Get<MaterialInstance>(entry.GetString(), Engine::Get().GetRenderer());
					if (!material) std::cerr << "Could not load material: " << entry.GetString() << std::endl;
					m_materials.push_back(material);
				}
				else
				{
					m_materials.push_back(nullptr);
				}
			}
		}

	}
}