import time
import numpy as np
import pyaudio
import config
import click


def start_stream(callback):
    p = pyaudio.PyAudio()

    frames_per_buffer = int(config.MIC_RATE / config.FPS)
    stream = p.open(format=pyaudio.paInt16,
                    channels=1,
                    rate=config.MIC_RATE,
                    input=True,
                    frames_per_buffer=frames_per_buffer,
                    input_device_index=1)
    overflows = 0
    prev_ovf_time = time.time()

    while True:
        try:
            if time.time() - config.switch_mode_time > 10:
                click.next()
                config.switch_mode_time = time.time()
                print("Switch mode")

            y = np.fromstring(stream.read(frames_per_buffer, exception_on_overflow=False), dtype=np.int16)
            y = y.astype(np.float32)
            stream.read(stream.get_read_available(), exception_on_overflow=False)
            callback(y)
        except IOError:
            overflows += 1
            if time.time() > prev_ovf_time + 1:
                prev_ovf_time = time.time()
                print('Audio buffer has overflowed {} times'.format(overflows))
    stream.stop_stream()
    stream.close()
    p.terminate()
