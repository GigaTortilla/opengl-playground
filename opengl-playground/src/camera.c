#include <glfw3.h>
#include <cglm/struct.h>
#include <gl_test.h>
#include <camera.h>

// fly style cam
void cam_updatePos(Camera* cam, GLFWwindow* window, float frameDelta) {
	cam->speed = 2.5f * frameDelta;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		cam->speed *= 1.5f;
	if (glfwGetKey(window, GLFW_KEY_W))
		cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(cam->front, cam->speed));
	if (glfwGetKey(window, GLFW_KEY_S))
		cam->pos = glms_vec3_sub(cam->pos, glms_vec3_scale(cam->front, cam->speed));
	if (glfwGetKey(window, GLFW_KEY_D))
		cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(glms_normalize(glms_cross(cam->front, cam->up)), cam->speed));
	if (glfwGetKey(window, GLFW_KEY_A))
		cam->pos = glms_vec3_sub(cam->pos, glms_vec3_scale(glms_normalize(glms_cross(cam->front, cam->up)), cam->speed));
}
void cam_updateMouse(Camera* cam) {
	float xRota = glm_rad(cam->pitch);
	float yRota = glm_rad(cam->yaw);
	vec3s direction = {
		.x = cos(yRota) * cos(xRota),
		.y = sin(xRota),
		.z = sin(yRota) * cos(xRota)
	};
	cam->front = glms_normalize(direction);
}

// fps style cam
void cam_updatePosFps(Camera* cam, GLFWwindow* window, float frameDelta) {
	cam->speed = 2.5f * frameDelta;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		cam->speed *= 1.5f;
	if (glfwGetKey(window, GLFW_KEY_W))
		cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale((vec3s){ cam->front.x, 0.0f, cam->front.z }, cam->speed));
	if (glfwGetKey(window, GLFW_KEY_S))
		cam->pos = glms_vec3_sub(cam->pos, glms_vec3_scale((vec3s){ cam->front.x, 0.0f, cam->front.z }, cam->speed));
	if (glfwGetKey(window, GLFW_KEY_D))
		cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(glms_normalize(glms_cross(cam->front, cam->up)), cam->speed));
	if (glfwGetKey(window, GLFW_KEY_A))
		cam->pos = glms_vec3_sub(cam->pos, glms_vec3_scale(glms_normalize(glms_cross(cam->front, cam->up)), cam->speed));
}