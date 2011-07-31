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
/** Contains MAGIC_THROW tests
 */

// include google test framework
#include <gtest/gtest.h>
#include <Exceptions/MagicException.h>
#include <Exceptions/ShaderCompileException.h>
#include <Util/magic_throw.h>


/** Fixture for MAGIC_THROW tests
 */
class Util_MAGIC_THROW_Tests : public ::testing::Test
{
protected:
    
    /// setup method
    virtual void SetUp()
    {
        // no setup
    }
    
    /// teardown method
    virtual void TearDown()
    {
        // no teardown
    }
    
    
};


// test to run depends on whether magic throws are enabled
#ifndef MAGIC3D_DISABLE_MAGIC_THROWS

/// tests that MAGIC_THROW will properly throw when enabled 
TEST_F(Util_MAGIC_THROW_Tests, MagicThrowEnabledTest)
{
    // An uneeded level of indirection so that the compiler does not complain 
    // about the obvious fact that a ternary operator with an always true
    // condition is pointless. No it ain't, supid compiler!
    int one = 1;
    int* onep = &one;
    
    // string message for exception
    const char* msg = "Throw!";
    
    // when enabled, MAGIC_THROW should throw a MagicException if the
    // condition is TRUE
    ASSERT_THROW( MAGIC_THROW( (*onep) == 1, msg ), Magic3D::_MagicException );
    
    // when enabled, MAGIC_THROW_EX should throw the exception given if the
    // condition is TRUE
    ASSERT_THROW( 
        MAGIC_THROW_EX( (*onep) == 1, ShaderCompileException, msg ), 
            Magic3D::_ShaderCompileException 
        );
    
    // when enabled, MAGIC_THROW should not throw a MagicException if the
    // condition is FALSE
    ASSERT_NO_THROW( MAGIC_THROW( (*onep) == 2, msg ) );
    
    // when enabled, MAGIC_THROW_EX should not throw the exception given if the
    // condition is FALSE
    ASSERT_NO_THROW( MAGIC_THROW_EX( (*onep) == 2, ShaderCompileException, msg ) );
    
}


// test to run depends on whether magic throws are enabled
#else


/// tests that MAGIC_THROW will not throw when disabled 
TEST_F(Util_MAGIC_THROW_Tests, MagicThrowDisabledTest)
{
    // An uneeded level of indirection so that the compiler does not complain 
    // about the obvious fact that a ternary operator with an always true
    // condition is pointless. No it ain't, supid compiler!
    int one = 1;
    int* onep = &one;
    
    // string message for exception
    const char* msg = "Throw!";
    
    // when disabled, MAGIC_THROW should not throw a MagicException if the
    // condition is TRUE
    ASSERT_NO_THROW( MAGIC_THROW( (*onep) == 1, msg ) );
    
    // when disabled, MAGIC_THROW_EX should not throw the exception given if the
    // condition is TRUE
    ASSERT_NO_THROW( MAGIC_THROW_EX( (*onep) == 1, ShaderCompileException, msg ) );
    
    // when disabled, MAGIC_THROW should not throw a MagicException if the
    // condition is FALSE
    ASSERT_NO_THROW( MAGIC_THROW( (*onep) == 2, msg ) );
    
    // when disabled, MAGIC_THROW_EX should not throw the exception given if the
    // condition is FALSE
    ASSERT_NO_THROW( MAGIC_THROW_EX( (*onep) == 2, ShaderCompileException, msg ) );
    
}



#endif





























