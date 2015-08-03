#include <Engine/Renderer/RenderQueue/RenderQueue.hpp>

namespace Ra {
namespace Engine {

template <typename Key, typename Child>
inline RenderableMap<Key, Child>::RenderableMap()
	: std::map<Key, Child>()
{
}

template <typename Key, typename Child>
inline RenderableMap<Key, Child>::~RenderableMap()
{
}

template <typename Key, typename Child>
inline void RenderableMap<Key, Child>::render(ShaderProgram* shader) const
{
	for (auto& it : *this)
	{
		it.first.bind(shader);
		it.second.render(shader);
	}
}

inline MeshVector::MeshVector()
	: std::vector<Mesh*>()
{
}

inline MeshVector::~MeshVector()
{
}

inline void MeshVector::render(ShaderProgram *shader) const
{
	CORE_UNUSED(shader);
	for (auto& it : *this)
	{
		it->render();
	}
}

inline RenderQueue::RenderQueue()
	: std::map<ShaderKey, MaterialRenderQueue>()
{
}

inline RenderQueue::~RenderQueue()
{
}

inline void RenderQueue::render() const
{
	for (auto& it : *this)
	{
		it.first.bind();
		it.second.render(it.first.getShader());
	}
}

inline void RenderQueue::render(const RenderParameters& params) const
{
	for (auto& it : *this)
	{
		it.first.bind(params);
		it.second.render(it.first.getShader());
	}
}

inline void RenderQueue::render(ShaderProgram* shader) const
{
	ShaderKey key(shader);
	key.bind();

	for (auto& it : *this)
	{
		it.second.render(key.getShader());
	}
}

inline void RenderQueue::render(ShaderProgram* shader, const RenderParameters& params) const
{
	ShaderKey key(shader);
	key.bind(params);

	for (auto& it : *this)
	{
		it.second.render(key.getShader());
	}
}

} // namespace Engine
} // namespace Ra
