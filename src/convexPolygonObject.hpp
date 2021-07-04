#ifndef CONVEXPOLYGONOBJECT_HPP
#define CONVEXPOLYGONOBJECT_HPP

#include <SDL2/SDL.h>
#include <vector>

class ConvexPolygonObject{
public:
	struct vec2{
		float x;
		float y;
	};

	std::vector<vec2> p;
	std::vector<vec2> o;
	vec2 pos = {rand()%800, rand()%600};
	float vx = (rand()%20)-10;
	float vy = (rand()%20)-10;
	float angle;
	bool damaged = false;
	bool onRadar = false;
public:
	ConvexPolygonObject(){
		static int cnt = 0;
		if(cnt % 3 == 0){
			CreatePentagon();
		} else if(cnt % 3 == 1){
			CreateTriangle();
		} else if(cnt % 3 == 2){
			CreateQuad();
		}
		++cnt;
	}
	void Update(float deltaTime){
		pos.x += vx * deltaTime;
		pos.y += vy * deltaTime;
	}
	void CreatePentagon(){
		float fTheta = 3.14159f * 2.0f / 5.0f;
		angle = 0.0f;
		for (int i = 0; i < 5; ++i)
		{
			o.push_back({ 30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i) });
			p.push_back({ 30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i) });
		}
	}

	void CreateTriangle(){
		float fTheta = 3.14159f * 2.0f / 3.0f;
		angle = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			o.push_back({ 20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i) });
			p.push_back({ 20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i) });
		}
	}

	void CreateQuad(){
		angle = 0.0f;
		p.push_back({ -30, -30 });
		p.push_back({ -30, +30 });
		p.push_back({ +30, +30 });
		p.push_back({ +30, -30 });
		p.resize(4);
	}
	void Render(SDL_Renderer* renderer) const {
		if(damaged)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		else
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for(int i=0; i<p.size(); ++i){
			SDL_RenderDrawLine(renderer, p[i].x+pos.x, p[i].y+pos.y, 
							p[(i + 1) % p.size()].x+pos.x, 
							p[(i + 1) % p.size()].y+pos.y);
		}
	}
	float GetX() const {
		return pos.x;
	}
	float GetY() const {
		return pos.y;
	}
	void GetDamaged() {
		damaged = true;
	}
	int GetPLength() const {
		return p.size();
	}
	void Detected(bool r) {
		onRadar = r;
	}
};

#endif
