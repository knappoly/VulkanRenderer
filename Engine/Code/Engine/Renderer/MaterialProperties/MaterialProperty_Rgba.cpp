#include "Engine/Renderer/MaterialProperties/MaterialProperty_Rgba.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/GLFunctions.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Binds the uniform on the shader
//
void MaterialProperty_Rgba::Bind(unsigned int programHandle)
{
	int bind = glGetUniformLocation( programHandle, m_name.c_str() );
	if(bind >= 0) 
	{
		glUniform4fv(bind, 1, m_data.GetAsFloats());
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a clone
//
MaterialProperty* MaterialProperty_Rgba::Clone()
{
	return new MaterialProperty_Rgba(*this);
}
