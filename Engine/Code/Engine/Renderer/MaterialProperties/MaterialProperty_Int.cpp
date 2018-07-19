#include "Engine/Renderer/MaterialProperties/MaterialProperty_Int.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/GLFunctions.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Binds the uniform on the shader
//
void MaterialProperty_Int::Bind(unsigned int programHandle)
{
	int bind = glGetUniformLocation( programHandle, m_name.c_str() );
	if(bind >= 0) 
	{
		glUniform1iv(bind, 1, &m_data);
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a clone 
//
MaterialProperty* MaterialProperty_Int::Clone()
{
	return new MaterialProperty_Int(*this);
}
