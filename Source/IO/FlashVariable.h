//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Implementação I2C
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#pragma once

namespace IO {
	template<typename T>
	class FlashVariable {
	private:
		uint8_t* _value;

	public:
		FlashVariable(unsigned int addr) : _value((uint8_t*) addr) {};

	public:
		FlashVariable<T>& operator=(const T value) {
			while ((BUSY & FCTL3));
			FCTL1 = FWKEY + ERASE; // Enable flash erase.
			FCTL3 = FWKEY; // Unlock the flash.

			*_value = 0x00; // Erase the data from the flash.

			while ((BUSY & FCTL3));
			FCTL1 = FWKEY; // Disable flash write.
			FCTL3 = FWKEY + LOCK; // Lock the flash.

			while ((BUSY & FCTL3));
			FCTL1 = FWKEY + WRT; // Enable flash write.
			FCTL3 = FWKEY; // Unlock the flash.

			const uint8_t* ptr = (const uint8_t*) &value;
			for(uint16_t b = 0; b < sizeof(T); b++) {
				*(_value+b) = *(ptr + b);
				while ((BUSY & FCTL3));
			}

			FCTL1 = FWKEY; // Disable flash write.
			FCTL3 = FWKEY + LOCK; // Lock the flash.
		}

		operator T() const {
			return *((T*) _value);
		}

		const T* operator->() const {
			return (T*) _value;
		}
	};

	/**
	 * Cria uma variável na memória flash. Variáveis na memória flash são removidas ao gravar o programa.
	 */
	#define DECLARE_FLASH_VARIABLE(Type, Name) \
		Type ____##Name __attribute__((section(".storage")));\
		IO::FlashVariable<Type> Name((uint16_t) &____##Name);
}