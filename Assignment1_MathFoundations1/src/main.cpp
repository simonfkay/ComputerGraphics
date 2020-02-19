// Includes for the assignment
#include "Vector4f.h"
#include "Matrix4f.h"
#include <iostream>

// Tests for comparing our library
// You may compare your operations against the glm library
// which is a well tested library.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

bool unitMat0() {
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity(1.0f,  0,     0,     0,
        			    0,     1.0f,  0,     0,
		        	    0,     0,     1.0f,  0,
			            0,     0,     0,     1.0f);

    if (
        glmIdentityMatrix[0][0]==myIdentity[0][0] &&
        glmIdentityMatrix[0][1]==myIdentity[0][1] &&
        glmIdentityMatrix[0][2]==myIdentity[0][2] &&
        glmIdentityMatrix[0][3]==myIdentity[0][3] &&
        glmIdentityMatrix[1][0]==myIdentity[1][0] &&
        glmIdentityMatrix[1][1]==myIdentity[1][1] &&
        glmIdentityMatrix[1][2]==myIdentity[1][2] &&
        glmIdentityMatrix[1][3]==myIdentity[1][3] &&
        glmIdentityMatrix[2][0]==myIdentity[2][0] &&
        glmIdentityMatrix[2][1]==myIdentity[2][1] &&
        glmIdentityMatrix[2][2]==myIdentity[2][2] &&
        glmIdentityMatrix[2][3]==myIdentity[2][3] &&
        glmIdentityMatrix[3][0]==myIdentity[3][0] &&
        glmIdentityMatrix[3][1]==myIdentity[3][1] &&
        glmIdentityMatrix[3][2]==myIdentity[3][2] &&
        glmIdentityMatrix[3][3]==myIdentity[3][3]) {
            return true;
    }
	
    return false;	
}

bool unitMat1() {
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity(1.0f,  0,     0,     0,
        			    0,     1.0f,  0,     0,
		        	    0,     0,     1.0f,  0,
			            0,     0,     0,     1.0f);

    if (
        glmIdentityMatrix[0][0]==myIdentity(0,0) &&
        glmIdentityMatrix[0][1]==myIdentity(0,1) &&
        glmIdentityMatrix[0][2]==myIdentity(0,2) &&
        glmIdentityMatrix[0][3]==myIdentity(0,3) &&
        glmIdentityMatrix[1][0]==myIdentity(1,0) &&
        glmIdentityMatrix[1][1]==myIdentity(1,1) &&
        glmIdentityMatrix[1][2]==myIdentity(1,2) &&
        glmIdentityMatrix[1][3]==myIdentity(1,3) &&
        glmIdentityMatrix[2][0]==myIdentity(2,0) &&
        glmIdentityMatrix[2][1]==myIdentity(2,1) &&
        glmIdentityMatrix[2][2]==myIdentity(2,2) &&
        glmIdentityMatrix[2][3]==myIdentity(2,3) &&
        glmIdentityMatrix[3][0]==myIdentity(3,0) &&
        glmIdentityMatrix[3][1]==myIdentity(3,1) &&
        glmIdentityMatrix[3][2]==myIdentity(3,2) &&
        glmIdentityMatrix[3][3]==myIdentity(3,3)) {
            return true;
    }
	
    return false;	
}

