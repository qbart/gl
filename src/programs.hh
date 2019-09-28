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
	uint create(const string &program);
	uint use(const string& program);
	string compileLog(const string &shader) const;
	bool link(const string& program);
	void del(const string &program);
	void load(const string &name);

private:
	bool exists(const string &program) const;
	uint addShader(uint type, const string &program, const string &shader, const string &source);
};
