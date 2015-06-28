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

#pragma once

#include <msp430.h>

namespace IO {
	/**
	 * Representa uma variável armazenada na memória flash.
	 *
	 * Este template auxilia a ativar e desativar os registradores da memória flash ao alterar o valor.
	 *
	 * @warning É fundamental que os tipos armazenados neste template sejam IMUTÁVEIS!
	 */
	template<typename T>
	class FlashVariable {
	private:
		/**
		 * Um ponteiro para o valor armazenado na memória flash.
		 * No msp430, a flash é mepeada na memória.
		 */
		uint8_t* _value;

	public:
		/**
		 * Cria uma nova instância de uma variável flash
		 *
		 * @param addr o endereço da variável no mapeamento da flash
		 */
		FlashVariable(unsigned int addr) : _value((uint8_t*) addr) {
			// configura o controlador de memória flash
			FCTL2 = FWKEY + FSSEL_2 + FN5 + FN3;
		};

	public:
		/**
		 * Overload do operator de atribuiçao.
		 *
		 * @param value o novo valor para ser gravado na memória flash
		 * @return this
		 */
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

		/**
		 * Retorna uma cópia do objeto armazenado na memória flash
		 * @return uma cópia do valor na flash
		 */
		operator T() const {
			return *((T*) _value);
		}

		/**
		 * Retorna uma referência imutável do objeto armazenado na memória flash
		 * @return uma referencia constante do valor na flash
		 */
		operator const T&() const {
			return *((T*) _value);
		}

		/**
		 * Overload do operator ->
		 * @return um ponteiro imutável para o valor na memória flash
		 */
		const T* operator->() const {
			return (T*) _value;
		}
	};

	/**
	 * Cria uma variável na memória flash. Variáveis na memória flash são removidas ao gravar o programa.
	 *
	 * @param Type o tipo de objeto/valor para armazenar na memória flash
	 * @param Name o nome da variável para acessar o wrapper
	 */
	#define DECLARE_FLASH_VARIABLE(Type, Name) \
		Type ____##Name __attribute__((section(".storage")));\
		IO::FlashVariable<Type> Name((uint16_t) &____##Name);
}