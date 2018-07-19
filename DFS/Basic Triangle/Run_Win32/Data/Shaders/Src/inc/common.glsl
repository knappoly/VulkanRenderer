// Frame information ;
layout(binding=1, std140) uniform cFrameBlock 
{
   float GAME_TIME;
   float GAME_DELTA_TIME;
   float SYSTEM_TIME; 
   float SYSTEM_DELTA_TIME; 
}; 

// Camera Constants
layout(binding=2, std140) uniform cCameraBlock 
{
   mat4 VIEW;
   mat4 PROJECTION; 
   mat4 VIEW_PROJECTION; 

   mat4 INV_VIEW;          // also just called the camera matrix
   mat4 INV_PROJECTION; 
   mat4 INT_VIEW_PROJECTION; 

   // dupliate of INV_VIEW, but just convenient
   vec3 CAMERA_RIGHT;      float pad0;
   vec3 CAMERA_UP;         float pad1; 
   vec3 CAMERA_FORWARD;    float pad2; 
   vec3 CAMERA_POSITION;   float pad3; 
}; 

// per model data
layout(binding=3, std140) uniform cModelBlock 
{
   mat4 MODEL; 
}; 
