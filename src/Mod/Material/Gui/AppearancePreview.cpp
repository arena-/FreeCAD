/***************************************************************************
 *   Copyright (c) 2023 David Carter <dcarter@david.carter.ca>             *
 *                                                                         *
 *   This file is part of FreeCAD.                                         *
 *                                                                         *
 *   FreeCAD is free software: you can redistribute it and/or modify it    *
 *   under the terms of the GNU Lesser General Public License as           *
 *   published by the Free Software Foundation, either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful, but        *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with FreeCAD. If not, see                               *
 *   <https://www.gnu.org/licenses/>.                                      *
 *                                                                         *
 **************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
#endif

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>

#include <App/Application.h>
#include <Base/Parameter.h>

#include "AppearancePreview.h"

using namespace MatGui;

/* TRANSLATOR MatGui::AppearancePreview */

AppearanceSettings::AppearanceSettings(const ParameterGrp::handle& hGrp,
                                       Gui::View3DInventorViewer* view)
    : Gui::View3DSettings(hGrp, view)
{}

AppearanceSettings::AppearanceSettings(const ParameterGrp::handle& hGrp,
                                       const std::vector<Gui::View3DInventorViewer*>& view)
    : Gui::View3DSettings(hGrp, view)
{}

void AppearanceSettings::OnChange(ParameterGrp::SubjectType& rCaller,
                                  ParameterGrp::MessageType Reason)
{
    // Exclude settings that either we're not interested in, or that
    // may interfere with functionality
    if (strcmp(Reason, "CornerCoordSystem") == 0) {
        return;
    }
    if (strcmp(Reason, "CornerCoordSystemSize") == 0) {
        return;
    }
    if (strcmp(Reason, "ShowAxisCross") == 0) {
        return;
    }
    if (strcmp(Reason, "UseNavigationAnimations") == 0) {
        return;
    }
    if (strcmp(Reason, "ShowFPS") == 0) {
        return;
    }
    if (strcmp(Reason, "ShowNaviCube") == 0) {
        return;
    }
    if (strcmp(Reason, "UseVBO") == 0) {
        return;
    }
    if (strcmp(Reason, "RenderCache") == 0) {
        return;
    }
    if (strcmp(Reason, "Orthographic") == 0) {
        return;
    }
    if (strcmp(Reason, "NavigationStyle") == 0) {
        return;
    }
    if (strcmp(Reason, "OrbitStyle") == 0) {
        return;
    }
    if (strcmp(Reason, "Sensitivity") == 0) {
        return;
    }
    if (strcmp(Reason, "ResetCursorPosition") == 0) {
        return;
    }
    if (strcmp(Reason, "DimensionsVisible") == 0) {
        return;
    }
    if (strcmp(Reason, "Dimensions3dVisible") == 0) {
        return;
    }
    if (strcmp(Reason, "DimensionsDeltaVisible") == 0) {
        return;
    }
    if (strcmp(Reason, "PickRadius") == 0) {
        return;
    }
    if (strcmp(Reason, "TransparentObjectRenderType") == 0) {
        return;
    }

    View3DSettings::OnChange(rCaller, Reason);
}

//===

AppearancePreview::AppearancePreview(QWidget* parent)
    : Gui::View3DInventorViewer(parent)
{
    setRedirectToSceneGraph(true);
    setViewing(true);
    setPopupMenuEnabled(false);

    applySettings();
    // setBackground();
    setEnabledNaviCube(false);

    auto root = dynamic_cast<SoSeparator*>(getSceneGraph());
    _material = new SoMaterial();
    _material->ref();
    root->addChild(_material);
    root->addChild(new SoSphere());

    setCameraType(SoOrthographicCamera::getClassTypeId());
    setViewDirection(SbVec3f(1, 1, -5));
    viewAll();
}

AppearancePreview::~AppearancePreview()
{
    _material->unref();
    _material = nullptr;
}

void AppearancePreview::applySettings()
{
    viewSettings = std::make_unique<AppearanceSettings>(
        App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/View"),
        this);
    viewSettings->applySettings();
}

void AppearancePreview::setAmbientColor(const QColor& color)
{
    _material->ambientColor.setValue(
        SbColor(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0));
    _material->ambientColor.setDefault(false);
}

void AppearancePreview::setDiffuseColor(const QColor& color)
{
    _material->diffuseColor.setValue(
        SbColor(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0));
    _material->diffuseColor.setDefault(false);
}

void AppearancePreview::setSpecularColor(const QColor& color)
{
    _material->specularColor.setValue(
        SbColor(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0));
    _material->specularColor.setDefault(false);
}

void AppearancePreview::setEmissiveColor(const QColor& color)
{
    _material->emissiveColor.setValue(
        SbColor(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0));
    _material->emissiveColor.setDefault(false);
}

void AppearancePreview::setShininess(double value)
{
    _material->shininess.setValue(value);
    _material->shininess.setDefault(false);
}

void AppearancePreview::setTransparency(double value)
{
    _material->transparency.setValue(value);
    _material->transparency.setDefault(false);
}

void AppearancePreview::resetAmbientColor()
{
    _material->ambientColor.deleteValues(0);
    _material->ambientColor.setDefault(true);
}

void AppearancePreview::resetDiffuseColor()
{
    _material->diffuseColor.deleteValues(0);
    _material->diffuseColor.setDefault(true);
}

void AppearancePreview::resetSpecularColor()
{
    _material->specularColor.deleteValues(0);
    _material->specularColor.setDefault(true);
}

void AppearancePreview::resetEmissiveColor()
{
    _material->emissiveColor.deleteValues(0);
    _material->emissiveColor.setDefault(true);
}

void AppearancePreview::resetShininess()
{
    _material->shininess.deleteValues(0);
    _material->shininess.setDefault(true);
}

void AppearancePreview::resetTransparency()
{
    _material->transparency.deleteValues(0);
    _material->transparency.setDefault(true);
}
