
#include "XAudio.h"

#ifdef ENABLE_XAUDIO

#include "signatures/sigdef.h"
#include "util/util.h"

#include "stb_vorbis.h"

#include <AL/al.h>

#include <string>
#include <map>
#include <vector>

using namespace std;

#define WORLD_VEC(L, ix, iy, iz) \
(lua_tonumber(L, ix) / world_scale), \
(lua_tonumber(L, iy) / world_scale), \
(lua_tonumber(L, iz) / world_scale)

namespace pd2hook {
	double world_scale = 1;

	class XAResource {
	public:
		XAResource(ALuint alhandle) : alhandle(alhandle) {}
		ALuint Handle() {
			return alhandle;
		}
		void Employ() {
			usecount++;
		}
		void Discard(bool force) {
			usecount--;

			if (force && usecount <= 0) {
				Close();
			}
		}
		void Close() {
			if (!valid) return;
			valid = false;
			alDeleteBuffers(1, &alhandle);
		}
	private:
		const ALuint alhandle;
		bool valid = true;
		int usecount = 0;
	};

	class XALuaHandle {
	public:
		XALuaHandle(XAResource *resource) : resource(resource) {
			resource->Employ();
		}
		ALuint Handle(lua_State *L) {
			if (!open) luaL_error(L, "Cannot use closed resource!");
			return resource->Handle();
		}
		void Close(bool force) {
			if (!open) return;

			resource->Discard(force);
			open = false;
		}
		bool Ready() {
			return open;
		}
	private:
		XAResource *resource;
		bool open = true;
	};

	namespace xabuffer {
		class XABuffer : public XAResource {
			using XAResource::XAResource;
		};

		map<string, XABuffer*> openBuffers;

		static int lX_loadbuffer(lua_State *L) {
			int count = lua_gettop(L);

			ALuint buffers[32];

			if (count > 32) {
				PD2HOOK_LOG_LOG("Attempted to create more than 32 ALbuffers in a single call!");
				return 0;
			}

			vector<string> filenames;
			for (size_t i = 0; i < count; i++) {
				// i+1 because the Lua stack starts at 1, not 0
				filenames.push_back(lua_tostring(L, i + 1));
			}
			lua_settop(L, 0);

			alGenBuffers(count, buffers);
			for (size_t i = 0; i < count; i++) {
				string filename = filenames[i];

				if (openBuffers.count(filename)) {
					*(XALuaHandle*)lua_newuserdata(L, sizeof(XALuaHandle)) = XALuaHandle(openBuffers[filename]);
					// TODO don't create buffers for cached stuff
				}
				else {
					XABuffer *buff = new XABuffer(buffers[i]);
					*(XALuaHandle*)lua_newuserdata(L, sizeof(XALuaHandle)) = XALuaHandle(buff);

					openBuffers[filename] = buff;

					// Set the metatable
					luaL_getmetatable(L, "XAudio.buffer");
					lua_setmetatable(L, -2);

					// Load the contents of the buffer

					ALenum format;
					ALvoid *data;
					ALsizei size;
					ALsizei freq;

					int vorbisLen, channels, sampleRate;
					short *vorbis;

					vorbisLen = stb_vorbis_decode_filename(filename.c_str(),
						&channels,
						&sampleRate,
						&vorbis);

					// Copy the file into our buffer
					// TODO do this in the background
					ALenum error;
					alBufferData(buffers[i],
						channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
						vorbis,
						vorbisLen * sizeof(short),
						sampleRate
					);
					if ((error = alGetError()) != AL_NO_ERROR) {
						throw "alBufferData buffer 0 : " + error;
					}
				}
			}

			return count;
		}

		static int XABuffer_close(lua_State *L) {
			// TODO check userdata

			((XALuaHandle*)lua_touserdata(L, 1))->Close(lua_toboolean(L, 2));

			return 0;
		}
	};

	namespace xasource {
		class XASource : public XAResource {
			using XAResource::XAResource;
		};

		vector<XASource*> openSources;

