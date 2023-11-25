#ifndef LA
#define LA

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>  // Primary GLM library
#include <glm/gtc/matrix_transform.hpp>  // For glm::translate, glm::rotate, and glm::scale.
#include <glm/gtx/string_cast.hpp>  // For glm::to_string.
#include <glm/gtc/type_ptr.hpp>  // For glm::value_ptr.

#include <QMatrix4x4>
#include <QVector4D>

//Functions used to convert glm data types to Qt data types.
namespace la {
    QMatrix4x4 to_qmat(const glm::mat4 &m);
    QVector4D to_qvec(const glm::vec4 &v);
}


#endif // LA
