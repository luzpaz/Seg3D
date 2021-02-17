/*
 For more information, please see: http://software.sci.utah.edu

 The MIT License

 Copyright (c) 2016 Scientific Computing and Imaging Institute,
 University of Utah.


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

// Application includes
#include <Application/PreferencesManager/PreferencesManager.h>

// QtUtils includes
#include <QtUtils/Bridge/QtBridge.h>
#include <QColorDialog>

// Interface includes
#include <Interface/Application/ColorPickerWidget.h>

// The interface from the designer
#include "ui_ColorPickerWidget.h"

namespace Seg3D
{

class ColorPickerWidgetPrivate
{
public:
  Ui::ColorPickerWidget ui_;

};

ColorPickerWidget::ColorPickerWidget( QWidget *parent ) :
    QWidget( parent ),
    private_( new ColorPickerWidgetPrivate )
{
  private_->ui_.setupUi( this );

  connect( this, SIGNAL( color_changed() ), this, SLOT( set_color() ) );

  connect( this->private_->ui_.set_color_button_, SIGNAL( clicked() ),
    this, SLOT( signal_color_set() ) );
}

ColorPickerWidget::~ColorPickerWidget()
{
}

void ColorPickerWidget::set_color()
{
  QString style_sheet = QString::fromUtf8( "QWidget#color_sample_{"
    "background-color: rgb(" ) + QString::number( this->r_ ) +
    QString::fromUtf8( ", " ) + QString::number( this->g_ ) +
    QString::fromUtf8( ", " ) + QString::number( this->b_ ) +
    QString::fromUtf8( "); }" );

  this->private_->ui_.color_sample_->setStyleSheet( style_sheet );
  this->private_->ui_.color_sample_->repaint();
}

void ColorPickerWidget::hide_show( Core::Color color, bool show )
{
  if ( !show )
  {
    this->setVisible( false );
    return;
  }

  this->r_ = static_cast< int >( color.r() );
  this->g_ = static_cast< int >( color.g() );
  this->b_ = static_cast< int >( color.b() );

  this->set_color();

  this->setVisible( true );

}

void ColorPickerWidget::signal_color_set()
{
  Core::StateEngine::lock_type lock(Core::StateEngine::GetMutex());

  QColor color = QColorDialog::getColor(QColor(159, 185, 255), this);

  if (color.isValid())
  {
    this->r_ = static_cast<int>(color.red());
    this->g_ = static_cast<int>(color.green());
    this->b_ = static_cast<int>(color.blue());
    Q_EMIT color_set(Core::Color(this->r_, this->g_, this->b_));
  }

}

} // end namespace Seg3D
