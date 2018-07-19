#include "Engine/Renderer/MaterialProperties/MaterialProperty_Vector3.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/GLFunctions.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Binds the uniform on the shader
//
void MaterialProperty_Vector3::Bind(unsigned int programHandle)
{
	int bind = glGetUniformLocation( programHandle, m_name.c_str() );
	if(bind >= 0) 
	{
		glUniform3fv(bind, 1, m_data.data);
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a clone
//
MaterialProperty* MaterialProperty_Vector3::Clone()
{
	return new MaterialProperty_Vector3(*this);
}
