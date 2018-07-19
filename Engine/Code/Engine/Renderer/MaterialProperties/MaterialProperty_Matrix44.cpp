#include "Engine/Renderer/MaterialProperties/MaterialProperty_Matrix44.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/GLFunctions.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Binds the uniform on the shader
//
void MaterialProperty_Matrix44::Bind(unsigned int programHandle)
{
	int bind = glGetUniformLocation( programHandle, m_name.c_str() );
	if(bind >= 0) 
	{
		glUniformMatrix4fv( bind, 1, m_isTranspose, m_data.data );
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a clone
//
MaterialProperty* MaterialProperty_Matrix44::Clone()
{
	return new MaterialProperty_Matrix44(*this);
}
