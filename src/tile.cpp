#include "tile.hpp"

tile::tile()
    : tPosition(glm::vec2(0.0f, 0.0f)), tSize(glm::vec2(1.0f, 1.0f)),
      tRotation(0.0f), tIsRendered(false) {
}

tile::tile(texture spriteTexture,glm::vec2 pos, glm::vec2 size, float rotation, int renderType)
  :tTexture(spriteTexture), tPosition(pos), tSize(size), tRotation(rotation),
      tIsRendered(renderType) {}

void tile::Draw(spriteRender &renderer) {
  renderer.DrawSprite(this->tTexture, this->tPosition, this->tRotation, this->tSize, pieceTypeToColor(this->tIsRendered));
}

glm::vec3 tile::pieceTypeToColor(int pieceType)
{
  switch(pieceType) {
  case 0:
    //Background color
    return glm::vec3(0.86f,1.0f,1.0f);
  case 1:
    return this->green;
  case 2:
    return this->red;
  case 3:
    return this->blue;
  case 4:
    return this->orange;
  case 5:
    return this->purple;
  case 6:
    return this->yellow;
  case 7:
    return this->tealBlue;
  case 8:
    //edge
    return glm::vec3(0.03f,0.03f,0.03f);
  case 10:
    //shadowTetronimo
    return glm::vec3(.8f,.8f,.8f);
  case 11:
    return glm::vec3(.4f,.4f,.4f);
  default:
    return glm::vec3(0.0f,0.0f,0.0f);
  }
}
