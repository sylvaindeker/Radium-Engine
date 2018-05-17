#include <Gui/LightEditor.hpp>
#include <Gui/MainWindow.hpp>
#include <Engine/ItemModel/ItemEntry.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Renderer/Light/DirLight.hpp>
#include <Engine/Renderer/Light/SpotLight.hpp>

#include <QPushButton>
#include <QColorDialog>
#include <QPalette>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QMessageBox>
#include <QCheckBox>

#include <math.h>
#define NB_DECIMAL 10
#define MAX_INTENSITY 100
#define MAX_ANGLE 360
#define MIN_CONSTANT -1000
#define MAX_CONSTANT 1000
#define MIN_LINEAR -100
#define MAX_LINEAR 100
#define MIN_QUADRA -100
#define MAX_QUADRA 100
#define MAX_COORD 10000000



namespace Ra {
namespace Gui {
LightEditor::LightEditor( QWidget* parent ) : QWidget( nullptr )    {
    setupUi( this );
    m_result_color->setAutoFillBackground(true);

    m_inner_angle_spinbox-> setDecimals (NB_DECIMAL);
    m_inner_angle_spinbox->setMaximum(MAX_ANGLE);
    m_inner_angle_slider->setMaximum(MAX_ANGLE);

    m_outer_angle_spinbox-> setDecimals (NB_DECIMAL);
    m_outer_angle_spinbox->setMaximum(MAX_ANGLE);
    m_outer_angle_slider->setMaximum(MAX_ANGLE);

    m_falloff_spinbox_constant->setDecimals (NB_DECIMAL);
    m_falloff_spinbox_constant->setMaximum(MAX_CONSTANT);
    m_falloff_spinbox_constant->setMinimum(MIN_CONSTANT);
    m_falloff_slider_constant->setMaximum(MAX_CONSTANT);
    m_falloff_slider_constant->setMinimum(MIN_CONSTANT);



    m_falloff_spinbox_linear->setDecimals (NB_DECIMAL);
    m_falloff_spinbox_linear->setMaximum(MAX_LINEAR);
    m_falloff_spinbox_linear->setMinimum(MIN_LINEAR);
    m_falloff_slider_linear->setMaximum(MAX_LINEAR);
    m_falloff_slider_linear->setMinimum(MIN_LINEAR);


    m_falloff_spinbox_quadratic->setDecimals (NB_DECIMAL);
    m_falloff_spinbox_quadratic->setMaximum(MAX_QUADRA);
    m_falloff_spinbox_quadratic->setMinimum(MIN_QUADRA);
    m_falloff_slider_quadratic->setMaximum(MAX_QUADRA);
    m_falloff_slider_quadratic->setMinimum(MIN_QUADRA);





    m_pos_x_spin->setDecimals (NB_DECIMAL);
    m_pos_x_spin->setMaximum(MAX_COORD);
    m_pos_x_spin->setMinimum(-MAX_COORD);

    m_pos_y_spin->setDecimals (NB_DECIMAL);
    m_pos_y_spin->setMaximum(MAX_COORD);
    m_pos_y_spin->setMinimum(-MAX_COORD);

    m_pos_z_spin->setDecimals (NB_DECIMAL);
    m_pos_z_spin->setMaximum(MAX_COORD);
    m_pos_z_spin->setMinimum(-MAX_COORD);

    m_dir_x_spin->setDecimals (NB_DECIMAL);
    m_dir_x_spin->setMaximum(MAX_COORD);
    m_dir_x_spin->setMinimum(-MAX_COORD);


    m_dir_y_spin->setDecimals (NB_DECIMAL);
    m_dir_y_spin->setMaximum(MAX_COORD);
    m_dir_y_spin->setMinimum(-MAX_COORD);


    m_dir_z_spin->setDecimals (NB_DECIMAL);
    m_dir_z_spin->setMaximum(MAX_COORD);
    m_dir_z_spin->setMinimum(-MAX_COORD);



    setWindowTitle("Light Editor");
    connect(m_button_color, &QPushButton::clicked,this,&LightEditor::open_dialogColor);


    // Angle
    connect(m_inner_angle_slider, &QSlider::valueChanged, this , &LightEditor::slot_inner_angle_slide_to_spin );
    connect(this,&LightEditor::sig_inner_angle_slide_to_spin,m_inner_angle_spinbox,&QDoubleSpinBox::setValue);

    connect(m_inner_angle_spinbox, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged), this , &LightEditor::slot_inner_angle_spin_to_slide );
    connect(this,&LightEditor::sig_inner_angle_spin_to_slide,m_inner_angle_slider,&QSlider::setValue);

