#include <GuiBase/TransformEditor/TransformEditor.hpp>

#include <Engine/Component/Component.hpp>
#include <Engine/Entity/Entity.hpp>

namespace Ra {
namespace GuiBase {
void TransformEditor::setEditable( const Engine::ItemEntry& ent ) {
    m_currentEdit = ent;
    getTransform();
}

void TransformEditor::getTransform() {
    if ( canEdit() )
    {
        if ( m_currentEdit.isEntityNode() )
        {
            m_transform = m_currentEdit.m_entity->getTransform();
        } else if ( m_currentEdit.isComponentNode() || m_currentEdit.isRoNode() )
        { m_transform = m_currentEdit.m_component->getTransform( m_currentEdit.m_roIndex ); }
    }
}

bool TransformEditor::canEdit() const {
    return m_currentEdit.isValid() && m_currentEdit.isSelectable() &&
           ( m_currentEdit.isEntityNode() ||
             m_currentEdit.m_component->canEdit( m_currentEdit.m_roIndex ) );
}

void TransformEditor::setTransform( const Ra::Core::Math::Transform& tr ) {
    if ( canEdit() )
    {
        if ( m_currentEdit.isEntityNode() )
        {
            m_currentEdit.m_entity->setTransform( tr );
        } else
        { m_currentEdit.m_component->setTransform( m_currentEdit.m_roIndex, tr ); }
    }
}

Core::Math::Transform TransformEditor::getWorldTransform() const {
    return m_currentEdit.isEntityNode() ? Ra::Core::Math::Transform::Identity()
                                        : m_currentEdit.m_entity->getTransform();
}

TransformEditor::~TransformEditor() {
    setEditable( Engine::ItemEntry() );
}
} // namespace GuiBase
} // namespace Ra