bool unitMat2() {
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Vector4f a(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4f b(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4f c(0.0f, 0.0f, 1.0f, 0.0f);
	Vector4f d(0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4f myIdentity(a,b,c,d);

    if (
        glmIdentityMatrix[0][0]==myIdentity[0][0] &&
        glmIdentityMatrix[0][1]==myIdentity[0][1] &&
        glmIdentityMatrix[0][2]==myIdentity[0][2] &&
        glmIdentityMatrix[0][3]==myIdentity[0][3] &&
        glmIdentityMatrix[1][0]==myIdentity[1][0] &&
        glmIdentityMatrix[1][1]==myIdentity[1][1] &&
        glmIdentityMatrix[1][2]==myIdentity[1][2] &&
        glmIdentityMatrix[1][3]==myIdentity[1][3] &&
        glmIdentityMatrix[2][0]==myIdentity[2][0] &&
        glmIdentityMatrix[2][1]==myIdentity[2][1] &&
        glmIdentityMatrix[2][2]==myIdentity[2][2] &&
        glmIdentityMatrix[2][3]==myIdentity[2][3] &&
        glmIdentityMatrix[3][0]==myIdentity[3][0] &&
        glmIdentityMatrix[3][1]==myIdentity[3][1] &&
        glmIdentityMatrix[3][2]==myIdentity[3][2] &&
        glmIdentityMatrix[3][3]==myIdentity[3][3]) {
            return true;
    }
	
    return false;	
}

bool unitMat3() {
	glm::mat4 glmScale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	Vector4f a(1.0f, 0, 0, 0);
	Vector4f b(0.0f, 1.0f, 0, 0);
	Vector4f c(0, 0, 1.0f, 0);
	Vector4f d(0, 0, 0, 1.0f);
	Matrix4f myScaled(a, b, c, d);
    myScaled = myScaled.MakeScale(2.0f, 2.0f, 2.0f);

    if (
        glmScale[0][0]==myScaled[0][0] &&
        glmScale[0][1]==myScaled[0][1] &&
        glmScale[0][2]==myScaled[0][2] &&
        glmScale[0][3]==myScaled[0][3] &&
        glmScale[1][0]==myScaled[1][0] &&
        glmScale[1][1]==myScaled[1][1] &&
        glmScale[1][2]==myScaled[1][2] &&
        glmScale[1][3]==myScaled[1][3] &&
        glmScale[2][0]==myScaled[2][0] &&
        glmScale[2][1]==myScaled[2][1] &&
        glmScale[2][2]==myScaled[2][2] &&
        glmScale[2][3]==myScaled[2][3] &&
        glmScale[3][0]==myScaled[3][0] &&
        glmScale[3][1]==myScaled[3][1] &&
        glmScale[3][2]==myScaled[3][2] &&
        glmScale[3][3]==myScaled[3][3]) {
            return true;
    }
	
    return false;	
}

bool unitMat4() {
	glm::mat4 glmTest = glm::mat4(1.0f);
    glmTest[1][3] = 72.0f;
    glmTest[2][3] = 2.1f;

	Matrix4f myMatrix(0, 0, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0);

    myMatrix[1][3] = 72.0f;
    myMatrix[2][3] = 2.1f;

    if ( glmTest[1][3]==myMatrix[1][3] &&
         glmTest[2][3]==myMatrix[2][3] ) {
            return true;
    }
	
    return false;	
}

bool unitMat5() {
    Vector4f vec(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 vecG(1.0f, 1.0f, 1.0f, 1.0f);
    float angle = 0.5f * glm::pi<float>();

    Matrix4f ident = Matrix4f();
    ident.identity();
    Matrix4f rotateX = ident.MakeRotationX(angle);
    Vector4f rotated = rotateX * vec;

    glm::vec4 rotatedG = glm::rotateX(vecG, angle);

    if (rotated.x == rotatedG.x &&
        rotated.y == rotatedG.y &&
        rotated.z == rotatedG.z &&
        rotated.w == rotatedG.w) {
        return true;
    }
    return false;
}

bool unitMat6() {
    Vector4f vec(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 vecG(1.0f, 1.0f, 1.0f, 1.0f);
    float angle = 0.5f * glm::pi<float>();

    Matrix4f ident = Matrix4f();
    ident.identity();
    Matrix4f rotateY = ident.MakeRotationY(angle);
    Vector4f rotated = rotateY * vec;

    glm::vec4 rotatedG = glm::rotateY(vecG, angle);

    if (rotated.x == rotatedG.x &&
        rotated.y == rotatedG.y &&
        rotated.z == rotatedG.z &&
        rotated.w == rotatedG.w) {
        return true;
    }
    return false;
}

bool unitMat7() {
    Vector4f vec(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 vecG(1.0f, 1.0f, 1.0f, 1.0f);
    float angle = 0.5f * glm::pi<float>();

    Matrix4f ident = Matrix4f();
    ident.identity();
    Matrix4f rotateZ = ident.MakeRotationZ(angle);
    Vector4f rotated = rotateZ * vec;

    glm::vec4 rotatedG = glm::rotateZ(vecG, angle);

    if (rotated.x == rotatedG.x &&
        rotated.y == rotatedG.y &&
        rotated.z == rotatedG.z &&
        rotated.w == rotatedG.w) {
        return true;
    }
    return false;
}

bool unitMat8() {
    Matrix4f mat(1.0f,  0.0f, 0.0f, 0.0f,
                 0.0f, -1.0f, 0.0f, 0.0f,
                 0.0f,  0.0f, 2.0f, 0.0f,
                 0.0f,  0.0f, 0.0f, 0.0f);
    Vector4f vec(2.0f, 1.5f, 3.0f, 5.5f);
    Vector4f transformed = mat * vec;

    if (transformed.x == 2.0f &&
        transformed.y == -1.5f &&
        transformed.z == 6.0f &&
        transformed.w == 0.0f) {
        return true;
    }
    return false;
}

bool unitMat9() {
    Matrix4f mat0(1.0f,  0.0f, 0.0f, 0.0f,
                  0.0f, -1.0f, 0.0f, 0.0f,
                  0.0f,  0.0f, 2.0f, 0.0f,
                  0.0f,  0.0f, 0.0f, 1.0f);
    Matrix4f mat1(5.0f,  4.0f, 0.0f, 0.0f,
                  0.0f,  4.0f, 0.0f, 1.0f,
                  0.0f,  0.0f, 2.0f, 0.0f,
                  0.0f,  0.0f, 3.0f, 4.0f);
    Matrix4f transform = mat0 * mat1;

    if (transform[0][0] ==  5.0f &&
        transform[0][1] ==  0.0f &&
        transform[0][2] ==  0.0f &&
        transform[0][3] ==  0.0f &&
        transform[1][0] ==  4.0f &&
        transform[1][1] == -4.0f &&
        transform[1][2] ==  0.0f &&
        transform[1][3] ==  0.0f &&
        transform[2][0] ==  0.0f &&
        transform[2][1] ==  0.0f &&
        transform[2][2] ==  4.0f &&
        transform[2][3] ==  3.0f &&
        transform[3][0] ==  0.0f &&
        transform[3][1] == -1.0f &&
        transform[3][2] ==  0.0f &&
        transform[3][3] ==  4.0f) {
        return true;
    }
    return false;
}

bool unitVec0() {
    Vector4f vec(1.0f, 1.5f, 2.0f, 2.5f);
    vec *= 2.0f;
    if (vec.x == 2.0f &&
        vec.y == 3.0f && 
        vec.z == 4.0f && 
        vec.w == 5.0f) {
        return true;
    }
    return false;
}

bool unitVec1() {
    Vector4f vec(2.0f, 4.0f, 6.0f, 8.0f);
    vec /= 2.0f;
    if (vec.x == 1.0f &&
        vec.y == 2.0f && 
        vec.z == 3.0f && 
        vec.w == 4.0f) {
        return true;
    }
    return false;
}

bool unitVec2() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    Vector4f vec1(2.0f, 4.0f, 6.0f, 8.0f);
    vec0 += vec1;
    if (vec0.x == 3.0f &&
        vec0.y == 5.5f && 
        vec0.z == 8.0f && 
        vec0.w == 10.5f) {
        return true;
    }
    return false;
}

bool unitVec3() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    Vector4f vec1(2.0f, 4.0f, 6.0f, 8.0f);
    vec1 -= vec0;
    if (vec1.x == 1.0f &&
        vec1.y == 2.5f && 
        vec1.z == 4.0f && 
        vec1.w == 5.5f) {
        return true;
    }
    return false;
}

bool unitVec4() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    Vector4f vec1(2.0f, 4.0f, 6.0f, 8.0f);
    if (Dot(vec0, vec1) == 40.0f) {
        return true;
    }
    return false;
}

bool unitVec5() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    float scalar = 2.0f;
    Vector4f vec1 = vec0 * scalar;
    vec0 *= scalar;

    if (vec0.x == vec1.x &&
        vec0.y == vec1.y && 
        vec0.z == vec1.z && 
        vec0.w == vec1.w) {
        return true;
    }
    return false;
}

bool unitVec6() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    float scalar = 2.0f;
    Vector4f vec1 = vec0 / scalar;
    vec0 /= scalar;

    if (vec0.x == vec1.x &&
        vec0.y == vec1.y && 
        vec0.z == vec1.z && 
        vec0.w == vec1.w) {
        return true;
    }
    return false;
}

bool unitVec7() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    Vector4f vec1 = -vec0;

    if (vec1.x == -1.0f &&
        vec1.y == -1.5f && 
        vec1.z == -2.0f && 
        vec1.w == -2.5f) {
        return true;
    }
    return false;
}

