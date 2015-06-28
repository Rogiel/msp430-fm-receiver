//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//
// https://github.com/Rogiel/msp430-fm-receiver
//

#include "FMReceiver.h"

namespace FM {
	namespace {
		union TEA5767Transmit {
			uint8_t bytes[5];

			TEA5767Transmit() {
				bytes[0] = 0;
				bytes[1] = 0;
				bytes[2] = 0;
				bytes[3] = 0;
				bytes[4] = 0;
			}

			/**
			 * Bits de configuração do receptor. Descrições retiradas do datahseet do TEA5767.
			 */
			struct __attribute__((__packed__)) {
				// byte 1
				/**
				 * if MUTE = 1 then L and R audio are muted; if MUTE = 0 then L and R audio are not muted
				 */
				uint8_t MUTE : 1;

				/**
				 * Search mode: if SM = 1 then in search mode; if SM = 0 then not in search mode
				 */
				uint8_t SM : 1;

				/**
				 * setting of synthesizer programmable counter for search or preset
				 */
				uint8_t PLL_H : 6;

				// byte 2
				/**
				 * setting of synthesizer programmable counter for search or preset
				 */
				uint8_t PLL_L : 8;

				// byte 3
				/**
				 * Search Up/Down: if SUD = 1 then search up; if SUD = 0 then search down
				 */
				uint8_t SUD : 1;

				/**
				 * Search Stop Level: see Table 11
				 */
				uint8_t SSL : 2;

				/**
				 * High/Low Side Injection:
				 * - if HLSI = 1 then high side LO injection;
				 * - if HLSI = 0 then low side LO injection
				 */
				uint8_t HLSI : 1;

				/**
				 * Mono to Stereo: if MS = 1 then forced mono; if MS = 0 then stereo ON
				 */
				uint8_t MS : 1;

				/**
				 * Mute Right:
				 * - if MR = 1 then the right audio channel is muted and forced mono;
				 * - if MR = 0 then the right audio channel is not muted
				 */
				uint8_t MR : 1;

				/**
				 * Mute Left:
				 * - if ML = 1 then the left audio channel is muted and forced mono;
				 * - if ML = 0 then the left audio channel is not muted
				 */
				uint8_t ML : 1;

				/**
				 * Software programmable port 1: if SWP1 = 1 then port 1 is HIGH; if SWP1 = 0 then port 1 is LOW
				 */
				uint8_t SWP1 : 1;

				// byte 4
				/**
				 * Software programmable port 2: if SWP2 = 1 then port 2 is HIGH; if SWP2 = 0 then port 2 is LOW
				 */
				uint8_t SWP2 : 1;

				/**
				 * Standby: if STBY = 1 then in Standby mode; if STBY = 0 then not in Standby mode
				 */
				uint8_t STBY : 1;

				/**
				 * Band Limits: if BL = 1 then Japanese FM band; if BL = 0 then US/Europe FM band
				 */
				uint8_t BL : 1;

				/**
				 * Clock frequency: see Table 16
				 */
				uint8_t XTAL : 1;

				/**
				 * Soft Mute: if SMUTE = 1 then soft mute is ON; if SMUTE = 0 then soft mute is OFF
				 */
				uint8_t SMUTE : 1;

				/**
				 * High Cut Control: if HCC = 1 then high cut control is ON; if HCC = 0 then high cut control is OFF
				 */
				uint8_t HCC : 1;

				/**
				 * Stereo Noise Cancelling: if SNC = 1 then stereo noise cancelling is ON; if SNC = 0 then stereo
				 * noise cancelling is OFF
				 */
				uint8_t SNC : 1;

				/**
				 * Search Indicator: if SI = 1 then pin SWPORT1 is output for the ready flag; if SI = 0 then pin
				 * SWPORT1 is software programmable port 1
				 */
				uint8_t SI : 1;

				// byte 5
				/**
				 * if PLLREF = 1 then the 6.5 MHz reference frequency for the PLL is enabled; if PLLREF = 0 then the
				 * 6.5 MHz reference frequency for the PLL is disabled; see Table 16
				 */
				uint8_t PLLREF : 1;

				/**
				 * if DTC = 1 then the de-emphasis time constant is 75 μs; if DTC = 0 then the de-emphasis time constant
				 * is 50 μs
				 */
				uint8_t DTC : 1;

				/**
				 * not used; position is don’t care
				 */
				uint8_t unused : 6;
			} bits;
		};
	}

	FMReceiver::FMReceiver(I2C::I2CMaster& bus) : _bus(bus), _needsUpdate(true), _mute(false), _frequency(93.0) {

	}

	FMReceiver::Frequency FMReceiver::getFrequency() const {
		return _frequency;
	}

	void FMReceiver::setFrequency(Frequency frequency) {
		_frequency = frequency;
		setNeedsUpdate();
	}

	bool FMReceiver::isMuted() const {
		return _mute;
	}

	void FMReceiver::mute() {
		_mute = true;
		setNeedsUpdate();
	}

	void FMReceiver::unmute() {
		_mute = false;
		setNeedsUpdate();
	}

	inline void FMReceiver::setNeedsUpdate() {
		_needsUpdate = true;
	}

	void FMReceiver::updateIfNeeded() {
		if(!_needsUpdate) return;
		update();
	}

	void FMReceiver::update() {
		TEA5767Transmit packet;

		uint16_t pll = calculatePLLWord();
		packet.bits.PLL_L = pll & 0xFF;
		packet.bits.PLL_H = pll >> 8;

		packet.bits.MUTE = (uint8_t) _mute;

		packet.bits.HLSI = 1;
		packet.bits.DTC = 1;

		// configure clock
		packet.bits.XTAL = 1;
		packet.bits.PLLREF = 0;

		_bus.transmit(I2C_ADDRESS, packet.bytes, 5);
		_needsUpdate = false;
	}

	uint16_t FMReceiver::calculatePLLWord() const {
		uint16_t word = 4 * (_frequency * 1000000 + 225000) / 32768;
		return word & 0x3FFF;
	}
}
