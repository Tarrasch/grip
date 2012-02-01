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
#include "GUI.h"
//--------------------------------------------------------------------
//   GLOBAL VARIABLES: Keep track of the user's current
//   loaded files, robot, planners, and other intentions w.r.t. the GUI
//---------------------------------------------------------------------

planning::World* mWorld = 0;
Collision* mCollision=0;
database d;
input i;
std::vector<int> keyCurrent;
std::vector<int> patternCurrent;
std::vector<int> dimensionsCurrent;
float costCurrent;
bool keyChanged = 0;
float keyScale = 4.0f;
float c_red[10] = 	{0.5, 0.5, 0.0, 0.0, 0.0, 1.0};
float c_green[10] = {0.0, 0.0, 0.0, 0.5, 1.0, 1.0};
float c_blue[10] = 	{0.0, 0.5, 0.5, 0.0, 1.0, 0.0};

GRIPFrame*	frame = 0;
Viewer*		viewer = 0;
TreeView*	treeView = 0;
wxNotebook*	tabView = 0;

TreeViewReturn* selectedTreeNode = 0;

bool reverseLinkOrder = false;
bool check_for_collisions = false;
int stateChangeType = 0;

// Camera position
Eigen::Matrix3d mCamRotT;
Eigen::Vector3d mWorldV = Eigen::Vector3d(1, 0, 0);
double mCamRadius = 0;
// World colors
Eigen::Vector3d mGridColor = Eigen::Vector3d(1, 0, 0);
Eigen::Vector3d mBackColor = Eigen::Vector3d(1, 0, 0);



DEFINE_EVENT_TYPE(wxEVT_GRIP_STATE_CHANGE)