bool unitVec8() {
    Vector4f vec0(3.0f, 3.0f, 3.0f, 3.0f);
    if (Magnitude(vec0) == 6.0f) {
        return true;
    }
    return false;
}

bool unitVec9() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    Vector4f vec1(2.0f, 4.0f, 6.0f, 8.0f);
    Vector4f vec2 = vec0 + vec1;
    vec0 += vec1;
    if (vec0.x == vec2.x &&
        vec0.y == vec2.y && 
        vec0.z == vec2.z && 
        vec0.w == vec2.w) {
        return true;
    }
    return false;
}

bool unitVec10() {
    Vector4f vec0(1.0f, 1.5f, 2.0f, 2.5f);
    Vector4f vec1(2.0f, 4.0f, 6.0f, 8.0f);
    Vector4f vec2 = vec0 - vec1;
    vec0 -= vec1;
    if (vec0.x == vec2.x &&
        vec0.y == vec2.y && 
        vec0.z == vec2.z && 
        vec0.w == vec2.w) {
        return true;
    }
    return false;
}

bool unitVec11() {
    Vector4f vec0(2.0f, 4.0f, 4.0f, 8.0f);
    Vector4f vec1 = Normalize(vec0);
    if (vec1.x == 0.2f &&
        vec1.y == 0.4f && 
        vec1.z == 0.4f && 
        vec1.w == 0.8f) {
        return true;
    }
    return false;
}

