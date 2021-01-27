#include <VirtualPanic/virtual_panic.hpp>

VPanic::Engine engine;

int main() {
	engine.init("Hello libVirtualPanic!", VPanic::Vec(1100, 800));
	engine.execute();
}
