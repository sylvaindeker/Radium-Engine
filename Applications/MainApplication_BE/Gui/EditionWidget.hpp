#ifndef EDITIONWIDEGET_HPP_
#define EDITIONWIDEGET_HPP_

#include <QWidget>
#include <PluginBase/RadiumPluginInterface.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <ui_EditionWidget.h>

namespace Ra {
namespace Gui {

class EditionWidget : public QWidget, private Ui::EditionWidget{
    Q_OBJECT

public:
    EditionWidget(QWidget* parent, Ra::GuiBase::SelectionManager* selectionManager);

    ///set the transform on the selected object, return false in case of failure
    bool setTransform(const Core::Transform &tf, bool doUpdateInfos);
    bool setMatrix(Ra::Core::Matrix4 &m);

    ///get the transform of the selected object, return false in case of failure
    bool getTransform(Ra::Core::Transform *tf);

public slots:
    void resetSelectedObject();
    void applyMatrix();
    void matriceSize3();
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void useTransformMatrix();
    void updateInfos(bool doProperties);
    void onValueChanged(double);

private:
    bool applyWolfram();
    bool applyDirect();
    bool transformation();

    Ra::Gui::PickingManager* m_pickingManager;
    Ra::GuiBase::SelectionManager* m_selectionManager;
    QDoubleSpinBox* m_TabButtonDirect[16];
    QCheckBox* m_matrice3;

};
}
}

#endif //EDITIONWIDEGET_HPP_
