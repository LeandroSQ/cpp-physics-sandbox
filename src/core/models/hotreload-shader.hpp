#pragma once

#include "../precomp.hpp"

class HotReloadShader {
  private:
	std::unique_ptr<raylib::Shader> shader = nullptr;
	std::string path;

#ifdef DEBUG
	unsigned long lastModified = 0L;
	float timer = 0;
#endif

  public:
	HotReloadShader(std::string_view path) : path(path) { }

	bool load();

#ifdef DEBUG
	void autoReloadIfNeeded();
#endif

	void beginMode();

	void endMode();

    const int getUniformLocation(std::string name);

    void setUniform(const int location, const void *value, ShaderUniformDataType uniformType);
};
