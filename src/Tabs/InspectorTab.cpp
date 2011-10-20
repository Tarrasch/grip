/*
 * Copyright (c) 2010, Georgia Tech Research Corporation
 * 
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the Georgia Tech Research Corporation nor
 *       the names of its contributors may be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GEORGIA
 * TECH RESEARCH CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <wx/wx.h>

#include "InspectorTab.h"

#include <GUI/Viewer.h>
#include <GUI/GUI.h>
#include <GUI/GRIPSlider.h>
#include <GUI/GRIPFrame.h>

#include <kinematics/BodyNode.h>
#include <kinematics/Joint.h>
#include <kinematics/Dof.h>
#include <planning/Robot.h>
#include <planning/Object.h>

using namespace std;
using namespace Eigen;

//Give each slider a number so we recognize them
enum sliderNames{
	J_SLIDER = 1000,
	X_SLIDER = 1001,
	Y_SLIDER = 1002,
	Z_SLIDER = 1003,
	ROLL_SLIDER = 1004,
	PITCH_SLIDER = 1005,
	YAW_SLIDER = 1006
};

//Add a handler for slider changes
BEGIN_EVENT_TABLE(InspectorTab, wxPanel)
EVT_COMMAND (wxID_ANY, wxEVT_GRIP_SLIDER_CHANGE, InspectorTab::OnSlider)
END_EVENT_TABLE()

// Class constructor for the tab: Each tab will be a subclass of wxPanel
IMPLEMENT_DYNAMIC_CLASS(InspectorTab, GRIPTab)
InspectorTab::InspectorTab(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
					   : GRIPTab(parent, id, pos, size, style)
 {
      sizerFull = new wxBoxSizer(wxHORIZONTAL);

      wxStaticBox* jointBox = new wxStaticBox(this,-1,wxT("Item Information"));
      wxStaticBox* posBox = new wxStaticBox(this,-1,wxT("Position"));
      wxStaticBox* rotBox = new wxStaticBox(this,-1,wxT("Orientation"));

      wxStaticBoxSizer* jointBoxS = new wxStaticBoxSizer(jointBox, wxVERTICAL);
      wxStaticBoxSizer* posBoxS = new wxStaticBoxSizer(posBox, wxVERTICAL);
      wxStaticBoxSizer* rotBoxS = new wxStaticBoxSizer(rotBox, wxVERTICAL);

      sizerFull->Add(jointBoxS, 1, wxEXPAND | wxALL, 6);
      sizerFull->Add(posBoxS, 1, wxEXPAND | wxALL, 6);
      sizerFull->Add(rotBoxS, 1, wxEXPAND | wxALL, 6);
      SetSizer(sizerFull);

      // Add the Joint Information and Joint Slider
      itemName = new wxStaticText(this,-1,wxT("Item: (none)"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);
      parentName = new wxStaticText(this,-1,wxT("Parent: (none)"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);

      jSlider = new GRIPSlider("Joint",-180,180,2000,0,1000,2000,this,J_SLIDER);
      jointBoxS->Add( parentName, 1, wxEXPAND | wxALL, 6 );
      jointBoxS->Add(itemName,1,wxEXPAND | wxALL, 6);
      jointBoxS->Add(jSlider,1,wxEXPAND | wxALL, 6);

      xSlider = new GRIPSlider("X",-10,10,500,0,100,500,this,X_SLIDER);
      ySlider = new GRIPSlider("Y",-10,10,500,0,100,500,this,Y_SLIDER);
      zSlider = new GRIPSlider("Z",-10,10,500,0,100,500,this,Z_SLIDER);
      posBoxS->Add( xSlider,1,wxEXPAND | wxALL, 6 );
      posBoxS->Add( ySlider,1,wxEXPAND | wxALL, 6 );
      posBoxS->Add( zSlider,1,wxEXPAND | wxALL, 6 );


      rollSlider = new GRIPSlider("R",-180,180,500,0,100,500,this,ROLL_SLIDER);
      pitchSlider = new GRIPSlider("P",-180,180,500,0,100,500,this,PITCH_SLIDER);
      yawSlider = new GRIPSlider("Y",-180,180,500,0,100,500,this,YAW_SLIDER);
      rotBoxS->Add(rollSlider,1,wxEXPAND | wxALL, 6);
      rotBoxS->Add(pitchSlider,1,wxEXPAND | wxALL, 6);
      rotBoxS->Add(yawSlider,1,wxEXPAND | wxALL, 6);

 }

/**
 * @function OnSlider
 * @brief Handle slider changes
 * @date 2011-10-13
 */
