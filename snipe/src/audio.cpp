#include "audio/audio.h"
#include "utils.h"

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
	if (pDecoder == NULL) {
		return;
	}
	ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
	(void)pInput;
}

void Audio::load(std::wstring _path) {
	ma_result result;
	filePath = ws2s(_path);
	result = ma_decoder_init_file(filePath.c_str(), NULL, &decoder);
	if (result != MA_SUCCESS) { printf("Could not load file\n"); }
	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = decoder.outputFormat;
	deviceConfig.playback.channels = decoder.outputChannels;
	deviceConfig.sampleRate = decoder.outputSampleRate;
	deviceConfig.dataCallback = data_callback;
	deviceConfig.pUserData = &decoder;
	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
		printf("Failed to open playback device.\n");
		ma_decoder_uninit(&decoder);
	}
	if (ma_device_start(&device) != MA_SUCCESS) {
		printf("Failed to start playback device.\n");
		ma_device_uninit(&device);
		ma_decoder_uninit(&decoder);
	}
	ma_device_stop(&device);
}

Audio::Audio(std::wstring _name) : name(_name){

}

Audio::~Audio() {
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);
}
void Audio::play() {
	playing = true;
	ma_device_start(&device);
}

void Audio::playLoop() {
	ma_data_source_set_looping(&decoder, MA_TRUE);

	playing = true;
	ma_device_start(&device);
}

void Audio::stop() {
	ma_device_stop(&device);
	playing = false;
}

void fadeout(Audio* target, float duration) {
	float step = target->volume / (duration * 10.0f);
	while (target->volume > 0.0f) {
		target->setVolume(target->volume - step);
		Sleep(100);
	}
	target->volume = 0.0f;
	target->stop();
}

void fadein(Audio* target, float duration) {
	float end = target->volume;
	float step = target->volume / (duration * 10.0f);
	target->volume = 0.0f;
	while (target->volume < end) {
		target->setVolume(target->volume + step);
		Sleep(100);
	}
	target->volume = end;
}

void Audio::stopFade(float dur) {
	std::thread t(fadeout, this, dur);
	t.detach();
	if (t.joinable())t.join();
}

void Audio::playFade(float dur) {
	std::thread t(fadein, this, dur);
	t.detach();
	if (t.joinable())t.join();
	play();
}

void Audio::playLoopFade(float dur) {
	std::thread t(fadein, this, dur);
	t.detach();
	if (t.joinable())t.join();
	playLoop();
}

void Audio::setVolume(float v){
	ma_device_set_master_volume(&device, clamp(v, 0.0f, 1.0f));
	volume = v;
}