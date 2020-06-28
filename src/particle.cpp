#include "particle.hpp"
#include <iostream>
void particleGen::init(int particlesDensity) {
  this->particlesPerBlock = particlesDensity;
  this->particleShader.compile(this->vbShaderSource, this->fbShaderSource);
  this->tileTexture.init("blockParticle.jpg");  
  particle newParticle;
}

void particleGen::draw(spriteRender &gameRender) {
  std::cout << this->particles.size() << std::endl;
  for (int i = 0; i < this->particles.size(); i++) {
    tile &xtile = this->particles[i].particleSprite;
    if(this->particles[i].lifespan < 1)
    {
      this->particles.erase(particles.begin()+i);
      continue;
    } else {
      this->particles[i].lifespan--;
    }
    if (xtile.tIsRendered != 0) {
      xtile.Draw(gameRender);
    }
  }
}

void particleGen::createParticles(float yPosition, float xPosition, int color){ 

    float randValx = rand() % 3+1;
    float randValy = -1 * (rand() % 5) - 5;
    float randLifespan = rand() % 15;
    randLifespan+= 25;
    float randRotation = rand() % 10;
    int randValSign = rand();
    if(randValSign % 2 == 1){
      randValx = randValx * -1;
      randRotation = randRotation * -1;
    }
    particle newParticle;
    newParticle.rotation = randRotation;
    std::cout << color << std::endl;
    newParticle.particleSprite = tile(tileTexture,glm::vec2((float)xPosition / 20 + this->board_offset_x,
                     (float)yPosition / 20 + this->board_offset_y),this->tileSize, 0, color);
  
    newParticle.velocity = glm::vec2(randValx/1000,randValy/1000);
    newParticle.lifespan = randLifespan;
    this->particles.push_back(newParticle);

}

void particleGen::updateParticles(){
  for (int i = 0; i < this->particles.size(); i++) {
      tile &xtile = this->particles[i].particleSprite;
      xtile.tPosition = glm::vec2(xtile.tPosition.x + particles[i].velocity.x,
                         xtile.tPosition.y  + particles[i].velocity.y);
      xtile.tRotation += this->particles[i].rotation;
  }
}