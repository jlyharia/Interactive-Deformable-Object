#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>
using namespace std;
using glm::vec3;
using glm::vec4;

void vout (const vector< vector<float>> &vec);
void vout (const vector< vector<int>> &vec);
void vout (const vector<int> &vec);
void vout (const vector<GLuint> &vec);
void vout (const vector<float> &vec);
void vout (const vec3 &vec);
void vout (const vec4 &vec);
