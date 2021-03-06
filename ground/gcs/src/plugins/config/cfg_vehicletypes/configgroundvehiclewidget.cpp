/**
 ******************************************************************************
 *
 * @file       configgroundvehiclemwidget.cpp
 * @author     K. Sebesta & The OpenPilot Team, http://www.openpilot.org Copyright (C) 2012.
 * @author     Tau Labs, http://taulabs.org, Copyright 2013
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup ConfigPlugin Config Plugin
 * @{
 * @brief ccpm configuration panel
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 */
#include "actuatorcommand.h"
#include "configgroundvehiclewidget.h"
#include "configvehicletypewidget.h"
#include "mixersettings.h"

#include <QDebug>
#include <QStringList>
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QBrush>
#include <math.h>
#include <QMessageBox>

/**
 Constructor
 */
ConfigGroundVehicleWidget::ConfigGroundVehicleWidget(Ui_AircraftWidget *aircraft, QWidget *parent)
    : VehicleConfig(parent)
{
    m_aircraft = aircraft;
}

/**
 Destructor
 */
ConfigGroundVehicleWidget::~ConfigGroundVehicleWidget()
{
    // Do nothing
}

/**
 Virtual function to setup the UI
 */
void ConfigGroundVehicleWidget::setupUI(SystemSettings::AirframeTypeOptions frameType)
{
    m_aircraft->differentialSteeringMixBox->setHidden(true);
    // STILL NEEDS WORK
    // Setup the UI
    setComboCurrentIndex(m_aircraft->aircraftType, m_aircraft->aircraftType->findText("Ground"));

    m_aircraft->gvEngineChannelBox->setEnabled(false);
    m_aircraft->gvEngineLabel->setEnabled(false);

    m_aircraft->gvAileron1ChannelBox->setEnabled(false);
    m_aircraft->gvAileron1Label->setEnabled(false);

    m_aircraft->gvAileron2ChannelBox->setEnabled(false);
    m_aircraft->gvAileron2Label->setEnabled(false);

    if (frameType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEDIFFERENTIAL) { // Tank
        setComboCurrentIndex(m_aircraft->groundVehicleType,
                             m_aircraft->groundVehicleType->findText("Differential (tank)"));
        m_aircraft->gvMotor1ChannelBox->setEnabled(true);
        m_aircraft->gvMotor1Label->setEnabled(true);

        m_aircraft->gvMotor2ChannelBox->setEnabled(true);
        m_aircraft->gvMotor2Label->setEnabled(true);

        m_aircraft->gvMotor1Label->setText("Left motor");
        m_aircraft->gvMotor2Label->setText("Right motor");

        m_aircraft->gvSteering1ChannelBox->setEnabled(false);
        m_aircraft->gvSteering1Label->setEnabled(false);

        m_aircraft->gvSteering2ChannelBox->setEnabled(false);
        m_aircraft->gvSteering2Label->setEnabled(false);

        m_aircraft->gvSteering2Label->setText("Rear steering");

        m_aircraft->differentialSteeringMixBox->setHidden(false);

    } else if (frameType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEMOTORCYCLE) { // Motorcycle
        setComboCurrentIndex(m_aircraft->groundVehicleType,
                             m_aircraft->groundVehicleType->findText("Motorcycle"));
        m_aircraft->gvMotor1ChannelBox->setEnabled(false);
        m_aircraft->gvMotor1Label->setEnabled(false);

        m_aircraft->gvMotor2ChannelBox->setEnabled(true);
        m_aircraft->gvMotor2Label->setEnabled(true);

        m_aircraft->gvMotor1Label->setText("Front motor");
        m_aircraft->gvMotor2Label->setText("Rear motor");

        m_aircraft->gvSteering1ChannelBox->setEnabled(true);
        m_aircraft->gvSteering1Label->setEnabled(true);

        m_aircraft->gvSteering2ChannelBox->setEnabled(true);
        m_aircraft->gvSteering2Label->setEnabled(true);

        m_aircraft->gvSteering2Label->setText("Balancing");

        m_aircraft->differentialSteeringMixBox->setHidden(true);
    } else { // Car
        setComboCurrentIndex(m_aircraft->groundVehicleType,
                             m_aircraft->groundVehicleType->findText("Turnable (car)"));

        m_aircraft->gvMotor1ChannelBox->setEnabled(true);
        m_aircraft->gvMotor1Label->setEnabled(true);

        m_aircraft->gvMotor2ChannelBox->setEnabled(true);
        m_aircraft->gvMotor2Label->setEnabled(true);

        m_aircraft->gvMotor1Label->setText("Front motor");
        m_aircraft->gvMotor2Label->setText("Rear motor");

        m_aircraft->gvSteering1ChannelBox->setEnabled(true);
        m_aircraft->gvSteering1Label->setEnabled(true);

        m_aircraft->gvSteering2ChannelBox->setEnabled(true);
        m_aircraft->gvSteering2Label->setEnabled(true);

        m_aircraft->differentialSteeringMixBox->setHidden(true);
    }
}

