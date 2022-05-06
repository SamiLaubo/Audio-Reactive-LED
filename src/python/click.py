import config
import time

active_color = '#16dbeb'
inactive_color = '#FFFFFF'

def energy_click(x, callback, energy_label, scroll_label, spectrum_label):
    config.visualization_effect = callback
    energy_label.setText('Energy', color=active_color)
    scroll_label.setText('Scroll', color=inactive_color)
    spectrum_label.setText('Spectrum', color=inactive_color)

    config.switch_mode_time = time.time()

def scroll_click(x, callback, energy_label, scroll_label, spectrum_label):
    config.visualization_effect = callback
    energy_label.setText('Energy', color=inactive_color)
    scroll_label.setText('Scroll', color=active_color)
    spectrum_label.setText('Spectrum', color=inactive_color)

    config.switch_mode_time = time.time()
    
def spectrum_click(x, callback, energy_label, scroll_label, spectrum_label):
    config.visualization_effect = callback
    energy_label.setText('Energy', color=inactive_color)
    scroll_label.setText('Scroll', color=inactive_color)
    spectrum_label.setText('Spectrum', color=active_color)

    config.switch_mode_time = time.time()

next = None