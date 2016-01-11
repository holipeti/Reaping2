#ifndef INCLUDED_CORE_MAP_MATRIX_GRID_H
#define INCLUDED_CORE_MAP_MATRIX_GRID_H

#include "i_grid.h"
#include "core/scene.h"

namespace map {

class MatrixGrid:public IGrid
{
protected:
    glm::vec2 GetPositionOnMatrix(glm::vec2 position);
    typedef std::vector<glm::vec2> NeighborDirs_t;
    NeighborDirs_t mNeighborDirs;
public:
    MatrixGrid(int32_t Id);
    virtual void Update( double DeltaTime );
    virtual void SetMousePosition(double x, double y);

    void GetPositionOnMatrix();
    virtual Neighbors GetNeighbors(glm::vec2 position, int32_t actorID);
private:
    Scene& mScene;
};



} // namespace map
#endif//INCLUDED_CORE_MAP_MATRIX_GRID_H