void ConfigGroundVehicleWidget::ResetActuators(GUIConfigDataUnion *configData)
{
    configData->ground.GroundVehicleSteering1 = 0;
    configData->ground.GroundVehicleSteering2 = 0;
    configData->ground.GroundVehicleThrottle1 = 0;
    configData->ground.GroundVehicleThrottle2 = 0;
}

QStringList ConfigGroundVehicleWidget::getChannelDescriptions()
{
    int i;
    QStringList channelDesc;

    // init a channel_numelem list of channel desc defaults
    for (i = 0; i < (int)(ActuatorCommand::CHANNEL_NUMELEM); i++) {
        channelDesc.append(QString("-"));
    }

    // get the gui config data
    GUIConfigDataUnion configData = GetConfigData();

    if (configData.ground.GroundVehicleSteering1 > 0)
        channelDesc[configData.ground.GroundVehicleSteering1 - 1] = QString("GroundSteering1");
    if (configData.ground.GroundVehicleSteering2 > 0)
        channelDesc[configData.ground.GroundVehicleSteering2 - 1] = QString("GroundSteering2");
    if (configData.ground.GroundVehicleThrottle1 > 0)
        channelDesc[configData.ground.GroundVehicleThrottle1 - 1] = QString("GroundThrottle1");
    if (configData.ground.GroundVehicleThrottle2 > 0)
        channelDesc[configData.ground.GroundVehicleThrottle2 - 1] = QString("GroundThrottle2");

    return channelDesc;
}

/**
 * @brief ConfigFixedWingWidget::updateConfigObjectsFromWidgets update the UI widget objects
 * @return airframe type
 */
SystemSettings::AirframeTypeOptions ConfigGroundVehicleWidget::updateConfigObjectsFromWidgets()
{
    SystemSettings::AirframeTypeOptions airframeType =
        SystemSettings::AIRFRAMETYPE_GROUNDVEHICLECAR;

    // Save the curve (common to all ground vehicle frames)
    MixerSettings *mixerSettings = MixerSettings::GetInstance(getObjectManager());
    Q_ASSERT(mixerSettings);

    // All airframe types must start with "GroundVehicle"
    if (m_aircraft->groundVehicleType->currentText() == "Turnable (car)") {
        airframeType = SystemSettings::AIRFRAMETYPE_GROUNDVEHICLECAR;
        setupGroundVehicleCar(airframeType);
    } else if (m_aircraft->groundVehicleType->currentText() == "Differential (tank)") {
        airframeType = SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEDIFFERENTIAL;
        setupGroundVehicleDifferential(airframeType);
    } else { // "Motorcycle"
        airframeType = SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEMOTORCYCLE;
        setupGroundVehicleMotorcycle(airframeType);
    }

    return airframeType;
}

/**
 Virtual function to refresh the UI widget values
 */