		static int lX_new_source(lua_State *L) {
			int count = lua_gettop(L) == 0 ? 1 : lua_tointeger(L, 1);
			lua_settop(L, 0);

			ALuint sources[32];

			if (count > 32) {
				PD2HOOK_LOG_LOG("Attempted to create more than 32 ALsources in a single call!");
				return 0;
			}

			// Generate Sources 
			alGenSources(count, sources);

			// TODO expand stack to ensure we can't crash

			// Error reporting
			ALenum error;
			if ((error = alGetError()) != AL_NO_ERROR) {
				throw "alGenSources 1 : " + error;
			}

			for (size_t i = 0; i < count; i++) {
				XASource *buff = new XASource(sources[i]);
				*(XALuaHandle*)lua_newuserdata(L, sizeof(XALuaHandle)) = XALuaHandle(buff);

				// Set the metatable
				luaL_getmetatable(L, "XAudio.source");
				lua_setmetatable(L, -2);
			}

			return count;
		}

		static int XASource_close(lua_State *L) {
			// TODO check userdata

			((XALuaHandle*)lua_touserdata(L, 1))->Close(true);

			// TODO remove from openSources when applicable

			return 0;
		}

		static int XASource_set_buffer(lua_State *L) {
			XALuaHandle *xthis = (XALuaHandle*)lua_touserdata(L, 1);
			// TODO validate 'valid' flag

			bool nil = lua_isnil(L, 2);
			ALuint buffid;
			if (nil) {
				buffid = 0; // NULL Buffer - basically, no audio
			}
			else {
				XALuaHandle *buff = (XALuaHandle*) lua_touserdata(L, 2);
				// TODO validate 'valid' flag
				buffid = buff->Handle(L);
			}

			// Attach buffer 0 to source 
			alSourcei(xthis->Handle(L), AL_BUFFER, buffid);
			ALenum error;
			if ((error = alGetError()) != AL_NO_ERROR) {
				throw string("alSourcei AL_BUFFER 0 : " + error);
			}

			return 0;
		}

		static int XASource_play(lua_State *L) {
			XALuaHandle *xthis = (XALuaHandle*)lua_touserdata(L, 1);
			// TODO validate 'valid' flag

			alSourcePlay(xthis->Handle(L));
			// TODO error checking

			return 0;
		}

		static void set_vector_property(lua_State *L, ALenum type) {
			XALuaHandle *xthis = (XALuaHandle*)lua_touserdata(L, 1);
			// TODO validate 'valid' flag

			alSource3f(
				xthis->Handle(L),
				type,
				WORLD_VEC(L, 2, 3, 4)
			);
		}

		static int XASource_set_position(lua_State *L) {
			set_vector_property(L, AL_POSITION);
			return 0;
		}

		static int XASource_set_velocity(lua_State *L) {
			set_vector_property(L, AL_VELOCITY);
			return 0;
		}

		static int XASource_set_direction(lua_State *L) {
			// FIXME doesn't *seem* to work?
			set_vector_property(L, AL_DIRECTION);
			return 0;
		}
	};

	namespace xalistener {
		static void set_vector_property(lua_State *L, ALenum type) {
			// TODO we can use the listener property in L.1

			alListener3f(
				type,
				WORLD_VEC(L, 2, 3, 4)
			);
		}

		static int XAListener_set_position(lua_State *L) {
			set_vector_property(L, AL_POSITION);
			return 0;
		}

		static int XAListener_set_velocity(lua_State *L) {
			set_vector_property(L, AL_VELOCITY);
			return 0;
		}

		static int XAListener_set_orientation(lua_State *L) {
			// TODO we can use the listener property in L.1

			ALfloat orientation[6] = {
				WORLD_VEC(L, 2, 3, 4), // Forward vector
				WORLD_VEC(L, 5, 6, 7)  // Up vector
			};

			alListenerfv(AL_ORIENTATION, orientation);

			return 0;
		}

		static void lua_register(lua_State *L) {
			// blt.xaudio.listener table
			luaL_Reg lib[] = {
				{ "setposition", xalistener::XAListener_set_position },
				{ "setvelocity", xalistener::XAListener_set_velocity },
				{ "setorientation", xalistener::XAListener_set_orientation },
				{ NULL, NULL }
			};

			// Make a new table and populate it with XAudio stuff
			lua_newtable(L);
			luaI_openlib(L, NULL, lib, 0);

			// Put that table into the BLT table, calling it XAudio. This removes said table from the stack.
			lua_setfield(L, -2, "listener");
		}
	};

