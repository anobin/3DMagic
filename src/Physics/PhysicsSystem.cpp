/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Implementation file for PhysicsSystem class
 *
 * @file PhyicsSystem.cpp
 * @author Andrew Keating
 */
 
#include <Physics/PhysicsSystem.h>


namespace Magic3D
{
    
    
void PhysicsSystem::init()
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
}
    
/// destructor
PhysicsSystem::~PhysicsSystem()
{
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;

    delete dynamicsWorld;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
};

















