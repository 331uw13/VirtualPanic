#include <VirtualPanic/virtual_panic.hpp>

VPanic::Engine engine;


void update() {
}


int main() {
	engine.set_update_callback(update);
	engine.init("Hello libVirtualPanic!", VPanic::Vec(1100, 800));
	engine.execute();
}
