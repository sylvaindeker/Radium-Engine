#include <Core/Geometry/ScalarField.hpp>

#include <Core/Geometry/TriangleOperation.hpp>

namespace Ra {
namespace Core {
namespace Geometry {

Gradient gradientOfFieldS( const Container::VectorArray<Math::Vector3>& p, const Container::VectorArray<Triangle>& T,
                           const ScalarField& S ) {
    Gradient g;
    for ( const auto& t : T )
    {
        uint v0 = t( 0 );
        uint v1 = t( 1 );
        uint v2 = t( 2 );

        Scalar u0 = S( v0 );
        Scalar u1 = S( v1 );
        Scalar u2 = S( v2 );

        Math::Vector3 e01 = p[v1] - p[v0];
        Math::Vector3 e12 = p[v2] - p[v1];
        Math::Vector3 e20 = p[v0] - p[v2];

        Math::Vector3 N = triangleNormal( p[v0], p[v1], p[v2] );
        Scalar area = triangleArea( p[v0], p[v1], p[v2] );

        g.push_back(
            -( ( ( u0 * N.cross( e12 ) ) + ( u1 * N.cross( e20 ) ) + ( u2 * N.cross( e01 ) ) ) /
               ( area * 2.0f ) )
                 .normalized() );
    }
    return g;
}

Divergence divergenceOfFieldX( const Container::VectorArray<Math::Vector3>& p, const Container::VectorArray<Triangle>& T,
                               const Gradient& X ) {
    Divergence div( p.size() );
    div.setZero();

    for ( uint i = 0; i < T.size(); ++i )
    {
        uint v0 = T[i]( 0 );
        uint v1 = T[i]( 1 );
        uint v2 = T[i]( 2 );

        Math::Vector3 e01 = p[v1] - p[v0];
        Math::Vector3 e12 = p[v2] - p[v1];
        Math::Vector3 e20 = p[v0] - p[v2];

        Scalar cotV0 = Math::Vector::cotan( e01, ( -e20 ).eval() );
        Scalar cotV1 = Math::Vector::cotan( e12, ( -e01 ).eval() );
        Scalar cotV2 = Math::Vector::cotan( e20, ( -e12 ).eval() );

        div( v0 ) += ( cotV2 * ( e01.dot( X[i] ) ) ) + ( cotV1 * ( ( -e20 ).dot( X[i] ) ) );
        div( v1 ) += ( cotV0 * ( e12.dot( X[i] ) ) ) + ( cotV2 * ( ( -e01 ).dot( X[i] ) ) );
        div( v2 ) += ( cotV1 * ( e20.dot( X[i] ) ) ) + ( cotV0 * ( ( -e12 ).dot( X[i] ) ) );
    }

    return ( div * 0.5 );
}

} // namespace Geometry
} // namespace Core
} // namespace Ra
