#ifndef RADIUMENGINE_POINTLIGHT_HPP
#define RADIUMENGINE_POINTLIGHT_HPP

#include <Engine/RaEngine.hpp>
#include <Engine/Renderer/Light/Light.hpp>

namespace Ra {
namespace Engine {

class RA_ENGINE_API PointLight final : public Light {
  public:
    struct Attenuation {
        Scalar constant;
        Scalar linear;
        Scalar quadratic;

        Attenuation() : constant( 1.0 ), linear(), quadratic() {}
    };

  public:
    RA_CORE_ALIGNED_NEW

    PointLight( Entity* entity, const std::string& name = "pointlight" );
    ~PointLight();

    void getRenderParameters( RenderParameters& params ) const override;

    void setPosition( const Core::Math::Vector3& pos ) override;
    inline const Core::Math::Vector3& getPosition() const;

    inline void setAttenuation( const Attenuation& attenuation );
    inline void setAttenuation( Scalar constant, Scalar linear, Scalar quadratic );
    inline const Attenuation& getAttenuation() const;

    /*!
       \brief Redefinition from Component to manipulate lights with Gizmos
       \param Core::Container::Index roIdx Useless here
       \param const Core::Math::Transform& transform the transformation
       \return void
    */
    void setTransform( Core::Container::Index roIdx, const Core::Math::Transform& transform ) override;

    /*!
       \brief Redefinition from Component to update Gizmos position when you use them on light
       \return void
    */
    Core::Math::Transform getTransform( Core::Container::Index roIdx ) const override ;

    std::string getShaderInclude() const;

  private:
    Core::Math::Vector3 m_position;

    Attenuation m_attenuation;
};

} // namespace Engine
} // namespace Ra

#include <Engine/Renderer/Light/PointLight.inl>

#endif // RADIUMENGINE_POINTLIGHT_HPP
