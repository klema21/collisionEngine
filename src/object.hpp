#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SDL2/SDL.h>

class Object{
	float x = rand()%800;
	float y = rand()%600;
	float vx = (rand()%20)-10;
	float vy = (rand()%20)-10;
	bool damaged = false;
	bool onRadar = false;
	SDL_Rect p = {x, y, 10, 10};
public:
	void Update(float deltaTime){
		x += vx * deltaTime;
		y += vy * deltaTime;
		p = {x, y, 10, 10};
	}
	float GetX() const {
		return x;
	}
	float GetY() const {
		return y;
	}
	void GetDamaged() {
		damaged = true;
	}
	void Detected(bool r) {
		onRadar = r;
	}
	SDL_Rect* GetSDLRect() {
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
