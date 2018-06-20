/*
================================================================================================================
----------------------------------------------------------------------------------------------------------------
Author - Mason J. Markle
----------------------------------------------------------------------------------------------------------------
Date Created - 06/06/2018
Date Modified - 06/06/2018
----------------------------------------------------------------------------------------------------------------
Purpose:
* Generate a Model-View-Projection (MVP) matrix for OpenGL object/camer interactions
----------------------------------------------------------------------------------------------------------------
Inputs:
  * (FOV) - Field of View
  * (WW) - window width
  * (WH) - window height
  * (Near) - Display Range Near
  * (Far) - Display range Far
  * (Cpos) - camera location vector
  * (Caim) - camera pointing vector
  * (Cup) - camera orientation vector (0,1,0 = up, 0,-1,0 = down)
  * (MM) - model matrix (model location)

Outpus - a MVP matrix variable

Usage Example:

----------------------------------------------------------------------------------------------------------------
Notes:

----------------------------------------------------------------------------------------------------------------
================================================================================================================
*/

#include "createMVPmat.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
using namespace glm;

mat4 MVP; // return variable

mat4 generate_MVP(float FOV, int WW, int WH, float Near, float Far, vec3 Cpos, vec3 Caim, vec3 Cup, float MM) {

  mat4 Projection = perspective(radians(FOV), (((float)WW)/((float)WH)), Near, Far);

  mat4 View = lookAt(
      Cpos,
      Caim,
      Cup
      );

  mat4 Model = mat4(MM);

  MVP = Projection * View * Model;

  return MVP;
}
