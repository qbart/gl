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
	uint inUseProg{ 0 };
	unordered_map<uint, unordered_map<string, uint>> uniformLocations;
	GL gl;

public:
	void load(const string &name);
	uint use(const string &program);
	void reloadAll(); 
	void delAll();

	void uniform(const string &name, const mat4 &mat);

private:
	uint create(const string &program);
	bool link(const string &program);
	string compileLog(const string &shader) const;
	uint addShader(uint type, const string &program, const string &shader, const string &source);
	int uniformLocation(const string &name);
};