void ConfigGroundVehicleWidget::refreshAirframeWidgetsValues(
    SystemSettings::AirframeTypeOptions frameType)
{
    MixerSettings *mixerSettings = MixerSettings::GetInstance(getObjectManager());
    Q_ASSERT(mixerSettings);

    GUIConfigDataUnion config = GetConfigData();

    // THIS SECTION STILL NEEDS WORK. FOR THE MOMENT, USE THE FIXED-WING ONBOARD SETTING IN ORDER TO
    // MINIMIZE CHANCES OF BOLLOXING REAL CODE
    // Retrieve channel setup values
    setComboCurrentIndex(m_aircraft->gvMotor1ChannelBox, config.ground.GroundVehicleThrottle1);
    setComboCurrentIndex(m_aircraft->gvMotor2ChannelBox, config.ground.GroundVehicleThrottle2);
    setComboCurrentIndex(m_aircraft->gvSteering1ChannelBox, config.ground.GroundVehicleSteering1);
    setComboCurrentIndex(m_aircraft->gvSteering2ChannelBox, config.ground.GroundVehicleSteering2);

    if (frameType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEDIFFERENTIAL) {
        // CURRENTLY BROKEN UNTIL WE DECIDE HOW DIFFERENTIAL SHOULD BEHAVE
        // If the vehicle type is "differential", restore the slider setting

        // Find the channel number for Motor1
        int channel = m_aircraft->gvMotor1ChannelBox->currentIndex() - 1;
        if (channel > -1) { // If for some reason the actuators were incoherent, we might fail here,
                            // hence the check.

            m_aircraft->differentialSteeringSlider1->setValue(
                getMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_ROLL)
                * 100);
            m_aircraft->differentialSteeringSlider2->setValue(
                getMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_PITCH)
                * 100);
        }
    }
    if (frameType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEMOTORCYCLE) {
        // CURRENTLY BROKEN UNTIL WE DECIDE HOW MOTORCYCLE SHOULD BEHAVE
        //		obj =
        //dynamic_cast<UAVDataObject*>(getObjectManager()->getObject(QString("MixerSettings")));
        //		Q_ASSERT(obj);
        //		int chMixerNumber = m_aircraft->gvMotor1ChannelBox->currentIndex()-1;
        //		if (chMixerNumber >=0) {
        //			field = obj->getField(mixerVectors.at(chMixerNumber));
        //			int ti = field->getElementNames().indexOf("Yaw");
        //			m_aircraft->differentialSteeringSlider1->setValue(field->getDouble(ti)*100);
        //
        //			ti = field->getElementNames().indexOf("Pitch");
        //			m_aircraft->differentialSteeringSlider2->setValue(field->getDouble(ti)*100);
        //		}
    }
}

/**
 Setup balancing ground vehicle.

 Returns False if impossible to create the mixer.
 */
bool ConfigGroundVehicleWidget::setupGroundVehicleMotorcycle(
    SystemSettings::AirframeTypeOptions airframeType)
{
    // Check coherence:
    // Show any config errors in GUI
    if (throwConfigError(airframeType)) {
        return false;
    }

    // Now setup the channels:
    GUIConfigDataUnion config = GetConfigData();
    ResetActuators(&config);

    config.ground.GroundVehicleThrottle1 = m_aircraft->gvMotor1ChannelBox->currentIndex();
    config.ground.GroundVehicleThrottle2 = m_aircraft->gvMotor2ChannelBox->currentIndex();

    SetConfigData(config, SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEMOTORCYCLE);

    MixerSettings *mixerSettings = MixerSettings::GetInstance(getObjectManager());
    Q_ASSERT(mixerSettings);
    resetMixers(mixerSettings);

    // motor
    int channel = m_aircraft->gvMotor2ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_MOTOR);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_THROTTLECURVE1,
                        mixerScale);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, mixerScale);

    // steering
    channel = m_aircraft->gvSteering1ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_SERVO);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, -mixerScale);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_ROLL, -mixerScale);

    // balance
    channel = m_aircraft->gvSteering2ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_SERVO);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, mixerScale);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_ROLL, mixerScale);

    m_aircraft->gvStatusLabel->setText("Mixer generated");

    return true;
}

/**
 Setup differentially steered ground vehicle.

 Returns False if impossible to create the mixer.
 */
bool ConfigGroundVehicleWidget::setupGroundVehicleDifferential(
    SystemSettings::AirframeTypeOptions airframeType)
{
    // Check coherence:
    // Show any config errors in GUI

    if (throwConfigError(airframeType)) {
        return false;
    }

    // Now setup the channels:
    GUIConfigDataUnion config = GetConfigData();
    ResetActuators(&config);

    config.ground.GroundVehicleThrottle1 = m_aircraft->gvMotor1ChannelBox->currentIndex();
    config.ground.GroundVehicleThrottle2 = m_aircraft->gvMotor2ChannelBox->currentIndex();

    SetConfigData(config, SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEDIFFERENTIAL);

    MixerSettings *mixerSettings = MixerSettings::GetInstance(getObjectManager());
    Q_ASSERT(mixerSettings);
    resetMixers(mixerSettings);

    // left motor
    int channel = m_aircraft->gvMotor1ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_MOTOR);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_THROTTLECURVE1,
                        mixerScale);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, mixerScale);

    // right motor
    channel = m_aircraft->gvMotor2ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_MOTOR);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_THROTTLECURVE2,
                        mixerScale);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, -mixerScale);

    // Output success message
    m_aircraft->gvStatusLabel->setText("Mixer generated");

    return true;
}

