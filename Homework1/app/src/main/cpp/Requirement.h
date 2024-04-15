//
// Created by Media on 4/4/2024.
//

#ifndef HOMEWORK1_SOL_REQUIREMENT_H
#define HOMEWORK1_SOL_REQUIREMENT_H
float startingPoints[] = {10.0f, 0.0f, 0.0f};
float endingPoints[] = {20.0f, 10.0f, 0.0f};
float scaleArbitrary = 2.0f;

float corner = 0.2f;
GLsizei lineVerticesSize = 64;
GLsizei lineIndicesSize = 36;
GLfloat lineVertices[] = {    startingPoints[0] + corner,  startingPoints[1], startingPoints[2] - corner, 0.3366f, -0.8125f, -0.4760f,  2.0000f,  2.0000f,
                              startingPoints[0] - corner,  startingPoints[1],  startingPoints[2] - corner, -0.56f, -0.2326f, -0.7941f,  1.7500f,  2.0000f,
                              startingPoints[0] - corner,  startingPoints[1], startingPoints[2] + corner, -0.56f, -0.2326f, 0.7941f,  2.0000f,  2.0000f,
                              startingPoints[0] + corner,  startingPoints[1],  startingPoints[2] + corner, 0.3366f, -0.8125f, 0.4760f,  1.7500f,  2.0000f,
                              endingPoints[0] + corner,  endingPoints[1], endingPoints[2] - corner, 0.56f, 0.2326f, -0.7941f,   2.0000f,  2.0000f,
                              endingPoints[0] - corner,  endingPoints[1],  endingPoints[2] - corner, -0.3366f, 0.8125f, -0.4760f,  1.7500f,  2.0000f,
                              endingPoints[0] - corner,  endingPoints[1], endingPoints[2] + corner, -0.3366f, 0.8125f, 0.4760f,   2.0000f,  2.0000f,
                              endingPoints[0] + corner,  endingPoints[1],  endingPoints[2] + corner, 0.56f, 0.2326f, 0.7941f,  1.7500f,  2.0000f};
GLushort lineIndices[] = {1, 0, 2, 2, 0, 3, 3, 0, 4, 4, 7, 3 ,3 ,7 ,6, 6, 2, 3, 6, 5, 2, 2, 5, 1, 4, 5, 6, 6, 7, 4, 4, 0, 1, 1, 5, 0};
#endif //HOMEWORK1_SOL_REQUIREMENT_H
