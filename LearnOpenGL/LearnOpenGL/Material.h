#pragma once

#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambientStrength;
	glm::vec3 diffuseStrength;
	glm::vec3 specularStrength;
	glm::vec3 shininess;
};