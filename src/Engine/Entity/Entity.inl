namespace Ra
{

inline void Engine::Entity::setTransform(const Core::Transform& transform)
{
    std::lock_guard<std::mutex> lock(m_transformMutex);
    m_transform = transform;
}

inline void Engine::Entity::setTransform(const Core::Matrix4& transform)
{
    std::lock_guard<std::mutex> lock(m_transformMutex);
    m_transform = Core::Transform(transform);
}

inline Core::Transform Engine::Entity::getTransform() const
{
    std::lock_guard<std::mutex> lock(m_transformMutex);
    return m_transform;
}

inline Core::Matrix4 Engine::Entity::getTransformAsMatrix() const
{
    std::lock_guard<std::mutex> lock(m_transformMutex);
    return m_transform.matrix();
}

} // namespace Ra