/**
 Setup steerable ground vehicle.

 Returns False if impossible to create the mixer.
 */
bool ConfigGroundVehicleWidget::setupGroundVehicleCar(
    SystemSettings::AirframeTypeOptions airframeType)
{
    // Check coherence:
    // Show any config errors in GUI
    if (throwConfigError(airframeType)) {
        return false;
    }

    // Now setup the channels:
    GUIConfigDataUnion config = GetConfigData();
    ResetActuators(&config);

    config.ground.GroundVehicleThrottle1 = m_aircraft->gvMotor1ChannelBox->currentIndex();
    config.ground.GroundVehicleThrottle2 = m_aircraft->gvMotor2ChannelBox->currentIndex();
    config.ground.GroundVehicleSteering1 = m_aircraft->gvSteering1ChannelBox->currentIndex();
    config.ground.GroundVehicleSteering2 = m_aircraft->gvSteering2ChannelBox->currentIndex();

    SetConfigData(config, SystemSettings::AIRFRAMETYPE_GROUNDVEHICLECAR);

    MixerSettings *mixerSettings = MixerSettings::GetInstance(getObjectManager());
    Q_ASSERT(mixerSettings);
    resetMixers(mixerSettings);

    int channel = m_aircraft->gvSteering1ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_SERVO);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, mixerScale);

    channel = m_aircraft->gvSteering2ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_SERVO);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_YAW, -mixerScale);

    channel = m_aircraft->gvMotor1ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_MOTOR);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_THROTTLECURVE1,
                        mixerScale);

    channel = m_aircraft->gvMotor2ChannelBox->currentIndex() - 1;
    setMixerType(mixerSettings, channel, MixerSettings::MIXER1TYPE_MOTOR);
    setMixerVectorValue(mixerSettings, channel, MixerSettings::MIXER1VECTOR_THROTTLECURVE2,
                        mixerScale);

    // Output success message
    m_aircraft->gvStatusLabel->setText("Mixer generated");

    return true;
}

/**
 This function displays text and color formatting in order to help the user understand what channels
 have not yet been configured.
 */
