#include <fstream>
#include <vector>

#include "xmltweaker_internal.h"
#include "util/util.h"

extern "C" {
#include "wren.h"
}

using namespace pd2hook;
using namespace pd2hook::tweaker;
using namespace std;

static WrenVM *vm = NULL;

static void err(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message)
{
	PD2HOOK_LOG_LOG(string("[WREN ERR] ") + string(module) + ":" + to_string(line) + " ] " + message);
}

static void log(WrenVM* vm)
{
	const char *text = wrenGetSlotString(vm, 1);
	PD2HOOK_LOG_LOG(string("[WREN] ") + text);
}

void io_listDirectory(WrenVM* vm) {
	string filename = wrenGetSlotString(vm, 1);
	bool dir = wrenGetSlotBool(vm, 2);
	vector<string> files = Util::GetDirectoryContents(filename, dir);

	wrenSetSlotNewList(vm, 0);

	for (string const &file : files)
	{
		if (file == "." || file == "..") continue;

		wrenSetSlotString(vm, 1, file.c_str());
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void io_info(WrenVM* vm) {
	const char* path = wrenGetSlotString(vm, 1);
	DWORD dwAttrib = GetFileAttributesA(path);

	if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
		wrenSetSlotString(vm, 0, "none");
	}
	else if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
		wrenSetSlotString(vm, 0, "dir");
	}
	else {
		wrenSetSlotString(vm, 0, "file");
	}
}

void io_read(WrenVM *vm) {
	string file = wrenGetSlotString(vm, 1);
	string contents = Util::GetFileContents(file);
	wrenSetSlotString(vm, 0, contents.c_str());
}

vector<string> import_todo;
void io_dynamic_import(WrenVM *vm) {
	// TODO do this properly
	string module = wrenGetSlotString(vm, 1);
	import_todo.push_back(module);
	printf("Preparing: %s\n", module.c_str());
}

static WrenForeignMethodFn bindForeignMethod(
	WrenVM* vm,
	const char* module,
	const char* className,
	bool isStatic,
	const char* signature)
{
	if (strcmp(module, "base/native") == 0)
	{
		if (strcmp(className, "Logger") == 0)
		{
			if (isStatic && strcmp(signature, "log(_)") == 0)
			{
				return &log; // C function for Math.add(_,_).
			}
			// Other foreign methods on Math...
		}
		else if (strcmp(className, "IO") == 0)
		{
			if (isStatic && strcmp(signature, "listDirectory(_,_)") == 0)
			{
				return &io_listDirectory;
			}
			if (isStatic && strcmp(signature, "info(_)") == 0)
			{
				return &io_info;
			}
			if (isStatic && strcmp(signature, "read(_)") == 0)
			{
				return &io_read;
			}
			if (isStatic && strcmp(signature, "dynamic_import(_)") == 0)
			{
				return &io_dynamic_import;
			}
		}
		// Other classes in main...
	}
	// Other modules...

	return NULL;
}

static char* getModulePath(WrenVM* vm, const char* name)
{
	string xname = string("mods/") + string(name) + string(".wren");
	string str = Util::GetFileContents(xname);

	size_t length = str.length() + 1;
	char* output = (char*)malloc(length); // +1 for the null
	strcpy_s(output, length, str.c_str());

	return output; // free()d by Wren
}

const char* tweaker::transform_file(const char* text)
{
	if (vm == NULL) {
		WrenConfiguration config;
		wrenInitConfiguration(&config);
		config.errorFn = &err;
		config.bindForeignMethodFn = &bindForeignMethod;
		config.loadModuleFn = &getModulePath;
		vm = wrenNewVM(&config);

		WrenInterpretResult compileResult = wrenInterpret(vm, R"!( import "base/base" )!");
		printf("Compile: %d\n", compileResult);
		if (compileResult != WREN_RESULT_SUCCESS) Sleep(20000);
	}

	for (string const& module : import_todo) {
		string line = string("import \"") + module + string("\"");
		WrenInterpretResult compileResult = wrenInterpret(vm, line.c_str());
		printf("Module Load: %d\n", compileResult);
		if (compileResult != WREN_RESULT_SUCCESS) Sleep(20000);
	}
	import_todo.clear();

	wrenEnsureSlots(vm, 4);

	wrenGetVariable(vm, "base/base", "BaseTweaker", 0);
	WrenHandle* tweakerClass = wrenGetSlotHandle(vm, 0);
	WrenHandle* sig = wrenMakeCallHandle(vm, "tweak(_,_,_)");

	char hex[17]; // 16-chars long +1 for the null

	wrenSetSlotHandle(vm, 0, tweakerClass);

	sprintf_s(hex, 17, "%016llx", tweaker::last_loaded_name);
	wrenSetSlotString(vm, 1, hex);

	sprintf_s(hex, 17, "%016llx", tweaker::last_loaded_ext);
	wrenSetSlotString(vm, 2, hex);

	wrenSetSlotString(vm, 3, text);

	WrenInterpretResult result2 = wrenCall(vm, sig);
	if (result2 != WREN_RESULT_SUCCESS) Sleep(20000); // TODO

	wrenReleaseHandle(vm, tweakerClass);
	wrenReleaseHandle(vm, sig);

	const char* new_text = wrenGetSlotString(vm, 0);

	return new_text;
}