    connect(m_outer_angle_slider, &QSlider::valueChanged, this , &LightEditor::slot_outer_angle_slide_to_spin );
    connect(this,&LightEditor::sig_outer_angle_slide_to_spin,m_outer_angle_spinbox,&QDoubleSpinBox::setValue);

    connect(m_outer_angle_spinbox, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged), this , &LightEditor::slot_outer_angle_spin_to_slide );
    connect(this,&LightEditor::sig_outer_angle_spin_to_slide,m_outer_angle_slider,&QSlider::setValue);

    // Falloff

    connect(m_falloff_slider_constant, &QSlider::valueChanged, this , &LightEditor::slot_falloff_constant_slide_to_spin );
    connect(this,&LightEditor::sig_falloff_constant_slide_to_spin,m_falloff_spinbox_constant,&QDoubleSpinBox::setValue);

    connect(m_falloff_spinbox_constant, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged), this , &LightEditor::slot_falloff_constant_spin_to_slide );
    connect(this,&LightEditor::sig_falloff_constant_spin_to_slide,m_falloff_slider_constant,&QSlider::setValue);

    connect(m_falloff_slider_linear, &QSlider::valueChanged, this , &LightEditor::slot_falloff_linear_slide_to_spin );
    connect(this,&LightEditor::sig_falloff_linear_slide_to_spin,m_falloff_spinbox_linear,&QDoubleSpinBox::setValue);

    connect(m_falloff_spinbox_linear, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged), this , &LightEditor::slot_falloff_linear_spin_to_slide );
    connect(this,&LightEditor::sig_falloff_linear_spin_to_slide,m_falloff_slider_linear,&QSlider::setValue);

    connect(m_falloff_slider_quadratic, &QSlider::valueChanged, this , &LightEditor::slot_falloff_quadratic_slide_to_spin );
    connect(this,&LightEditor::sig_falloff_quadratic_slide_to_spin,m_falloff_spinbox_quadratic,&QDoubleSpinBox::setValue);

    connect(m_falloff_spinbox_quadratic, static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged), this , &LightEditor::slot_falloff_quadratic_spin_to_slide );
    connect(this,&LightEditor::sig_falloff_quadratic_spin_to_slide,m_falloff_slider_quadratic,&QSlider::setValue);

// OK Button
    connect(m_button_create,&QPushButton::clicked,this,&LightEditor::edit_light);
    connect(this,&LightEditor::sig_close_window,this,&QWidget::close);

}

LightEditor::~LightEditor(){
}

void LightEditor::open_dialogColor(){
  m_color = QColorDialog::getColor ();
  m_pal.setColor(QPalette::Background,m_color);
  m_result_color->setPalette(m_pal);
}

/// Angle

void LightEditor::slot_inner_angle_slide_to_spin(int val){
  double tmp =  (double) val;
  emit sig_inner_angle_slide_to_spin(tmp);
}

void LightEditor::slot_inner_angle_spin_to_slide(double val){
  int tmp = (int) val;
  emit sig_inner_angle_spin_to_slide(tmp);
}

void LightEditor::slot_outer_angle_slide_to_spin(int val){
  double tmp =  (double) val;
  emit sig_outer_angle_slide_to_spin(tmp);
}

void LightEditor::slot_outer_angle_spin_to_slide(double val){
  int tmp = (int) val;
  emit sig_outer_angle_spin_to_slide(tmp);
}


/// Falloff
void LightEditor::slot_falloff_constant_slide_to_spin(int val){
  double tmp =  (double) val;
  emit sig_falloff_constant_slide_to_spin(tmp);
}

void LightEditor::slot_falloff_constant_spin_to_slide(double val){
  int tmp = (int) val;
  emit sig_falloff_constant_spin_to_slide(tmp);
}

void LightEditor::slot_falloff_linear_slide_to_spin(int val){
  double tmp =  (double) val;
  emit sig_falloff_linear_slide_to_spin(tmp);
}

void LightEditor::slot_falloff_linear_spin_to_slide(double val){
  int tmp = (int) val;
  emit sig_falloff_linear_spin_to_slide(tmp);
}
void LightEditor::slot_falloff_quadratic_slide_to_spin(int val){
  double tmp =  (double) val;
  emit sig_falloff_quadratic_slide_to_spin(tmp);
}

