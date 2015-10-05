#ifndef RADIUMENGINE_DRAW_PRIMITIVES_HPP_
#define RADIUMENGINE_DRAW_PRIMITIVES_HPP_
#include <Engine/RaEngine.hpp>
#include <Core/Math/LinearAlgebra.hpp>
#include <Core/Math/Ray.hpp>

namespace Ra
{
    namespace  Engine
    {
        class RenderObject;
        class Component;

        /// A set of convenient functions to instantiate simple displays such as points, lines, etc.
        /// note that objects will be drawn in their entity's local frame.
        /// For "instant" debug drawing, @see DebugDisplay.
        namespace DrawPrimitives
        {
            /// Displays given point shown as the crossing of 3 lines of length 'scale'
            RA_ENGINE_API RenderObject* Point( const Component* comp,
                                               const Core::Vector3& point,
                                               const Core::Color& color,
                                               Scalar scale = 0.1f);

            /// Displays given vector shown as an arrow originating from 'start'
            RA_ENGINE_API RenderObject* Vector( const Component* comp,
                                                const Core::Vector3& start,
                                                const Core::Vector3& v,
                                                const Core::Color& color);

            /// Displays given ray as a straight line.
            RA_ENGINE_API RenderObject* Ray( const Component* comp,
                                             const Core::Ray& ray,
                                             const Core::Color& color);

            /// Displays given triangle ABC, either in wireframe (fill = false)
            /// or filled with the color(fill = true).
            RA_ENGINE_API RenderObject* Triangle( const Component* comp,
                                                  const Core::Vector3& a,
                                                  const Core::Vector3& b,
                                                  const Core::Vector3& c,
                                                  const Core::Color& color,
                                                  bool fill = false);

            /// Displays circle computed with given center and radius, in plane normal to given vector in wireframe
            RA_ENGINE_API RenderObject* Circle( const Component* comp,
                                                const Core::Vector3& center,
                                                const Core::Vector3& normal,
                                                Scalar radius,
                                                uint segments,
                                                const Core::Color& color);

            /// Displays disk (filled circle) computed with given center and radius, in plane normal to given vector  in wireframe
            RA_ENGINE_API RenderObject* Disk( const Component* comp,
                                              const Core::Vector3& center,
                                              const Core::Vector3& normal,
                                              Scalar radius,
                                              uint segments,
                                              const Core::Color& color);

            /// Displays a normal vector emanating from the given point as a vector arrow
            /// and a normal plane of size 'scale'.
            RA_ENGINE_API RenderObject* Normal( const Component* comp,
                                                const Core::Vector3& point,
                                                const Core::Vector3& normal,
                                                const Core::Color& color,
                                                Scalar scale = 0.1f);

            /// Displays a 3D frame representing the given transform.
            /// Each axis has length 'scale' and are in usual colors (X,Y,Z = red, green blue)
            /// Remainder : the transform will be drawn relative to the component's entity transform.
            RA_ENGINE_API RenderObject* Frame( const Component* comp,
                                               const Core::Transform& frameFromEntity,
                                               Scalar scale = 0.1f);

            /// Create a res*res square grid centered on center, in plane normal to normal.
            RA_ENGINE_API RenderObject* Grid( const Component* comp,
                                              const Core::Vector3& center,
                                              const Core::Vector3& x,
                                              const Core::Vector3& y,
                                              const Core::Color& color,
                                              Scalar cellSize = 1.f,
                                              uint res = 10);
        }

    }
}

#endif // RADIUMENGINE_DRAW_PRIMITIVES_HPP_
