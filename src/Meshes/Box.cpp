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
/** Implementation file for built-in Box batch
 *
 * @file Box.cpp
 * @author Andrew Keating
 */


#include <Graphics/MeshBuilder.h>
#include <Shapes/Vertex.h>

namespace Magic3D
{
	
/** Build a box batch
 * @param width the width of the box
 * @param height the height of the box
 * @param depth the depth of the box
 */
MeshBuilderPTNT& MeshBuilderPTNT::buildBox(float width, float height, float depth)
{
	// we always work with half lengths
	width = width/2;
	height = height/2;
	depth = depth/2;

    this->vertices.reserve(36);

	// 6 points per side, 6 sides
		
	// top, 6 points
    this->addVertex(
        PositionAttr(-width, height, -depth),
        TexCoordAttr(0.0f, 0.0f)
    ); // top left
    this->addVertex(
        PositionAttr(-width, height, depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom left*/
    this->addVertex(
        PositionAttr(width, height, -depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top right
    this->addVertex(
        PositionAttr(-width, height, depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(width, height, depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom right
	this->addVertex(
        PositionAttr(width, height, -depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top right
    
    // bottom, 6 points
    this->addVertex(
        PositionAttr(-width, -height, -depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top left
	this->addVertex(
        PositionAttr(width, -height, -depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top right
    this->addVertex(
        PositionAttr(-width, -height, depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(-width, -height, depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom left
	this->addVertex(
        PositionAttr(width, -height, -depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top right
    this->addVertex(
        PositionAttr(width, -height, depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom right

    // left side, 6 points
    this->addVertex(
        PositionAttr(-width, height, -depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top left
    this->addVertex(
        PositionAttr(-width, -height, -depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(-width, height, depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top right
    this->addVertex(
        PositionAttr(-width, -height, -depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(-width, -height, depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom right
	this->addVertex(
        PositionAttr(-width, height, depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top right
    
    // right side, 6 points
    this->addVertex(
        PositionAttr(width, height, -depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top left
	this->addVertex(
        PositionAttr(width, height, depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top right
    this->addVertex(
        PositionAttr(width, -height, -depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(width, -height, -depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom left
	this->addVertex(
        PositionAttr(width, height, depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top right
	this->addVertex(
        PositionAttr(width, -height, depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom right
	
	// front, 6 points
    this->addVertex(
        PositionAttr(-width, height, -depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top left
    this->addVertex(
        PositionAttr(width, height, -depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top right
	this->addVertex(
        PositionAttr(-width, -height, -depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(-width, -height, -depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom left
	this->addVertex(
        PositionAttr(width, height, -depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top right
	this->addVertex(
        PositionAttr(width, -height, -depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom right
	
    // back, 6 points
    this->addVertex(
        PositionAttr(-width, height, depth),
        TexCoordAttr(0.0f, 0.0f)
    ); //  top left
    this->addVertex(
        PositionAttr(-width, -height, depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom left
    this->addVertex(
        PositionAttr(width, height, depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top right
    this->addVertex(
        PositionAttr(-width, -height, depth),
        TexCoordAttr(0.0f, 1.0f)
    ); //  bottom left
	this->addVertex(
        PositionAttr(width, -height, depth),
        TexCoordAttr(1.0f, 1.0f)
    ); //  bottom right
    this->addVertex(
        PositionAttr(width, height, depth),
        TexCoordAttr(1.0f, 0.0f)
    ); //  top right
	
    this->calculateNormals();
    this->calculateTangents();

    return *this;
}
	
	
	
	
	
	
	
	
};

















































