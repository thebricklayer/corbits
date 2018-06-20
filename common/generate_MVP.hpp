// Preamble:
#ifndef CREATEMVPMAT_H
#define CREATEMVPMAT_H

#include <glm/glm.hpp>

// function definition:
glm::mat4 generate_MVP(float Field_of_view, int window_width, int window_height, float display_near,
                    float display_far, glm::vec3 camera_location, glm::vec3 camera_direction, glm::vec3 camera_orientation,
                    float model_matrix_format);

#endif
