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
/** Header file for PhysicsSystem class
 *
 * @file PhyicsSystem.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_PHYSICS_SYSTEM_H
#define MAGIC3D_PHYSICS_SYSTEM_H

// include Bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "PhysicalBody.h"

namespace Magic3D
{

/** Represents master interface to physics system
 */
class PhysicsSystem
{
private:
    btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
    
	btDiscreteDynamicsWorld* dynamicsWorld;

public:
    /// standard constructor
    inline PhysicsSystem(): broadphase(NULL), collisionConfiguration(NULL),
        dispatcher(NULL), solver(NULL), dynamicsWorld( NULL ) {}
    
    /// destructor
    inline ~PhysicsSystem()
    {   
    }

    void init();
    
    void deinit();
    
    inline void setGravity( float x, float y, float z )
    {
        dynamicsWorld->setGravity(btVector3(x,y,z));
    }
    
    inline void addBody( PhysicalBody& body )
    {
        dynamicsWorld->addRigidBody(body.getRigidBody());
    }
    
    inline void removeBody( PhysicalBody& body )
    {
        if (body.getRigidBody())
            dynamicsWorld->removeRigidBody(body.getRigidBody());
    }

    inline void stepSimulation( float secs, int substeps )
    {
        dynamicsWorld->stepSimulation(secs,substeps);
    }

};


};





#endif









