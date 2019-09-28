#pragma once
#include "types.hh"
#include "files.hh"
#include "gl.hh"
#include <atomic>
#include <future>
#include <functional>

class ShadersLoader
{
private:
	std::atomic<uint> pendingTaskCount = 0;
	unordered_map<string, std::future<Files::ReadStatus>> pendingTasks;
	bool done = true;

public:
	void load(const string &path);
	bool isLoading() const;
	void onFinish(const string &path, const Files::OnRead &callback);
	void onFinish(const std::function<void()> &callback);
	void updateLoadProgress();
};


class Programs
{
private:
	GL gl;
	unordered_map<string, uint> shaders;
	unordered_map<string, uint> programs;
	unordered_map<string, vector<uint>> attachedShaders;

public:
	ShadersLoader loader;

	uint create(const string &program);
	uint use(const string& program);
	uint addShader(uint type, const string &program, const string &source);
	auto addVertexShader(const string &shader, const string &source) { return addShader(GL_VERTEX_SHADER, shader, source); }
	auto addFragmentShader(const string &shader, const string &source) { return addShader(GL_FRAGMENT_SHADER, shader, source); }
	string compileLog(const string &shader) const;
	bool link(const string& program);
	void del(const string &program);
	void attachShader(const string &program, const string &shader);
	bool exists(const string &program) const;
};
