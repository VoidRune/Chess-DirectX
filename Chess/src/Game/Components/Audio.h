#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <assert.h>

#include <xaudio2.h>
#include <x3daudio.h>
#pragma comment(lib,"xaudio2.lib") 

struct Sound
{
private:
	friend class AudioEngine;

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	int currentVoice = -1;
	bool loaded = false;
};

struct float3
{
	float x;
	float y;
	float z;
};

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	void Play2D(Sound& sound,const int& channel);
	// +z is sound from right
	// -z is sound from left
	// innerRadius is blending point between channels
	void Play3D(Sound& sound, const int& channel, float3 direction, float innerRadius = 3.0f);
	void Update3D(Sound& sound, const int& channel, float3 direction, float innerRadius = 3.0f);

	//Returns channel ID
	int CreateChannel(int numOfStereoVoices = 16, int numOfMonoVoices = 16);

	void SetVolume(float volume, const int& channel);
	void SetMasterVolume(float volume);

	void Flush(const int& channel);
	void FlushAll();

	//Loads the sound in memory
	//Can load .wav .mp3 .ogg files
	void LoadData(Sound& sound, std::string filename, bool looping = false);

	//Loads the sound in memory
	//Can load .wav .mp3 .ogg files
	Sound& LoadData(std::string filename, bool looping = false);

	AudioEngine(const AudioEngine&) = delete;
private:

	struct Channel
	{
		int m_MaxStereoVoices = 32;
		int m_CurrentStereoVoice = 0;
		std::vector<IXAudio2SourceVoice*> StereoVoices;
		int m_MaxMonoVoices = 32;
		int m_CurrentMonoVoice = 0;
		std::vector<IXAudio2SourceVoice*> MonoVoices;
		IXAudio2SubmixVoice* submixVoice;
		XAUDIO2_SEND_DESCRIPTOR SFXSend = {};
		XAUDIO2_VOICE_SENDS SFXSendList = {};
	};

	std::vector<Channel> channels;

	IXAudio2* pXAudio2;
	X3DAUDIO_HANDLE audio3D = {};
	IXAudio2MasteringVoice* pMasterVoice;

	XAUDIO2_VOICE_DETAILS masterVoiceDetails;

	void Calculate3DMatrix(int channels, float* matrix, float3 direction, float innerRadius);
	void LoadWav(Sound& sound, std::string filename, bool looping = false);
	void LoadMp3(Sound& sound, std::string filename, bool looping = false);
	void LoadOgg(Sound& sound, std::string filename, bool looping = false);

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
};