bool ConfigGroundVehicleWidget::throwConfigError(SystemSettings::AirframeTypeOptions airframeType)
{
    // Initialize configuration error flag
    bool error = false;

    // Create a red block. All combo boxes are the same size, so any one should do as a model
    int size = m_aircraft->gvEngineChannelBox->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size, size);
    pixmap.fill(QColor("red"));

    if (airframeType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLECAR) { // Car
        if (m_aircraft->gvMotor1ChannelBox->currentText() == "None"
            && m_aircraft->gvMotor2ChannelBox->currentText() == "None") {
            pixmap.fill(QColor("green"));
            m_aircraft->gvMotor1ChannelBox->setItemData(0, pixmap,
                                                        Qt::DecorationRole); // Set color palettes
            m_aircraft->gvMotor2ChannelBox->setItemData(0, pixmap,
                                                        Qt::DecorationRole); // Set color palettes
            //			m_aircraft->gvMotor1Label->setText("<font color='red'>" +
            //m_aircraft->gvMotor1Label->text() + "</font>");
            //			m_aircraft->gvMotor2Label->setText("<font color='red'>" +
            //m_aircraft->gvMotor2Label->text() + "</font>");
            error = true;

        } else {
            m_aircraft->gvMotor1ChannelBox->setItemData(0, 0,
                                                        Qt::DecorationRole); // Reset color palettes
            m_aircraft->gvMotor2ChannelBox->setItemData(0, 0,
                                                        Qt::DecorationRole); // Reset color palettes
            //			QTextEdit* htmlText=new QTextEdit(m_aircraft->gvMotor1Label->text());  //
            //HtmlText is any QString with html tags.
            //			m_aircraft->gvMotor1Label->setText(htmlText->toPlainText());
            //			delete htmlText;
            //
            //			htmlText=new QTextEdit(m_aircraft->gvMotor2Label->text());  // HtmlText is any
            //QString with html tags.
            //			m_aircraft->gvMotor2Label->setText(htmlText->toPlainText());
        }

        if (m_aircraft->gvSteering1ChannelBox->currentText() == "None"
            && m_aircraft->gvSteering2ChannelBox->currentText() == "None") {
            m_aircraft->gvSteering1ChannelBox->setItemData(
                0, pixmap, Qt::DecorationRole); // Set color palettes
            m_aircraft->gvSteering2ChannelBox->setItemData(
                0, pixmap, Qt::DecorationRole); // Set color palettes
            //			m_aircraft->gvStatusLabel->setText("<font color='red'>ERROR: check steering
            //channel assignment</font>");
            //			m_aircraft->gvSteering1Label->setText("<font color='red'>" +
            //m_aircraft->gvSteering1Label->text() + "</font>");
            //			m_aircraft->gvSteering2Label->setText("<font color='red'>" +
            //m_aircraft->gvSteering2Label->text() + "</font>");
            error = true;
        } else {
            m_aircraft->gvSteering1ChannelBox->setItemData(
                0, 0, Qt::DecorationRole); // Reset color palettes
            m_aircraft->gvSteering2ChannelBox->setItemData(
                0, 0, Qt::DecorationRole); // Reset color palettes
            //			QTextEdit* htmlText=new QTextEdit(m_aircraft->gvSteering1Label->text());  //
            //HtmlText is any QString with html tags.
            //			m_aircraft->gvSteering1Label->setText(htmlText->toPlainText());
            //			delete htmlText;
            //
            //			htmlText=new QTextEdit(m_aircraft->gvSteering2Label->text());  // HtmlText is
            //any QString with html tags.
            //			m_aircraft->gvSteering2Label->setText(htmlText->toPlainText());
        }

    } else if (airframeType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEDIFFERENTIAL) { // Tank
        if (m_aircraft->gvMotor1ChannelBox->currentText() == "None"
            || m_aircraft->gvMotor2ChannelBox->currentText() == "None") {
            m_aircraft->gvMotor1ChannelBox->setItemData(0, pixmap,
                                                        Qt::DecorationRole); // Set color palettes
            m_aircraft->gvMotor2ChannelBox->setItemData(0, pixmap,
                                                        Qt::DecorationRole); // Set color palettes
            error = true;
        } else {
            m_aircraft->gvMotor1ChannelBox->setItemData(0, 0,
                                                        Qt::DecorationRole); // Reset color palettes
            m_aircraft->gvMotor2ChannelBox->setItemData(0, 0,
                                                        Qt::DecorationRole); // Reset color palettes
        }

        // Always reset
        m_aircraft->gvSteering1ChannelBox->setItemData(0, 0,
                                                       Qt::DecorationRole); // Reset color palettes
        m_aircraft->gvSteering2ChannelBox->setItemData(0, 0,
                                                       Qt::DecorationRole); // Reset color palettes
    } else if (airframeType == SystemSettings::AIRFRAMETYPE_GROUNDVEHICLEMOTORCYCLE) { // Motorcycle
        if (m_aircraft->gvMotor1ChannelBox->currentText() == "None"
            && m_aircraft->gvMotor2ChannelBox->currentText() == "None") {
            m_aircraft->gvMotor2ChannelBox->setItemData(0, pixmap,
                                                        Qt::DecorationRole); // Set color palettes
            error = true;
        } else {
            m_aircraft->gvMotor2ChannelBox->setItemData(0, 0,
                                                        Qt::DecorationRole); // Reset color palettes
        }

        if (m_aircraft->gvSteering1ChannelBox->currentText() == "None"
            && m_aircraft->gvSteering2ChannelBox->currentText() == "None") {
            m_aircraft->gvSteering1ChannelBox->setItemData(
                0, pixmap, Qt::DecorationRole); // Set color palettes
            error = true;
        } else {
            m_aircraft->gvSteering1ChannelBox->setItemData(
                0, 0, Qt::DecorationRole); // Reset color palettes
        }

        // Always reset
        m_aircraft->gvMotor1ChannelBox->setItemData(0, 0,
                                                    Qt::DecorationRole); // Reset color palettes
        m_aircraft->gvSteering2ChannelBox->setItemData(0, 0,
                                                       Qt::DecorationRole); // Reset color palettes
    }

    if (error) {
        m_aircraft->gvStatusLabel->setText(
            QString("<font color='red'>ERROR: Assign all necessary channels</font>"));
    }
    return error;
}