	static int lX_setup(lua_State *L) {
		try {
			XAudio::GetXAudioInstance();
		}
		catch (string msg) {
			PD2HOOK_LOG_ERROR("Exception while loading XAudio API: " + msg);
			lua_pushboolean(L, false);
			return 1;
		}
		lua_pushboolean(L, true);
		return 1;
	}

	static int lX_getworldscale(lua_State *L) {
		lua_pushnumber(L, world_scale);
		return 1;
	}

	static int lX_setworldscale(lua_State *L) {
		world_scale = lua_tonumber(L, 1);
		return 0;
	}

	XAudio::XAudio() {
		struct stat statbuf;

		dev = alcOpenDevice(NULL);
		if (!dev) {
			throw string("Cannot open OpenAL Device");
		}
		ctx = alcCreateContext(dev, NULL);
		alcMakeContextCurrent(ctx);
		if (!ctx) {
			throw string("Could not create OpenAL Context");
		}

		PD2HOOK_LOG_LOG("Loaded OpenAL XAudio API");
	}

	XAudio::~XAudio() {
		PD2HOOK_LOG_LOG("Closing OpenAL XAudio API");

		// Delete all sources
		// Do this first, otherwise buffers might not be deleted properly
		for (auto source : xasource::openSources) {
			source->Close();

			delete source;
		}

		// To remove dangling pointers
		// Should not be used again, but just to be safe
		xasource::openSources.clear();

		// Delete all buffers
		for (auto const& pair : xabuffer::openBuffers) {
			xabuffer::XABuffer *buff = pair.second;

			buff->Close();

			delete buff;
		}

		// Same as above
		xabuffer::openBuffers.clear();

		// TODO: Make sure above works properly.

		// Close the OpenAL context/device
		alcMakeContextCurrent(NULL);
		alcDestroyContext(ctx);
		alcCloseDevice(dev);
	}

	XAudio* XAudio::GetXAudioInstance() {
		static XAudio audio;
		return &audio;
	}

	void XAudio::Register(void * state) {
		lua_State *L = (lua_State*)state;

		// Buffer metatable
		luaL_Reg XABufferLib[] = {
			{ "close", xabuffer::XABuffer_close },
			{ NULL, NULL }
		};

		luaL_newmetatable(L, "XAudio.buffer");

		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);  /* pushes the metatable */
		lua_settable(L, -3);  /* metatable.__index = metatable */

		luaI_openlib(L, NULL, XABufferLib, 0);
		lua_pop(L, 1);

		// Source metatable
		luaL_Reg XASourceLib[] = {
			{ "close", xasource::XASource_close },
			{ "setbuffer", xasource::XASource_set_buffer },
			{ "play", xasource::XASource_play },
			{ "setposition", xasource::XASource_set_position },
			{ "setvelocity", xasource::XASource_set_velocity },
			{ "setdirection", xasource::XASource_set_direction },
			{ NULL, NULL }
		};

		luaL_newmetatable(L, "XAudio.source");

		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);  /* pushes the metatable */
		lua_settable(L, -3);  /* metatable.__index = metatable */

		luaI_openlib(L, NULL, XASourceLib, 0);
		lua_pop(L, 1);

		// blt.xaudio table
		luaL_Reg lib[] = {
			{ "setup", lX_setup },
			{ "loadbuffer", xabuffer::lX_loadbuffer },
			{ "newsource", xasource::lX_new_source },
			{ "getworldscale", lX_getworldscale },
			{ "setworldscale", lX_setworldscale },
			{ NULL, NULL }
		};

		// Grab the BLT table
		lua_getglobal(L, "blt");

		// Make a new table and populate it with XAudio stuff
		lua_newtable(L);
		luaI_openlib(L, NULL, lib, 0);

		// Add the blt.xaudio.listener table
		xalistener::lua_register(L);

		// Put that table into the BLT table, calling it XAudio. This removes said table from the stack.
		lua_setfield(L, -2, "xaudio");

		// Remove the BLT table from the stack.
		lua_pop(L, 1);
	}

}

#endif // ENABLE_XAUDIO