void InspectorTab::OnSlider(wxCommandEvent &evt) {

    planning::Object* pObject;
    kinematics::BodyNode* pBodyNode;
    planning::Robot* pRobot;

    int slnum = evt.GetId();
    double pos = *(double*)evt.GetClientData();
    char numBuf[1000];
    numBuf[0] = '\0';
    //sprintf(numBuf,"");

	if(selectedTreeNode == NULL){ return; }

	int selected = selectedTreeNode->dType;
	if(selected == Return_Type_Object){
            pObject = (planning::Object*)(selectedTreeNode->data);
	}
	else if(selected == Return_Type_Node){
            pBodyNode = (kinematics::BodyNode*)(selectedTreeNode->data);
	}
	else if(selected == Return_Type_Robot){
            pRobot = (planning::Robot*)(selectedTreeNode->data);
	}
  
        /// 
	if(slnum == J_SLIDER && selected == Return_Type_Node){

		if( pBodyNode->getParentJoint()->getJointType() == kinematics::Joint::J_HINGE )
		{    pBodyNode->getParentJoint()->getDof(0)->setValue( DEG2RAD(pos) ); }
		else if ( pBodyNode->getParentJoint()->getJointType() == kinematics::Joint::J_TRANS )
		{    pBodyNode->getParentJoint()->getDof(0)->setValue( pos ); 
                     /// Update the robot or object (both Skeletons)
                     pBodyNode->getSkel()->initSkel();
 		}
		sprintf(numBuf,"Joint Change: %7.4f", pos);

	} else{
		switch(slnum){
			case X_SLIDER:
				pObject->setPositionX( pos );
				sprintf(numBuf,"X Change: %7.4f", pos);
				break;
			case Y_SLIDER:
				pObject->setPositionY( pos );
				sprintf(numBuf,"Y Change: %7.4f", pos);
				break;
			case Z_SLIDER:
				pObject->setPositionZ( pos );
				sprintf(numBuf,"Z Change: %7.4f", pos);
				break;
			case ROLL_SLIDER:
			case PITCH_SLIDER:
			case YAW_SLIDER:
                                pObject->setRotationRPY( rollSlider->pos, pitchSlider->pos, yawSlider->pos );
				sprintf(numBuf,"Angle Change: %7.4f", pos);
				break;
			default:
				return;
		}

		if(selected == Return_Type_Robot) {
                    pObject->initSkel();
		}
        }

	if(frame!=NULL)	frame->SetStatusText(wxString(numBuf,wxConvUTF8));
	viewer->UpdateCamera();
}

/**
 * @function GRIPStateChange
 * @brief  This function is called when an object is selected in the Tree View
 * the sliders are shown / hidden and set according to the type of object
 * that is selected and its properties
 * @date 2011-10-13
 */
void InspectorTab::GRIPStateChange() {

    if(selectedTreeNode==NULL){
        itemName->SetLabel(wxString("Item: None",wxConvUTF8));
	parentName->Hide();
	jSlider->Hide();
	return;
    }

    planning::Object* pObject;
    kinematics::BodyNode* pBodyNode;
    planning::Robot* pRobot;

    double x; double y; double z; 
    double roll; double pitch; double yaw;

    string statusBuf;
    string buf,buf2;

    int selected = selectedTreeNode->dType;

    //-- Return type Object
    if(selected == Return_Type_Object){
        pObject = (planning::Object*)(selectedTreeNode->data);

	statusBuf = " Selected Object: " + pObject->getName();
	buf = "Object: " + pObject->getName();
	itemName->SetLabel(wxString(buf.c_str(),wxConvUTF8));
	parentName->Show();
	parentName->SetLabel(wxString("",wxConvUTF8));
	jSlider->Hide();

        //-- Get XYZ and RPY
        pObject->getRotationRPY( roll, pitch, yaw);
        pObject->getPositionX( x );
        pObject->getPositionY( y );
        pObject->getPositionZ( z );
    }

   //-- Return type Node
    else if(selected == Return_Type_Node){
        pBodyNode = (kinematics::BodyNode*)(selectedTreeNode->data);

	statusBuf = " Selected Node: " + string( pBodyNode->getName() ) + " of Robot: " + string( ((planning::Robot*) pBodyNode->getSkel())->getName() );
	buf = "Body Node: " + string( pBodyNode->getName() );
	itemName->SetLabel(wxString(buf.c_str(),wxConvUTF8));

	if( pBodyNode->getParentNode() != NULL ) {
	    buf2 = "Parent Link: " + string( pBodyNode->getParentNode()->getName() ) + "   Robot: " + string( ((planning::Robot*) pBodyNode->getSkel())->getName() );
        
	    jSlider->setRange( RAD2DEG(pBodyNode->getParentJoint()->getDof(0)->getMin() ),
                               RAD2DEG(pBodyNode->getParentJoint()->getDof(0)->getMax() ) );
	    if( pBodyNode->getParentJoint()->getJointType() == kinematics::Joint::J_HINGE ) {
	        jSlider->setValue(RAD2DEG(pBodyNode->getParentJoint()->getDof(0)->getValue() ) );
	    } else {
		jSlider->setValue(pBodyNode->getParentJoint()->getDof(0)->getValue());
	    }
	    jSlider->Show();
        } else {
	    buf2 = " (Root Link) ";
	    jSlider->Hide();
	}

       x =3; y = 3; z = 3; roll = 8; pitch = 8; yaw = 8;

    }
 
    //-- Return type Robot
    else if(selected == Return_Type_Robot) {

        pRobot = (planning::Robot*)(selectedTreeNode->data);

	statusBuf = " Selected Robot: " + pRobot->getName();
	buf = "Robot: " + pRobot->getName();
	itemName->SetLabel(wxString(buf.c_str(),wxConvUTF8));
	parentName->Show();
	parentName->SetLabel(wxString("",wxConvUTF8));
	jSlider->Hide();

        //-- Get XYZ and RPY
        pRobot->getRotationRPY( roll, pitch, yaw);
        pRobot->getPositionX( x );
        pRobot->getPositionY( y );
        pRobot->getPositionZ( z );
    }

    frame->SetStatusText(wxString(statusBuf.c_str(),wxConvUTF8));
    sizerFull->Layout(); 

    xSlider->setValue( x, false);
    ySlider->setValue( y, false);
    zSlider->setValue( z, false);

    rollSlider->setValue( RAD2DEG(roll), false );
    pitchSlider->setValue( RAD2DEG(pitch), false );
    yawSlider->setValue( RAD2DEG(yaw), false );

}