void LightEditor::slot_falloff_quadratic_spin_to_slide(double val){
  int tmp = (int) val;
  emit sig_falloff_quadratic_spin_to_slide(tmp);
}

void LightEditor::init(Ra::Engine::ItemEntry item){
    m_light = (Ra::Engine::Light *) item.m_component;
    m_lineEdit->setText(QString::fromStdString(m_light->getName()));
    m_lineEdit->setDisabled(true);
    m_type = m_light->getType();
    m_kind_of_light->setDisabled(true);
    m_kind_of_light->setCurrentIndex(m_type);
    
    m_angle_lab->setVisible(false);
    m_angle_lab->setVisible(false);
    m_inner_angle_spinbox->setVisible(false);
    m_inner_angle_slider->setVisible(false);
    m_inner_angle_lab->setVisible(false);
    m_outer_angle_spinbox->setVisible(false);
    m_outer_angle_slider->setVisible(false);
    m_outer_angle_lab->setVisible(false);

    m_pos_x_lab->setVisible(true);
    m_pos_y_lab->setVisible(true);
    m_pos_z_lab->setVisible(true);
    m_pos_x_spin->setVisible(true);
    m_pos_y_spin->setVisible(true);
    m_pos_z_spin->setVisible(true);
    m_coord_lab->setVisible(true);

    m_falloff_tip->setVisible(true);
    m_falloff_lab->setVisible(true);
    m_falloff_spinbox_linear->setVisible(true);
    m_falloff_spinbox_constant->setVisible(true);
    m_falloff_spinbox_quadratic->setVisible(true);
    m_falloff_slider_linear->setVisible(true);
    m_falloff_slider_constant->setVisible(true);
    m_falloff_slider_quadratic->setVisible(true);
    m_falloff_lab_linear->setVisible(true);
    m_falloff_lab_constant->setVisible(true);
    m_falloff_lab_quadratic->setVisible(true);

    m_dir_tip->setVisible(true);
    m_dir_x_spin->setVisible(true);
    m_dir_y_spin->setVisible(true);
    m_dir_z_spin->setVisible(true);
    m_dir_x_lab->setVisible(true);
    m_dir_y_lab->setVisible(true);
    m_dir_z_lab->setVisible(true);
    m_direction_lab->setVisible(true);
   
    // Color 
    Core::Color col =  m_light->getColor();
    double dr, dg, db;
    dr = col.x();
    dg = col.y();
    db = col.z();
    m_color = QColor((int) (dr*255), (int) (dg*255), (int) (db*255));
    m_pal.setColor(QPalette::Background,m_color);
    m_result_color->setPalette(m_pal);
    
    switch (m_type) {
        case 0 : // Directional
            m_direction = ((Ra::Engine::DirectionalLight *) m_light)->getDirection();
            m_dir_x_spin->setValue((double) m_direction.x());
            m_dir_y_spin->setValue((double) m_direction.y());
            m_dir_z_spin->setValue((double) m_direction.z());
            /* m_intensity = m_light->getColor().w();
            m_intensity_spinbox->setValue(m_intensity);*/

            m_pos_x_lab->setVisible(false);
            m_pos_y_lab->setVisible(false);
            m_pos_z_lab->setVisible(false);
            m_pos_x_spin->setVisible(false);
            m_pos_y_spin->setVisible(false);
            m_pos_z_spin->setVisible(false);
            m_coord_lab->setVisible(false);
            
            m_falloff_tip->setVisible(false);
            m_falloff_lab->setVisible(false);
            m_falloff_spinbox_linear->setVisible(false);
            m_falloff_spinbox_constant->setVisible(false);
            m_falloff_spinbox_quadratic->setVisible(false);
            m_falloff_slider_linear->setVisible(false);
            m_falloff_slider_constant->setVisible(false);
            m_falloff_slider_quadratic->setVisible(false);
            m_falloff_lab_linear->setVisible(false);
            m_falloff_lab_constant->setVisible(false);
            m_falloff_lab_quadratic->setVisible(false);
            break;
        case 1 : // Point
            m_falloff_p = ((Ra::Engine::PointLight *) m_light)->getAttenuation();
            m_falloff_spinbox_linear->setValue((double) m_falloff_p.linear);
            m_falloff_spinbox_constant->setValue((double) m_falloff_p.constant);
            m_falloff_spinbox_quadratic->setValue((double) m_falloff_p.quadratic);

            m_position = ((Ra::Engine::PointLight *) m_light)->getPosition();
            m_pos_x_spin->setValue(m_position.x());
            m_pos_y_spin->setValue(m_position.y());
            m_pos_z_spin->setValue(m_position.z());

            m_dir_tip->setVisible(false);
            m_dir_x_spin->setVisible(false);
            m_dir_y_spin->setVisible(false);
            m_dir_z_spin->setVisible(false);
            m_dir_x_lab->setVisible(false);
            m_dir_y_lab->setVisible(false);
            m_dir_z_lab->setVisible(false);
            m_direction_lab->setVisible(false);
            break;
        case 2 : // Spot
            m_direction = ((Ra::Engine::SpotLight *) m_light)->getDirection();
            m_dir_x_spin->setValue((double) m_direction.x());
            m_dir_y_spin->setValue((double) m_direction.y());
            m_dir_z_spin->setValue((double) m_direction.z());
            
            m_position = ((Ra::Engine::SpotLight *) m_light)->getPosition();
            m_pos_x_spin->setValue(m_position.x());
            m_pos_y_spin->setValue(m_position.y());
            m_pos_z_spin->setValue(m_position.z());

            m_falloff_s = ((Ra::Engine::SpotLight *) m_light)->getAttenuation();
            m_falloff_spinbox_linear->setValue((double) m_falloff_s.linear);
            m_falloff_spinbox_constant->setValue((double) m_falloff_s.constant);
            m_falloff_spinbox_quadratic->setValue((double) m_falloff_s.quadratic);
            
            m_inner_angle = ((Ra::Engine::SpotLight *) m_light)->getInnerAngle();
            m_outer_angle = ((Ra::Engine::SpotLight *) m_light)->getOuterAngle();
            m_inner_angle_spinbox->setValue(Core::Math::toDegrees(m_inner_angle));
            m_outer_angle_spinbox->setValue(Core::Math::toDegrees(m_outer_angle));

            m_angle_lab->setVisible(true);
            m_angle_lab->setVisible(true);
            m_inner_angle_spinbox->setVisible(true);
            m_inner_angle_slider->setVisible(true);
            m_inner_angle_lab->setVisible(true);
            m_outer_angle_spinbox->setVisible(true);
            m_outer_angle_slider->setVisible(true);
            m_outer_angle_lab->setVisible(true);
            break;
        default :
            return;
    }

    show();
}

