#include "Engine/Renderer/MaterialProperties/MaterialProperty_Float.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/GLFunctions.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Binds the float uniform in the shader
//
void MaterialProperty_Float::Bind(unsigned int programHandle)
{
	int bind = glGetUniformLocation( programHandle, m_name.c_str() );
	if(bind >= 0)
	{
		glUniform1fv(bind, 1, &m_data);
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a clone of the material property
//
MaterialProperty* MaterialProperty_Float::Clone()
{
	return new MaterialProperty_Float(*this);
}
