#include "hotreload-shader.hpp"
#include "raylib.h"

bool HotReloadShader::load() {
	auto shader = std::make_unique<raylib::Shader>(nullptr, path.c_str());
	if (shader->IsReady()) {
		this->shader = std::move(shader);
#ifdef DEBUG
		lastModified = raylib::GetFileModTime(path);
#endif
		return true;
	}

	return false;
}

#ifdef DEBUG
void HotReloadShader::autoReloadIfNeeded() {
	timer += GetFrameTime();
    if (timer >= 1.0) timer -= 1.0;
    else return;

	const unsigned long modifiedTime = raylib::GetFileModTime(path);
	if (modifiedTime > lastModified) {
        TraceLog(LOG_INFO, "HotReloadShader::autoReloadIfNeeded() - Reloading shader: %s", path.c_str());
        load();
    }
}
#endif

void HotReloadShader::beginMode() {
	if (shader != nullptr && shader->IsReady()) {
		shader->BeginMode();
	}
}

void HotReloadShader::endMode() {
	if (shader != nullptr && shader->IsReady()) {
		shader->EndMode();
	}
}

const int HotReloadShader::getUniformLocation(std::string name) {
    if (shader != nullptr && shader->IsReady()) {
        return shader->GetLocation(name);
    }

    return -1;
}

void HotReloadShader::setUniform(const int location, const void *value, ShaderUniformDataType uniformType) {
    if (shader != nullptr && shader->IsReady()) {
        shader->SetValue(location, value, uniformType);
    }
}
