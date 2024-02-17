#include "precomp.hpp"
#include "models/app.hpp"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main() {
    /* int* p = NULL;
    *p = 0; */

    char* p = new char[10];

    auto app = std::make_shared<App>();
	app->setup();
    app->run();

	return 0;
}