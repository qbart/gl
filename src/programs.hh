#pragma once
#include "types.hh"
#include "files.hh"
#include "gl.hh"
#include <atomic>
#include <future>
#include <functional>

class Programs
{
private:
	unordered_map<string, uint> shaders;
	unordered_map<string, uint> programs;
	unordered_map<string, vector<uint>> attachedShaders;
	GL gl;

public:
	void load(const string &name);
	uint use(const string& program);
	void reloadAll();
	void delAll();

private:
	uint create(const string &program);
	bool link(const string& program);
	string compileLog(const string &shader) const;
	uint addShader(uint type, const string &program, const string &shader, const string &source);
};
