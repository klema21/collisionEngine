#include "./radar.hpp"

int main(int argc, char *args[]) {
	/*
		Object, Object3D, ConvexPolygonObject
	*/
	Radar<ConvexPolygonObject> radar(100);

    radar.Initialize(config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
	
    while (radar.IsRunning()) {
        radar.ProcessInput();
		radar.RadarChecking();
		radar.CollisionChecking();
        radar.Update();
        radar.Render();
    }

    return 0;
}