void LightEditor::edit_light(){
    int ir, ig, ib;
    m_color.getRgb(&ir, &ig, &ib);
    m_light->setColor(Core::Color(((double) ir)/255, ((double) ig)/255, ((double) ib)/255, 0));

    switch (m_type){
        case 0 : // Directional Light
            m_direction = Core::Vector3(m_dir_x_spin->value(), m_dir_y_spin->value(), m_dir_z_spin->value());
            ((Ra::Engine::DirectionalLight *) m_light)->setDirection(m_direction);

            break;
        case 1 : // Point Light
            ((Ra::Engine::PointLight *) m_light)->setAttenuation(m_falloff_spinbox_constant->value(), m_falloff_spinbox_linear->value(), m_falloff_spinbox_quadratic->value());

            m_position = Core::Vector3(m_pos_x_spin->value(), m_pos_y_spin->value(), m_pos_z_spin->value());
            ((Ra::Engine::PointLight *) m_light)->setPosition(m_position);

            break;
        case 2 : // Spot Light
            ((Ra::Engine::SpotLight *) m_light)->setAttenuation(m_falloff_spinbox_constant->value(), m_falloff_spinbox_linear->value(), m_falloff_spinbox_quadratic->value());

            m_direction = Core::Vector3(m_dir_x_spin->value(), m_dir_y_spin->value(), m_dir_z_spin->value());
            ((Ra::Engine::SpotLight *) m_light)->setDirection(m_direction);

            m_position = Core::Vector3(m_pos_x_spin->value(), m_pos_y_spin->value(), m_pos_z_spin->value());
            ((Ra::Engine::SpotLight *) m_light)->setPosition(m_position);

            ((Ra::Engine::SpotLight *) m_light)->setInnerAngleInDegrees(m_inner_angle_spinbox->value());
            ((Ra::Engine::SpotLight *) m_light)->setOuterAngleInDegrees(m_outer_angle_spinbox->value());

            break;
        default :
            break;
    }
    emit sig_close_window();
}

} // namespace Gui
} // namespace Ra
