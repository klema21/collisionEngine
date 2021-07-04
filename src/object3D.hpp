#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include <SDL2/SDL.h>

struct vec3 {
	float x;
	float y;
	float z;
};

class Object3D {
public:
	vec3 pos = {rand()%800, rand()%600, rand()%10};
	float vx = (rand()%20)-10;
	float vy = (rand()%20)-10;
	float vz = (rand()%20)-10;
	bool damaged = false;
	bool onRadar = false;
	SDL_Rect p = {pos.x, pos.y, 10, 10};
public:
	void Update(float deltaTime){
		pos.x += vx * deltaTime;
		pos.y += vy * deltaTime;
		pos.z += vz * deltaTime;
		p = {pos.x, pos.y, 10, 10};
	}
	float GetX() const {
		return pos.x;
	}
	float GetY() const {
		return pos.y;
	}
	float GetZ() const {
		return pos.z;
	}
	void GetDamaged(){
		damaged = true;
	}
	void Detected(bool r){
		onRadar = r;
	}
	SDL_Rect* GetSDLRect(){
		return &p;
	}
	void Render(SDL_Renderer* renderer) const {
		if(damaged)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		else{
			if(onRadar)
				SDL_SetRenderDrawColor(renderer, 88, 88, 123, 13);
			else
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderFillRect(renderer, &p);
	}
	
};

#endif
