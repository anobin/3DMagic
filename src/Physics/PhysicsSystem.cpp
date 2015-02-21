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
     broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    //dynamicsWorld->getSolverInfo().m_splitImpulse = true;
    //dynamicsWorld->getSolverInfo().m_splitImpulsePenetrationThreshold =0.0f;
    //dynamicsWorld->getSolverInfo().m_restitution = 0.8f;
    //dynamicsWorld->getSolverInfo().m_damping = 1.5f;
    //dynamicsWorld->getSolverInfo().m_tau = 0;
    //dynamicsWorld->getSolverInfo().m_friction = 0.01f;
    //dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD;
    //dynamicsWorld->getSolverInfo().m_linearSlop = -0.1f;
    //dynamicsWorld->getSolverInfo().m_sor = 0;
    
        
}
    
/// destructor
void PhysicsSystem::deinit()
{
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;

    delete dynamicsWorld;
}
    

btVector3 createBtVector(const Vector3& inputPoint)
{
	return btVector3(inputPoint.x(), inputPoint.y(), inputPoint.z());
}

Vector3 createPoint(const btVector3& inputVector)
{
	return Vector3(inputVector.getX(), inputVector.getY(), inputVector.getZ());
}

// TODO: return a complex ray object, not just a hitpoint
Vector3 PhysicsSystem::createRay(const Vector3& startPoint, const Vector3& direction, Scalar maxLength) const
{
	btVector3 start = createBtVector(startPoint);
	btVector3 end = createBtVector(startPoint.translate(direction, maxLength));

	btCollisionWorld::ClosestRayResultCallback result(start, end); 
	this->dynamicsWorld->rayTest(start, end, result);

	if (!result.hasHit())
		return startPoint.translate(direction, maxLength);

	return createPoint(result.m_hitPointWorld);
}
    
    
    
    
    
    
    
    
    
    
    
};

















