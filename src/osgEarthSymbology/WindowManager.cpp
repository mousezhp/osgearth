/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2009 Pelican Ventures, Inc.
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <osgEarthSymbology/WindowManager>
#include <osgEarthSymbology/WidgetMessageBox>
#include <osgWidget/ViewerEventHandlers>
#include <osgViewer/Viewer>

using namespace osgEarth::Symbology;

WindowManager::WindowManager(osgViewer::Viewer& viewer)
{
    const unsigned int MASK_2D = 0xF0000000;
    osgWidget::WindowManager* wm = new osgWidget::WindowManager(
        &viewer,
        viewer.getCamera()->getGraphicsContext()->getTraits()->width,
        viewer.getCamera()->getGraphicsContext()->getTraits()->height,
        MASK_2D,
        0 //osgWidget::WindowManager::WM_PICK_DEBUG
        );
    osg::Camera* camera = wm->createParentOrthoCamera();
    viewer.getSceneData()->asGroup()->addChild(camera);
    viewer.addEventHandler(new osgWidget::MouseHandler(wm));
    viewer.addEventHandler(new osgWidget::KeyboardHandler(wm));
    viewer.addEventHandler(new osgWidget::ResizeHandler(wm, camera));
    viewer.addEventHandler(new osgWidget::CameraSwitchHandler(wm, camera));
    wm->resizeAllWindows();
    _windowManager = wm;
}


void WindowManager::popUp(WidgetMessageBox& msg)
{
    _windowManager->addChild(msg.getWindow());

    osgWidget::point_type w = _windowManager->getWidth();
    osgWidget::point_type h = _windowManager->getHeight();
    osgWidget::point_type ww = msg.getWindow()->getWidth();
    osgWidget::point_type hw = msg.getWindow()->getHeight();
    osgWidget::point_type ox = (w - ww) / 2;
    osgWidget::point_type oy = (h - hw) / 2;
    msg.getWindow()->setPosition(osgWidget::Point(osg::round(ox), osg::round(oy), msg.getWindow()->getPosition()[2] ));
    _windowManager->resizeAllWindows();
}