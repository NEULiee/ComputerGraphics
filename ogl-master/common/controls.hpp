#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
float getRotation();
float getHeight();
float getySpeed();
bool getChute();
bool getLaunch();
bool getLanding();
bool getDirection();

#endif