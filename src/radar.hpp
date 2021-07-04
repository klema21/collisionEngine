#ifndef RADAR_HPP
#define RADAR_HPP

//include <SDL2/SDL.h>

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cmath>

#include "./object.hpp"
#include "./object3D.hpp"
#include "./convexPolygonObject.hpp"
#include "./config.hpp"

template<typename T>
class Radar {
private:
    int ticksLastFrame = 0;
	bool isRunning = false;
	int N = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
	std::vector<T*> objects;
public:
	Radar(int _N);
	Radar(const Radar&) = delete;
	Radar& operator=(const Radar&) = delete;
	Radar(Radar&&) = delete;
	Radar& operator=(const Radar&&) = delete;
	~Radar();
    bool IsRunning() const;
	template<typename A>
	float Distance(A* a, A* b);
	float Distance(Object3D* a, Object3D* b);
	bool HasIntersection(Object* a, Object* b);
	bool HasIntersection(Object3D* a, Object3D* b);
	bool HasIntersection(ConvexPolygonObject* a, ConvexPolygonObject* b);
	void RadarChecking();
	void CollisionChecking();
    void Initialize(int width, int height);
	void SetPixel(SDL_Renderer* r, int x, int y);
	void DrawCircle(SDL_Renderer* s, int n_cx, int n_cy, int r);
	void FillCircle(SDL_Renderer* s, int cx, int cy, int r);
    void ProcessInput();
    void Update();
    void Render();
};

template<typename T>
Radar<T>::Radar(int _N) :N(_N){}

