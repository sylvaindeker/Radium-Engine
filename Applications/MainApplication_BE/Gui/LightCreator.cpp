/*!
   \file LightCreator.cpp
   \brief The Class LightCreator is associated with its ./ui/LightCreator.ui and ./LightCreator.hpp to design GUI to add Lights in RadiumEngine
   \author Sylvain
   \date 16/05/2018
*/

#include <Gui/LightCreator.hpp>
#include <Gui/MainWindow.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Engine/Renderer/Light/PointLight.hpp>
#include <Engine/ItemModel/ItemEntry.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>
#include <Engine/Renderer/Light/DirLight.hpp>
#include <Engine/Renderer/Light/SpotLight.hpp>
#include <Engine/Renderer/Light/PointLight.hpp>
#include <Core/Asset/LightData.hpp>
#include <GuiBase/Viewer/Viewer.hpp>


#include <QPushButton>
#include <QColorDialog>
#include <QPalette>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QMessageBox>


#include <math.h>
#define NB_DECIMAL 3
#define MAX_ANGLE 360
#define MAX_CONSTANT 100
#define MAX_LINEAR 100
#define MAX_QUADRA 100
#define MAX_COORD 10000000
#define STEP_SLIDER 10



namespace Ra {
namespace Gui {
/*!
   \brief Constructor (call only once)
   \param QWidget* parent only used for the icon
   \param Ra::Gui::Viewer *viewer to add new Lights to the Viewer
*/
LightCreator::LightCreator( QWidget* parent,const Ra::Gui::Viewer *viewer ) :
    QWidget( nullptr ),
    m_viewer(viewer)     {
    setupUi( this );
    setWindowTitle("Light Creator");
    setWindowIcon(parent->windowIcon()); // same icon as the main App
    setFixedSize(size());
    m_position = Core::Math::Vector3(0,0,0);
    m_direction = Core::Math::Vector3(0,0,1);
    m_color = QColor(255,255,255); // Color is white by default
    m_name = QString("");
    m_inner_angle_val = 0;
    m_outer_angle_val = 0;
    m_falloff_val_constant = 1; // 1 = value by default
    m_falloff_val_linear = 0;
    m_falloff_val_quadratic = 0;
    m_lightType = 0;
    m_entity_selected = nullptr;
    // Qwidget that show the selected color
    // QColor is manipulate with QPalette
    QPalette p;
    p.setColor(QPalette::Background,m_color);
    m_result_color->setAutoFillBackground(true);
    m_result_color->setPalette(p);


    init();//private function only called once, here.


}

/*!
   \brief init function only called once in the Constructor
*/
void LightCreator::init(){
  connect(this, &LightCreator::sig_show,this,&LightCreator::show);
  // Color Selecter
  connect(m_button_color,
     &QPushButton::clicked,
     this,
     &LightCreator::open_dialogColor);

  // LightType Selecter (to hide unrelevent options )
  connect(m_kind_of_light,
    static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
    this,
    &LightCreator::slot_select_light);

  // Entity Selecter
  connect(m_entity_group_box,
    static_cast<void (QComboBox::*) (const QString&)>(&QComboBox::activated),
    this,
    &LightCreator::slot_selected_entity);

  // All connect about hiding Angles options :
  // 7 elements ( 3 QLabel, 2 QDoubleSpinBox and 2 QSlider) with
  // hide and show actions so 14 connect()
  connect(this,&LightCreator::sig_show_angle,m_angle_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_angle,
          m_inner_angle_slider,&QSlider::show);
  connect(this,&LightCreator::sig_show_angle,
          m_inner_angle_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_angle,
          m_inner_angle_spinbox,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_angle,
          m_outer_angle_slider,&QSlider::show);
  connect(this,&LightCreator::sig_show_angle,
          m_outer_angle_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_angle,
          m_outer_angle_spinbox,&QDoubleSpinBox::show);

  connect(this,&LightCreator::sig_hide_angle,m_angle_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_angle,
          m_outer_angle_slider,&QSlider::hide);
  connect(this,&LightCreator::sig_hide_angle,
          m_outer_angle_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_angle,
          m_inner_angle_slider,&QSlider::hide);
  connect(this,&LightCreator::sig_hide_angle,
          m_inner_angle_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_angle,
          m_inner_angle_spinbox,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_angle,
    m_outer_angle_spinbox,&QDoubleSpinBox::hide);

  // All connect about hiding Direction options :
  // 8 elements ( 5 QLabel and 3 QDoubleSpinBox)
  // with hide and show actions so 16 connect()
  connect(this,&LightCreator::sig_hide_dir,m_dir_x_spin,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_dir,m_dir_y_spin,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_dir,m_dir_z_spin,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_dir,m_dir_x_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_dir,m_dir_y_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_dir,m_dir_z_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_dir,m_dir_tip,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_dir,m_direction_lab,&QLabel::hide);

  connect(this,&LightCreator::sig_show_dir,m_dir_x_spin,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_dir,m_dir_y_spin,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_dir,m_dir_z_spin,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_dir,m_dir_x_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_dir,m_dir_y_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_dir,m_dir_z_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_dir,m_dir_tip,&QLabel::show);
  connect(this,&LightCreator::sig_show_dir,m_direction_lab,&QLabel::show);

  // All connect about hiding Falloff options :
  // 11 elements ( 5 QLabel, 3 QDoubleSpinBox and 3 QSlider) with hide and show
  //actions so 22 connect()
  connect(this,&LightCreator::sig_hide_falloff,m_falloff_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_lab_quadratic,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_lab_constant,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_lab_linear,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_tip,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_spinbox_linear,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_spinbox_constant,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_spinbox_quadratic,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_slider_linear,&QSlider::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_slider_constant,&QSlider::hide);
  connect(this,&LightCreator::sig_hide_falloff,
          m_falloff_slider_quadratic,&QSlider::hide);


  connect(this,&LightCreator::sig_show_falloff,m_falloff_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_lab_quadratic,&QLabel::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_lab_constant,&QLabel::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_lab_linear,&QLabel::show);
  connect(this,&LightCreator::sig_show_falloff,m_falloff_tip,&QLabel::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_spinbox_linear,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_spinbox_constant,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_spinbox_quadratic,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_slider_linear,&QSlider::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_slider_constant,&QSlider::show);
  connect(this,&LightCreator::sig_show_falloff,
          m_falloff_slider_quadratic,&QSlider::show);


  // All connect about hiding Position options :
  // 7 elements ( 4 QLabel and 3 QDoubleSpinBox)
  // with hide and show actions so 14 connect()
  connect(this,&LightCreator::sig_hide_pos,m_pos_x_spin,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_pos,m_pos_y_spin,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_pos,m_pos_z_spin,&QDoubleSpinBox::hide);
  connect(this,&LightCreator::sig_hide_pos,m_pos_x_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_pos,m_pos_y_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_pos,m_pos_z_lab,&QLabel::hide);
  connect(this,&LightCreator::sig_hide_pos,m_coord_lab,&QLabel::hide);

  connect(this,&LightCreator::sig_show_pos,m_pos_x_spin,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_pos,m_pos_y_spin,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_pos,m_pos_z_spin,&QDoubleSpinBox::show);
  connect(this,&LightCreator::sig_show_pos,m_pos_x_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_pos,m_pos_y_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_pos,m_pos_z_lab,&QLabel::show);
  connect(this,&LightCreator::sig_show_pos,m_coord_lab,&QLabel::show);

  // Follow connect() sync the QSlider and the QDoubleSpinBox

    // Angle
    connect(m_inner_angle_slider, &QSlider::valueChanged,
      this , &LightCreator::slot_inner_angle_slide_to_spin );
    connect(this,&LightCreator::sig_inner_angle_slide_to_spin,
      m_inner_angle_spinbox,&QDoubleSpinBox::setValue);

    connect(m_inner_angle_spinbox,
       static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
        this , &LightCreator::slot_inner_angle_spin_to_slide );
    connect(this,&LightCreator::sig_inner_angle_spin_to_slide,
      m_inner_angle_slider,&QSlider::setValue);

    connect(m_outer_angle_slider, &QSlider::valueChanged, this ,
       &LightCreator::slot_outer_angle_slide_to_spin );
    connect(this,&LightCreator::sig_outer_angle_slide_to_spin,
      m_outer_angle_spinbox,&QDoubleSpinBox::setValue);

    connect(m_outer_angle_spinbox,
      static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
      this ,
      &LightCreator::slot_outer_angle_spin_to_slide );
    connect(this,&LightCreator::sig_outer_angle_spin_to_slide,
      m_outer_angle_slider,&QSlider::setValue);


    // Falloff

    connect(m_falloff_slider_constant, &QSlider::valueChanged,
       this , &LightCreator::slot_falloff_constant_slide_to_spin );
    connect(this,&LightCreator::sig_falloff_constant_slide_to_spin,
      m_falloff_spinbox_constant,&QDoubleSpinBox::setValue);

    connect(m_falloff_spinbox_constant,
      static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
      this ,
      &LightCreator::slot_falloff_constant_spin_to_slide );
    connect(this,
      &LightCreator::sig_falloff_constant_spin_to_slide,
      m_falloff_slider_constant,
      &QSlider::setValue);

    connect(m_falloff_slider_linear,
       &QSlider::valueChanged,
       this ,
       &LightCreator::slot_falloff_linear_slide_to_spin );
    connect(this,
      &LightCreator::sig_falloff_linear_slide_to_spin,
      m_falloff_spinbox_linear,
      &QDoubleSpinBox::setValue);

    connect(m_falloff_spinbox_linear,
       static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
       this ,
       &LightCreator::slot_falloff_linear_spin_to_slide );
    connect(this,
      &LightCreator::sig_falloff_linear_spin_to_slide,
      m_falloff_slider_linear,
      &QSlider::setValue);

    connect(m_falloff_slider_quadratic,
       &QSlider::valueChanged,
       this ,
       &LightCreator::slot_falloff_quadratic_slide_to_spin );
    connect(this,
      &LightCreator::sig_falloff_quadratic_slide_to_spin,
      m_falloff_spinbox_quadratic,
      &QDoubleSpinBox::setValue);


    connect(m_falloff_spinbox_quadratic,
      static_cast<void (QDoubleSpinBox::*) (double)>(&QDoubleSpinBox::valueChanged),
      this ,
      &LightCreator::slot_falloff_quadratic_spin_to_slide );
    connect(this,&LightCreator::sig_falloff_quadratic_spin_to_slide,
      m_falloff_slider_quadratic,&QSlider::setValue);


  // OK Button
  connect(m_button_create,&QPushButton::clicked,
      this,&LightCreator::open_dialogueConfirm);// Confirm and save
  // Cancel Button
  connect(m_button_cancel,&QPushButton::clicked,
      this,&LightCreator::close_dialogueConfirm);
  connect(this,&LightCreator::sig_close_windows,
      this,&LightCreator::close_dialogueConfirm);


  connect(this,&LightCreator::sig_addLight,m_viewer,&Gui::Viewer::addLight);

  // Init with Directionnal selected by default
  m_angle_lab->setVisible(false);
  m_angle_lab->setVisible(false);
  m_inner_angle_spinbox->setVisible(false);
  m_inner_angle_slider->setVisible(false);
  m_inner_angle_lab->setVisible(false);
  m_outer_angle_spinbox->setVisible(false);
  m_outer_angle_slider->setVisible(false);
  m_outer_angle_lab->setVisible(false);


  m_pos_x_lab->setVisible(false);
  m_pos_y_lab->setVisible(false);
  m_pos_z_lab->setVisible(false);
  m_pos_x_spin->setVisible(false);
  m_pos_y_spin->setVisible(false);
  m_pos_z_spin->setVisible(false);
  m_coord_lab->setVisible(false);
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
  m_falloff_tip->setVisible(false);

  m_lineEdit->setPlaceholderText(m_name);

  m_dir_x_spin->setValue((double) m_direction.x());
  m_dir_y_spin->setValue((double) m_direction.y());
  m_dir_z_spin->setValue((double) m_direction.z());

  m_pos_x_spin->setValue((double) m_position.x());
  m_pos_y_spin->setValue((double) m_position.y());
  m_pos_z_spin->setValue((double) m_position.z());

  m_falloff_spinbox_constant->setValue(1);
  m_falloff_slider_constant->setValue(1*STEP_SLIDER);

  // Details information

  m_inner_angle_spinbox-> setDecimals (NB_DECIMAL);
  m_inner_angle_spinbox->setMaximum(MAX_ANGLE);
  m_inner_angle_slider->setMaximum(MAX_ANGLE*STEP_SLIDER);

  m_outer_angle_spinbox-> setDecimals (NB_DECIMAL);
  m_outer_angle_spinbox->setMaximum(MAX_ANGLE);
  m_outer_angle_slider->setMaximum(MAX_ANGLE*STEP_SLIDER);

  m_falloff_spinbox_constant->setDecimals (NB_DECIMAL);
  m_falloff_spinbox_constant->setMaximum(MAX_CONSTANT);
  m_falloff_slider_constant->setMaximum(MAX_CONSTANT*STEP_SLIDER);

  m_falloff_spinbox_linear->setDecimals (NB_DECIMAL);
  m_falloff_spinbox_linear->setMaximum(MAX_LINEAR);
  m_falloff_slider_linear->setMaximum(MAX_LINEAR*STEP_SLIDER);


  m_falloff_spinbox_quadratic->setDecimals (NB_DECIMAL);
  m_falloff_spinbox_quadratic->setMaximum(MAX_QUADRA);
  m_falloff_slider_quadratic->setMaximum(MAX_QUADRA*STEP_SLIDER);


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

}

/*!
     \brief Destructor
*/
LightCreator::~LightCreator(){
}

/*!
     \brief Select a color
*/
void LightCreator::open_dialogColor(){
    QPalette p;
    QColor tmp_color = QColorDialog::getColor();
    if(tmp_color == QColor(0,0,0)){
            QMessageBox::warning(this,"Watch out !",
              "Selected color: invalid\nNo effect on renderer.");
    }
    if(tmp_color.isValid()){
            m_color = tmp_color;
    }
    p.setColor(QPalette::Background,m_color);
    m_result_color->setPalette(p);

}

/*!
     \brief Slot that check data and save
*/
void LightCreator::open_dialogueConfirm(){
    m_name = m_lineEdit->text();
    if( m_name.isEmpty()){
        QMessageBox::critical(this, "Watch out !",
                "A new light must have a name !");
    }
    else if (m_entity_selected == nullptr){
            QMessageBox::critical(this, "Watch out !",
                "A light must be linked to an entity !");
    }
    else if( m_entity_selected->getComponent( m_name.toStdString()) != nullptr) {
        QMessageBox::critical(this, "Watch out !","The name is already used !");
    }
    else if ( m_lightType != _POINT_LIGHT
            && m_dir_x_spin->value()== 0
             && m_dir_y_spin->value()==0
              && m_dir_z_spin->value() == 0 ){
        // Only _DIR_LIGHT and _SPOT_LIGHT required sush check
        QMessageBox::critical(this, "Watch out !",
          "Direction Vector cannot be null on each conponent (x,y,z) ! ");
    }
    else {
        save_light(); // private function
        emit sig_close_windows() ;

        // Reset data set
        m_lineEdit->setText(QString::fromStdString(""));

        m_kind_of_light->setCurrentIndex(0);

        m_color.setRgb(255,255,255);
        QPalette p;
        p.setColor(QPalette::Background,m_color);
        m_result_color->setPalette(p);

        m_entity_selected = nullptr;

        m_dir_x_spin->setValue(0.0);
        m_dir_y_spin->setValue(0.0);
        m_dir_z_spin->setValue(1.0);

        m_pos_x_spin->setValue(0.0);
        m_pos_y_spin->setValue(0.0);
        m_pos_z_spin->setValue(0.0);

        emit sig_falloff_constant_slide_to_spin(1.0);
        emit sig_falloff_linear_slide_to_spin(0.0);
        emit sig_falloff_quadratic_slide_to_spin(0.0);
        emit sig_inner_angle_slide_to_spin(0.0);
        emit sig_outer_angle_slide_to_spin(0.0);
        m_inner_angle_val = 0;
        m_outer_angle_val = 0;

    }
}

void LightCreator::close_dialogueConfirm(){
        QWidget::close();
}

/*!
     \brief Private function that save config light parameters
     \param Ra::Engine::Entity *entity because Lights are components and need to be attached to an entity
     \return void
*/
void LightCreator::save_light(){
    CORE_ASSERT(m_entity_selected != nullptr, "No entity selected");
    // The function m_color.getRgb(...) give value in RGB range 0 to
    // 255 (int) and Core::Math::Color() need in range 0.0 to 1.0 (double)
    double dr,dg,db;
    int ir,ig,ib;
    m_color.getRgb(&ir,&ig,&ib);
    dr=(double)ir;
    dg=(double)ig;
    db=(double)ib;
    dr/=255;
    dg/=255;
    db/=255;

    // Every kind of Light need Color specification
    Core::Math::Color c = Core::Math::Color( dr, dg, db, 0 );

    switch (m_lightType) {
        /*
        in the file ./LightCreator.hpp :
        - #define _DIR_LIGHT 0
        - #define _POINT_LIGHT 1
        - #define _SPOT_LIGHT 2
        */
        case _DIR_LIGHT:
        Ra::Engine::DirectionalLight * dir_light;
            dir_light = new Ra::Engine::DirectionalLight( m_entity_selected,
                                                        m_name.toStdString() );
            m_direction = Core::Math::Vector3(m_dir_x_spin->value(),
                                              m_dir_y_spin->value(),
                                              m_dir_z_spin->value());
            dir_light->setDirection(m_direction);
            dir_light->setColor(c);
            emit sig_addLight(dir_light);

            break;
        case _POINT_LIGHT:
            Ra::Engine::PointLight * point_light;
            point_light = new Ra::Engine::PointLight( m_entity_selected,
                                                      m_name.toStdString() );
            m_position = Core::Math::Vector3(m_pos_x_spin->value(),
                                             m_pos_y_spin->value(),
                                             m_pos_z_spin->value());
            point_light->setPosition(m_position);
            point_light->setColor(c);
            point_light->setAttenuation((Scalar)m_falloff_val_constant,
                                        (Scalar)m_falloff_val_linear,
                                        (Scalar)m_falloff_val_quadratic);
            emit sig_addLight(point_light);
                break;
        case _SPOT_LIGHT:

            Ra::Engine::SpotLight * spot_light;
            spot_light = new Ra::Engine::SpotLight( m_entity_selected,
                                                    m_name.toStdString() );
            m_position = Core::Math::Vector3(m_pos_x_spin->value(),
                                             m_pos_y_spin->value(),
                                             m_pos_z_spin->value());
            spot_light->setPosition(m_position);
            m_direction = Core::Math::Vector3(m_dir_x_spin->value(),
                                              m_dir_y_spin->value(),
                                              m_dir_z_spin->value());
            spot_light->setDirection(m_direction);
            spot_light->setColor(c);
            spot_light->setInnerAngleInDegrees( m_inner_angle_val );
            spot_light->setOuterAngleInDegrees( m_outer_angle_val );
            spot_light->setAttenuation((Scalar)m_falloff_val_constant,
                                       (Scalar)m_falloff_val_linear,
                                       (Scalar)m_falloff_val_quadratic);

            emit sig_addLight(spot_light);
                break;

        default:
            // Default case is not expected
            assert(false);
    }

}


/*!
     \brief Slot that hides elements that are not required
     \param _DIR_LIGHT or _POINT_LIGHT or _SPOT_LIGHT define in LightCreator.hpp
     \return void
*/
void LightCreator::slot_select_light(const int type){
    m_lightType = type;
    /*
    usable in the switch    :

    emit sig_show_angle();
    emit sig_show_dir();
    emit sig_show_pos();
    emit sig_show_falloff();
    emit sig_hide_angle();
    emit sig_hide_dir();
    emit sig_hide_pos();
    emit sig_hide_falloff();
    */

    switch (type) {
        /*
        in the file ./LightCreator.hpp :
        - #define _DIR_LIGHT 0
        - #define _POINT_LIGHT 1
        - #define _SPOT_LIGHT 2
        */
        case _DIR_LIGHT: // Directionnal
            emit sig_show_dir();
            emit sig_hide_pos();
            emit sig_hide_falloff();
            emit sig_hide_angle(); break;
        case _POINT_LIGHT: // Point

                emit sig_show_pos();
                emit sig_show_falloff();
                emit sig_hide_dir();
                emit sig_hide_angle(); break;

        case _SPOT_LIGHT: // Spot
                emit sig_show_angle();
                emit sig_show_dir();
                emit sig_show_falloff();
                emit sig_show_pos();
                                      break;
        default:
        // Default case is not expected
            assert(false);

    }
}

/////////////////////////////////////////     ANGLES    SLOTS///////////////////
/*!
     \brief Slot that sync QDoubleSpinBox from QSlider of Inner Angles options
*/
void LightCreator::slot_inner_angle_slide_to_spin(const int val){
    double tmp =    ((double) val ) ;
    m_inner_angle_val = tmp/STEP_SLIDER;
    emit sig_inner_angle_slide_to_spin(m_inner_angle_val);
}
/*!
     \brief Slot that sync QSlider from QDoubleSpinBox of Inner Angles options
*/
void LightCreator::slot_inner_angle_spin_to_slide(const double val){
    m_inner_angle_val = val;
    int tmp = (int) (val*STEP_SLIDER);
    emit sig_inner_angle_spin_to_slide(tmp);
}
/*!
     \brief Slot that sync QDoubleSpinBox from QSlider of Outer Angles options
*/
void LightCreator::slot_outer_angle_slide_to_spin(const int val){
    double tmp =    ((double) val );
    m_outer_angle_val = tmp/STEP_SLIDER;
    emit sig_outer_angle_slide_to_spin(m_outer_angle_val);
}
/*!
     \brief Slot that sync QSlider from QDoubleSpinBox of Outer Angles options
*/
void LightCreator::slot_outer_angle_spin_to_slide(const double val){
    m_outer_angle_val = val;
    int tmp = (int) (val*STEP_SLIDER);
    emit sig_outer_angle_spin_to_slide(tmp);
}

/////////////////////////////////////     FALLOFF    SLOTS///////////////////
/*!
     \brief Slot that sync QDoubleSpinBox from QSlider of falloff options
*/
void LightCreator::slot_falloff_constant_slide_to_spin(const int val){
    double tmp =    (double) val;
    m_falloff_val_constant = tmp/STEP_SLIDER;
    emit sig_falloff_constant_slide_to_spin(m_falloff_val_constant);
}

/*!
     \brief Slot that sync QSlider from QDoubleSpinBox    of falloff options
*/
void LightCreator::slot_falloff_constant_spin_to_slide(const double val){
    m_falloff_val_constant = val;
    int tmp = (int)(val * STEP_SLIDER);
    emit sig_falloff_constant_spin_to_slide(tmp);
}

/*!
     \brief Slot that sync QDoubleSpinBox from QSlider of falloff options
*/
void LightCreator::slot_falloff_linear_slide_to_spin(const int val){
    double tmp =    (double) val;
    m_falloff_val_linear = tmp/STEP_SLIDER;
    emit sig_falloff_linear_slide_to_spin(m_falloff_val_linear);
}

/*!
     \brief Slot that sync QSlider from QDoubleSpinBox of falloff options
*/
void LightCreator::slot_falloff_linear_spin_to_slide(const double val){
    m_falloff_val_linear = val;
    int tmp = (int)(val * STEP_SLIDER);
    emit sig_falloff_linear_spin_to_slide(tmp);
}

/*!
     \brief Slot that sync QDoubleSpinBox from QSlider of falloff options
*/
void LightCreator::slot_falloff_quadratic_slide_to_spin(const int val){
    double tmp = (double) val;
    m_falloff_val_quadratic = tmp/STEP_SLIDER;
    emit sig_falloff_quadratic_slide_to_spin(m_falloff_val_quadratic);
}

/*!
     \brief Slot that sync QSlider from QDoubleSpinBox of falloff options
*/
void LightCreator::slot_falloff_quadratic_spin_to_slide(const double val){
    m_falloff_val_quadratic = val;
    int tmp = (int) (val*STEP_SLIDER);
    emit sig_falloff_quadratic_spin_to_slide(tmp);
}

void LightCreator::slot_start(){
    // Reset and init the combobox used for entities
    m_entity_group_box->clear();
    m_entity_group_box->addItem("");
    m_entities_vector = Ra::Engine::Entity::getEntityMgr()->getEntities();
    for (int i = 0; i < m_entities_vector.size(); ++i){
            m_entity_group_box->addItem(
                QString::fromStdString(m_entities_vector[i]->getName())
              );
    }
    emit sig_show();
}

void LightCreator::slot_selected_entity(const QString &m_name_entity){
    if(m_name_entity != QString(""))
    m_entity_selected = Ra::Engine::Entity::getEntityMgr()->getEntity(
        m_name_entity.toStdString()
      );
}

} // namespace Gui
} // namespace Ra
