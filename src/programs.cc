#include "programs.hh"

void ShadersLoader::load(const string &path)
{
	done = false;
	if (pendingTasks.count(path) == 0)
	{
		++pendingTaskCount;
		pendingTasks.emplace(path,
			std::async(std::launch::async, [this](const string &path) {
			--pendingTaskCount;
			return Files::readText(string(SHADERS_PATH) + path + string(".glsl"));
		}, path)
		);
	}

}

bool ShadersLoader::isLoading() const
{
	return !done;
}

void ShadersLoader::updateLoadProgress()
{
	done = (pendingTaskCount == 0);
}

void ShadersLoader::onFinish(const string &path, const Files::OnRead &callback)
{
	if (done)
		callback(pendingTasks[path].get());
}

void ShadersLoader::onFinish(const std::function<void()> &callback)
{
	if (done)
	{
		pendingTasks.clear();
		callback();
	}
}

//

uint Programs::create(const string &program)
{
	auto id = gl.program.create();
	programs[program] = id;

	return id;
}

uint Programs::use(const string& program)
{
	auto found = programs.find(program);
	if (found != programs.end())
	{
		auto id = found->second;
		gl.program.use(id);
		return id;
	}

	return 0;
}

uint Programs::addShader(uint type, const string &shader, const string &source)
{
	auto id = gl.shader.create(type);
	gl.shader.source(id, source);
	gl.shader.compile(id);
	shaders[shader] = id;

	return id;
}

string Programs::compileLog(const string &shader) const
{
	auto id = shaders.at(shader);
	auto type = gl.shader.getInt(id, GL_SHADER_TYPE);
	auto maxLen = gl.shader.getInt(id, GL_INFO_LOG_LENGTH);

	string kind = "???";
	if (gl.shader.Types.find(type) != std::end(gl.shader.Types))
		kind = gl.shader.Types.at(type);

	std::stringstream ss;

	if (!gl.shader.getInt(id, GL_COMPILE_STATUS))
	{
		ss << kind << ":\n";
		ss << gl.shader.infoLog(id, maxLen) << "\n";
	}
	else
		ss << kind << ": ok\n";

	return ss.str();
}

bool Programs::link(const string& program)
{
	auto id = programs[program];
	gl.program.link(id);
	use(program);
	bool success = gl.program.getInt(id, GL_LINK_STATUS);

	if (success)
	{
		// mark for deletion
		for (const auto& id : attachedShaders[program])
			gl.shader.detach(programs[program], id);

		for (const auto& id : attachedShaders[program])
			gl.shader.del(id);
	}
	gl.program.use(0);

	return success;
}

void Programs::del(const string &program)
{
	if (exists(program))
	{
		gl.program.del(programs[program]);
		attachedShaders.erase(program);
	}
}

void Programs::attachShader(const string &program, const string &shader)
{
	auto shaderID = shaders[shader];
	gl.shader.attach(programs[program], shaderID);
	attachedShaders[program].push_back(shaderID);
}

bool Programs::exists(const string &program) const
{
	auto found = programs.find(program);
	return found != programs.end();
}

