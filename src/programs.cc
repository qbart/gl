#include "programs.hh"
#include "log.hh"
#include <taskflow.hpp>


uint Programs::create(const string &program)
{
	auto id = gl.program.create();
	programs[program] = id;
	for (const auto &id : attachedShaders[program])
		gl.shader.attach(programs[program], id);
	
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

void Programs::load(const string & name)
{
	string basepath = string(SHADERS_PATH) + name;
	string vertpath = basepath + "/vert.glsl";
	string geompath = basepath + "/geom.glsl";
	string fragpath = basepath + "/frag.glsl";

	struct Shader
	{
		Files::ReadStatus vert;
		Files::ReadStatus geom;
		Files::ReadStatus frag;
	} shader;

	tf::Executor executor;
	tf::Taskflow taskflow;

	taskflow.emplace([&vertpath, &name, &shader]() {
		if (Files::exists(vertpath))
			shader.vert = Files::readText(vertpath);
	});
	taskflow.emplace([&geompath, &name, &shader]() {
		if (Files::exists(geompath))
			shader.geom = Files::readText(geompath);
	});
	taskflow.emplace([&fragpath, &name, &shader]() {
		if (Files::exists(fragpath))
			shader.frag = Files::readText(fragpath);
	});

	executor.run(taskflow).get();

	if (Files::isLoaded(shader.vert))
		addShader(GL_VERTEX_SHADER, name, "vert", Files::text(shader.vert));
	if (Files::isLoaded(shader.geom))
		addShader(GL_GEOMETRY_SHADER, name, "geom", Files::text(shader.geom));
	if (Files::isLoaded(shader.frag))
		addShader(GL_FRAGMENT_SHADER, name, "frag", Files::text(shader.frag));

	create(name);
	if (!link(name))
	{
		status(std::cerr, "program/" + name, false);
		std::cerr << "-- Vert --\n" << compileLog(name + "/vert");
		std::cerr << "-- Frag --\n" << compileLog(name + "/frag");
	}
	else
		status(std::cout, "program/" + name, true);
}

bool Programs::exists(const string &program) const
{
	auto found = programs.find(program);
	return found != programs.end();
}

uint Programs::addShader(uint type, const string &program, const string &shader, const string &source)
{
	auto id = gl.shader.create(type);
	gl.shader.source(id, source);
	gl.shader.compile(id);
	shaders[program + "/" + shader] = id;
	attachedShaders[program].push_back(id);

	return id;
}