// Preamble:
#ifndef CAM_CONTROLS_H
#define CAM_CONTROLS_H

#include <glm/glm.hpp>

// function definition:
// glm::mat4 cam_control_MVP(float Field_of_view, int window_width, int window_height, float display_near,
//                     float display_far, glm::vec3 camera_location, glm::vec3 camera_direction, glm::vec3 camera_orientation, float model_matrix_format);
void cam_controls();

#endif