bool unitVec12() {
    Vector4f vec0(1.0f, -2.0f, 2.0f, 0.0f);
    Vector4f vec1(4.0f, 0.0f, -3.0f, 0.0f);
    Vector4f vec2 = Project(vec0, vec1);
    if (vec2.x == -0.32f &&
        vec2.y == 0.0f && 
        vec2.z == 0.24f && 
        vec2.w == 0.0f) {
        return true;
    }
    return false;
}

bool unitVec13() {
    Vector4f vec0(3.0f, -3.0f, 1.0f, 1.0f);
    Vector4f vec1(4.0f, 9.0f, 2.0f, 1.0f);
    Vector4f vec2 = CrossProduct(vec0, vec1);
    if (vec2.x == -15.0f &&
        vec2.y == -2.0f && 
        vec2.z == 39.0f && 
        vec2.w == 1.0f) {
        return true;
    }
    return false;
}

int main() {
    // Run 'unit tests'
    std::cout << "Passed Mat 0: " << unitMat0() << " \n";
    std::cout << "Passed Mat 1: " << unitMat1() << " \n";
    std::cout << "Passed Mat 2: " << unitMat2() << " \n";
    std::cout << "Passed Mat 3: " << unitMat3() << " \n";
    std::cout << "Passed Mat 4: " << unitMat4() << " \n";
    std::cout << "Passed Mat 5: " << unitMat5() << " \n";
    std::cout << "Passed Mat 6: " << unitMat6() << " \n";
    std::cout << "Passed Mat 7: " << unitMat7() << " \n";
    std::cout << "Passed Mat 8: " << unitMat8() << " \n";
    std::cout << "Passed Mat 9: " << unitMat9() << " \n\n";

    std::cout << "Passed Vec 0: " << unitVec0() << " \n";
    std::cout << "Passed Vec 1: " << unitVec1() << " \n";
    std::cout << "Passed Vec 2: " << unitVec2() << " \n";
    std::cout << "Passed Vec 3: " << unitVec3() << " \n";
    std::cout << "Passed Vec 4: " << unitVec4() << " \n";
    std::cout << "Passed Vec 5: " << unitVec5() << " \n";
    std::cout << "Passed Vec 6: " << unitVec6() << " \n";
    std::cout << "Passed Vec 7: " << unitVec7() << " \n";
    std::cout << "Passed Vec 8: " << unitVec8() << " \n";
    std::cout << "Passed Vec 9: " << unitVec9() << " \n";
    std::cout << "Passed Vec 10: " << unitVec10() << " \n";
    std::cout << "Passed Vec 11: " << unitVec11() << " \n";
    std::cout << "Passed Vec 12: " << unitVec12() << " \n";
    std::cout << "Passed Vec 13: " << unitVec13() << " \n";

    return 0;
}
