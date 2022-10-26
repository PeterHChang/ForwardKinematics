#include "Bone_Animation.h"
#include <glm/gtx/euler_angles.hpp>


Bone_Animation::Bone_Animation()
{
}


Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
    root_position = { 2.0f,1.0f,2.0f };

    scale_vector =
    {
        {1.0f,1.0f,1.0f},
        {0.5f,4.0f,0.5f},
        {0.5f,3.0f,0.5f},
        {0.5f,2.0f,0.5f}
    };

    rotation_degree_vector =
    {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f}
    };

    colors =
    {
        {0.7f,0.0f,0.0f,1.0f},
        {0.7f,0.7f,0.0f,1.0f},
        {0.7f,0.0f,0.7f,1.0f},
        {0.0f,0.7f,0.7f,1.0f}
    };

    bone_mat = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

    Rotation();
}

void Bone_Animation::update(float delta_time)
{

    pivot.clear();
    X_axis.clear();
    Y_axis.clear();
    Z_axis.clear();

    Rotation();

}


void Bone_Animation::Rotation()
{
    int i, j;

    std::vector<float> X_angle = { glm::radians(rotation_degree_vector[0][2]), glm::radians(rotation_degree_vector[1][2]), glm::radians(rotation_degree_vector[2][2]), glm::radians(rotation_degree_vector[3][2]) };

    std::vector<float> Y_angle = { glm::radians(rotation_degree_vector[0][0]), glm::radians(rotation_degree_vector[1][0]), glm::radians(rotation_degree_vector[2][0]), glm::radians(rotation_degree_vector[3][0]) };

    std::vector<float> Z_angle = { glm::radians(rotation_degree_vector[0][1]), glm::radians(rotation_degree_vector[1][1]), glm::radians(rotation_degree_vector[2][1]), glm::radians(rotation_degree_vector[3][1]) };

    rotateX = { glm::eulerAngleX(X_angle[0]),
                glm::eulerAngleX(X_angle[1]),
                glm::eulerAngleX(X_angle[2]),
                glm::eulerAngleX(X_angle[3]) };

    rotateY = { glm::eulerAngleX(Y_angle[0]),
                glm::eulerAngleY(Y_angle[1]),
                glm::eulerAngleY(Y_angle[2]),
                glm::eulerAngleY(Y_angle[3]) };

    rotateZ = { glm::eulerAngleX(Z_angle[0]),
                glm::eulerAngleZ(Z_angle[1]),
                glm::eulerAngleZ(Z_angle[2]),
                glm::eulerAngleZ(Z_angle[3]) };

    for (i = 0; i < 4; i++)
    {
        j = i - 1;
        rotate_mat[i] = rotateX[i] * rotateZ[i] * rotateY[i];

        if (i == 0)
        {
            translate_mat_origin[i] = glm::translate(glm::mat4(1.0f), { 0, scale_vector[i][1] / 2.0f, 0 });
            translate_mat_link[i] = glm::translate(glm::mat4(1.0f), { root_position[0], (root_position[1] - scale_vector[i][1] / 2.0f),root_position[2] });
            translate_mat_end[i] = glm::translate(glm::mat4(1.0f), { 0, scale_vector[i][1], 0 });
            world_mat[i] = translate_mat_link[i] * rotate_mat[i];
            bone_mat[i] = glm::translate(glm::mat4(1.0f), root_position);
        }
        else
        {
            translate_mat_origin[i] = glm::translate(glm::mat4(1.0f), { 0, scale_vector[i][1] / 2.0f, 0 });
            translate_mat_link[i] = glm::translate(glm::mat4(1.0f), { 0, scale_vector[i - 1][1], 0 });
            translate_mat_end[i] = glm::translate(glm::mat4(1.0f), { 0, scale_vector[i][1], 0 });
            world_mat[i] = world_mat[i - 1] * translate_mat_link[i] * rotate_mat[i];
            bone_mat[i] = world_mat[i] * translate_mat_origin[i];

            pivot.push_back(glm::vec3(world_mat[j] * translate_mat_end[j] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
            X_axis.push_back(normalize(glm::vec3(world_mat[j] * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f))));
            Y_axis.push_back(normalize(glm::vec3(world_mat[j] * rotateX[i] * rotateZ[i] * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))));
            Z_axis.push_back(normalize(glm::vec3(world_mat[j] * rotateX[i] * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))));

        }

    }
}


void Bone_Animation::reset()
{

    rotation_degree_vector =
    {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f}
    };

    bone_mat = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
}