template<typename T>
Radar<T>::~Radar(){
	for(auto o : objects){
		delete o;
	}
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

template<typename T>
bool Radar<T>::IsRunning() const {
    return this->isRunning;
}

template<typename T>
template<typename A>
float Radar<T>::Distance(A* a, A* b){
	return sqrt((b->GetX() - a->GetX()) * (b->GetX() - a->GetX())+
				(b->GetY() - a->GetY()) * (b->GetY() - a->GetY()));
}

template<typename T>
float Radar<T>::Distance(Object3D* a, Object3D* b){
	float x = pow(a->GetX() - b->GetX(), 2);
	float y = pow(a->GetY() - b->GetY(), 2);
	float z = pow(a->GetZ() - b->GetZ(), 2);
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

template<typename T>
bool Radar<T>::HasIntersection(Object* a, Object* b){
	if(a->GetX() < b->GetX() + 10 &&
	   a->GetX() + 10 > b->GetX() &&
	   a->GetY() < b->GetY() + 10 &&
	   a->GetY() + 10 > b->GetY())
			return true;
	return false;
}

template<typename T>
bool Radar<T>::HasIntersection(Object3D* a, Object3D* b){
	if(a->GetX() < b->GetX() + 10 &&
	   a->GetX() + 10 > b->GetX() &&
	   a->GetY() < b->GetY() + 10 &&
	   a->GetY() + 10 > b->GetY() &&
	   a->GetZ() < b->GetZ() + 10 &&
	   a->GetZ() + 10 > b->GetZ())
			return true;
	return false;
}

template<typename T>
bool Radar<T>::HasIntersection(ConvexPolygonObject* a, ConvexPolygonObject* b){
		ConvexPolygonObject* cpo1 = a;
		ConvexPolygonObject* cpo2 = b;
		struct vec2{
			float x;
			float y;
		};

		for (int shape = 0; shape < 2; ++shape)
		{
			if (shape == 1)
			{
				cpo1 = b;
				cpo2 = a;
			}
		
			for (int a = 0; a < cpo1->p.size(); ++a)
			{
				int b = (a + 1) % cpo1->p.size();
				vec2 axisProj = { -(cpo1->p[b].y - cpo1->p[a].y), cpo1->p[b].x - cpo1->p[a].x };
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// std::numeric_limits<float>::infinity();
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < cpo1->p.size(); ++p)
				{
					float q = (cpo1->p[p].x * axisProj.x + cpo1->p[p].y * axisProj.y);
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(max_r1, q);
				}

				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < cpo2->p.size(); ++p)
				{
					float q = (cpo2->p[p].x * axisProj.x + cpo2->p[p].y * axisProj.y);
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}
		return true;		
}

template<typename T>
void Radar<T>::RadarChecking(){
	int midW = config::WINDOW_WIDTH/2;
	int midH = config::WINDOW_HEIGHT/2;
	for(int i = 0; i<N; ++i){
		int arg1 = objects[i]->GetX() - midW;
		int arg2 = objects[i]->GetY() - midH;
		if((std::pow(arg1, 2) + (std::pow(arg2, 2))) < (std::pow(250, 2))){
			objects[i]->Detected(true);
		}else{
			objects[i]->Detected(false);
		}
	}
}

template<typename T>
void Radar<T>::CollisionChecking(){
	for(int i=0; i<N-1; ++i){
		for(int j=i+1; j<N; ++j){
			float d = Distance(objects[i], objects[j]);
			if(d < 20 && d > 0){
				if(HasIntersection(objects[i], objects[j])){
					objects[i]->GetDamaged();
					objects[j]->GetDamaged();
				}
			}
		}
	}
}

template<typename T>
void Radar<T>::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    isRunning = true;

	for(int i=0; i<N; ++i){
		T* o = new T;
		objects.push_back(o);
	}
    return;
}

template<typename T>
void Radar<T>::SetPixel(SDL_Renderer*r, int x, int y){
	SDL_SetRenderDrawColor(r, 255, 30, 25, 16);
	SDL_RenderDrawPoint(r, x, y);
}

template<typename T>
void Radar<T>::DrawCircle(SDL_Renderer* s, int n_cx, int n_cy, int r){
	double error = (double)-r;
	double x = (double)r - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;
	while(x >= y){
		SetPixel(s, (int)(cx + x), (int)(cy + y));
		SetPixel(s, (int)(cx + y), (int)(cy + x));

		if (x != 0){
			SetPixel(s, (int)(cx - x), (int)(cy + y));
			SetPixel(s, (int)(cx + y), (int)(cy - x));
		}

		if (y != 0){
			SetPixel(s, (int)(cx + x), (int)(cy - y));
			SetPixel(s, (int)(cx - y), (int)(cy + x));
		}

		if (x != 0 && y != 0){
			SetPixel(s, (int)(cx - x), (int)(cy - y));
			SetPixel(s, (int)(cx - y), (int)(cy - x));
		}

		error += y;
		++y;
		error += y;

		if (error >= 0){
			--x;
			error -= x;
			error -= x;
		}
	}

}

template<typename T>
void Radar<T>::FillCircle(SDL_Renderer* s, int cx, int cy, int r){
	for(double dy = 1; dy <= r; dy += 1.0){
		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_SetRenderDrawColor(s, 97, 97, 0, 67);
		SDL_RenderDrawLine(s, cx - dx, cy + dy - r, cx + dx, cy + dy - r);
		SDL_RenderDrawLine(s, cx - dx, cy - dy + r, cx + dx, cy - dy + r);
	}
}

template<typename T>
void Radar<T>::ProcessInput() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        }
        default: {
            break;
        }
    }
}

template<typename T>
void Radar<T>::Update() {
    // Waste some time / sleep until we reach the target frame time in milliseconds
    int timeToWait = config::FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    // Only sleep if we are too fast
    if (timeToWait > 0 && timeToWait <= config::FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    // Delta time is the difference in ticks from last frame converted to secomds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

	for(auto o : objects){
		o->Update(deltaTime);
	}
}

template<typename T>
void Radar<T>::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);
	
	DrawCircle(renderer, config::WINDOW_WIDTH/2, 
					config::WINDOW_HEIGHT/2, 250);
	FillCircle(renderer, config::WINDOW_WIDTH/2-0.2, 
					config::WINDOW_HEIGHT/2+0.2, 249);

	for(auto o : objects){
		o->Render(renderer);
	}
    SDL_RenderPresent(renderer);
}

#endif
