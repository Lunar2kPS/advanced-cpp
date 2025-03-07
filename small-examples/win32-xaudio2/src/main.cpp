#include <iostream>
#include <Windows.h>
#include <xaudio2.h>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;
using std::strncmp;
using std::getline;

namespace this_thread = std::this_thread;
namespace chrono = std::chrono;

/// @brief The data contained from the beginning of a .wav file.
struct WavHeader {
    char riff[4];
    uint32_t fileSize;
    char wave[4];
    char fmt[4];
    uint32_t fmtSize;
    uint16_t formatTag;
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4];
    uint32_t dataSize;
};

class VoiceCallback : public IXAudio2VoiceCallback {
    private:
        HANDLE event;

    public:
        VoiceCallback() {
            event = CreateEvent(nullptr, false, false, nullptr);
        }

        ~VoiceCallback() {
            CloseHandle(event);
        }

        HANDLE getEvent() { return event; }

        void OnBufferEnd(void* bufferContext) override {
            SetEvent(event);
        }

        void OnVoiceProcessingPassStart(UINT32 bytesRequired) override { }
        void OnVoiceProcessingPassEnd() override { }
        void OnStreamEnd() override { }
        void OnBufferStart(void* bufferContext) override { }
        void OnLoopEnd(void* bufferContext) override { }
        void OnVoiceError(void* bufferContext, HRESULT error) override { }
};

//NOTE: Just here for RAII:
struct MainProgramData {
    bool initializedXAudio2 = false;
    IXAudio2* audio = nullptr;
    IXAudio2MasteringVoice* master = nullptr;
    IXAudio2SourceVoice* voice = nullptr;
    BYTE* audioData;
    bool isPlaying = false;
    VoiceCallback voiceCallback = { };

    ~MainProgramData() {
        if (voice != nullptr)
        voice->DestroyVoice();
        if (master != nullptr)
            master->DestroyVoice();
        if (audio != nullptr)
            audio->Release();

        if (audioData != nullptr)
            delete[] audioData;
        if (initializedXAudio2)
        CoUninitialize();
    }
};

bool loadWavFile(const char* filePath, WavHeader* result, BYTE** data) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open WAV file: " << filePath << endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(result), sizeof(WavHeader));
    if (strncmp(result->riff, "RIFF", 4) != 0 || strncmp(result->wave, "WAVE", 4) != 0) {
        cerr << "Invalid WAV file format!" << endl;
        return false;
    }

    *data = new BYTE[result->dataSize];
    file.read(reinterpret_cast<char*>(*data), result->dataSize);
    file.close();

    return true;
}

int main() {
    try {
        MainProgramData data = { };

        //NOTE: Required for XAudio2
        HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (result != S_OK) {
            cerr << "Failed to initialize XAudio2." << endl;
            throw 1;
        }
        data.initializedXAudio2 = true;

        //NOTE: Returns S_OK if good, or an XAudio2 error code (see: https://learn.microsoft.com/en-us/windows/win32/xaudio2/xaudio2-error-codes)
        result = XAudio2Create(&data.audio);
        if (result != S_OK) {
            cerr << "Failed to create XAudio2 interface." << endl;
            throw 2;
        }
        
        cout << "What .wav file would you like to play?" << endl;
        string input = "";
        getline(cin, input);

        if (input.empty())
            throw 3;

        WavHeader wavHeader = { };
        if (!loadWavFile(input.c_str(), &wavHeader, &data.audioData)) {
            cerr << "Failed to load WAV file at " << input << "." << endl;
            throw 3;
        }

        data.audio->CreateMasteringVoice(&data.master);

        WAVEFORMATEX format = { };
        format.wFormatTag = wavHeader.formatTag;
        format.nChannels = wavHeader.channels;
        format.nSamplesPerSec = wavHeader.sampleRate;
        format.nAvgBytesPerSec = wavHeader.byteRate;
        format.nBlockAlign = wavHeader.blockAlign;
        format.wBitsPerSample = wavHeader.bitsPerSample;
        format.cbSize = 0;

        result = data.audio->CreateSourceVoice(&data.voice, &format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &data.voiceCallback);
        if (result != S_OK) {
            cerr << "Failed to create audio voice for playback." << endl;
            throw 4;
        }

        XAUDIO2_BUFFER buffer = { };
        buffer.AudioBytes = wavHeader.dataSize;
        buffer.pAudioData = data.audioData;
        buffer.Flags = XAUDIO2_END_OF_STREAM;

        result = data.voice->SubmitSourceBuffer(&buffer);
        if (result != S_OK) {
            cerr << "Failed to submit audio buffer." << endl;
            throw 5;
        }

        result = data.voice->Start(0);
        if (result != S_OK) {
            cerr << "Failed to play audio file." << endl;
        } else {
            data.isPlaying = true;

            //NOTE: This waits in one go for the full audio to finish:
            // WaitForSingleObject(data.voiceCallback.getEvent(), INFINITE);

            //NOTE: This loops, demonstrating potentially doing other work, while polling to see if the audio is finished:
            while (data.isPlaying) {
                this_thread::sleep_for(chrono::milliseconds(50));
                cout << "... (50ms)" << endl;
                data.isPlaying = WaitForSingleObject(data.voiceCallback.getEvent(), 0) == WAIT_TIMEOUT;
            }
            cout << "Audio playback complete!" << endl;
        }

        return 0;
    } catch (...) {
        //NOTE: Re-throw to check its type. Generally avoid this, but the main program should catch EVERYTHING to make sure RAII takes effect.
        try {
            throw;
        } catch (int value) {
            return value;
        } catch (...) {
            cerr << "Caught an unknown type of exception in the main program!" << endl;
            return 1000;
        }
    }
}
