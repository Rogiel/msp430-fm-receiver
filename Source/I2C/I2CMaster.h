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
#include <stdint.h>

namespace I2C {
	class I2CMaster {
	public:
		/**
		 * O tipo representando o endeço de um dispositivo. Endereços no I2C tem 7 bits.
		 */
		typedef uint8_t DeviceAddress;

		/**
		 * O tipo byte
		 */
		typedef uint8_t DataByte;

		/**
		 * O tipo de dados
		 */
		typedef DataByte* Data;

		/**
		 * O tipo representando o handler para transmissões
		 */
		typedef void(*TransmitHandler)(I2CMaster& master);

		/**
		 * O tipo representando o handler para recepções
		 */
		typedef void(*ReceiveHandler)(I2CMaster& master, uint8_t bytes);

	private:
		/**
		 * Define o buffer de transmissão
		 */
		volatile DataByte _transmitBuffer[64];

		/**
		 * Define o tamanho atual do buffer de transmissão
		 */
		volatile uint8_t _transmitBufferSize;

		/**
		 * Define o ponteiro atual dentro do buffer de transmissão
		 */
		volatile uint8_t _transmitBufferPointer;

		/**
		 * O handler chamado após terminar uma transmissão
		 */
		TransmitHandler _transmitHandler;

	private:
		/**
		 * Define o buffer de recepção
		 */
		volatile DataByte _receiveBuffer[64];

		/**
		 * Define o tamanho atual do buffer de recepção
		 */
		volatile uint8_t _receiveBufferSize;

		/**
		 * Define o ponteiro atual dentro do buffer de recepção
		 */
		volatile uint8_t _receiveBufferPointer;

		/**
		 * Define o ponteiro atual de leitura dentro do buffer de recepção
		 */
		volatile uint8_t _receiveBufferReadPointer;

		/**
		 * O handler invocado após uma recepção
		 */
		ReceiveHandler _receiveHandler;

	private:
		/**
		 * Uma flag indicando se o bus está ocupado
		 */
		volatile bool _busy;

	public:
		/**
		 * Cria um novo bus I2C
		 */
		I2CMaster();

		/**
		 * Destroi o bus I2C e seta os SCL e SDA para o valor baixo
		 */
		~I2CMaster();

	public:
		/**
		 * Transmite uma série bytes para o bus
		 *
		 * @param address o endereço do dispostivo destinatário (7 bits)
		 * @param data uma array de dados
		 * @param size o tamanho da array de dados
		 *
		 * @return true se os dados podem ser enfileirados para envio
		 */
		bool transmit(DeviceAddress address, Data data, uint8_t size);

		/**
		 * Transmite uma série de bytes para o bus
		 *
		 * @param address o endereço do dispostivo destinatário (7 bits)
		 * @param string uma string
		 *
		 * @return true se os dados podem ser enfileirados para envio
		 */
		bool transmit(DeviceAddress address, const char* string);

		/**
		 * Anexa um handler de transmissão
		 *
		 * @param transmitHandler o handler de transmissão
		 */
		void attachTransmitHandler(TransmitHandler transmitHandler);

	public:
		/**
		 * Recebe um byte do bus.
		 *
		 * @param ack envia um sinal de ACK após recebebimento
		 *
		 * @return o byte lido do bus
		 */
		bool receive(DeviceAddress address, uint8_t size, bool ack = true);

		/**
		 * Verifica se há bytes disponiveis no buffer
		 *
		 * @return true se há buffers no buffer
		 */
		bool available();

		/**
		 * Retorna um byte de dados do buffer de recepção
		 *
		 * @return o byte no buffer de recepção
		 */
		DataByte get();

		/**
		 * Anexa um handler de recepção
		 *
		 * @param receiveHandler o handler de recepção
		 */
		void attachReceiveHandler(ReceiveHandler receiveHandler);

	public:
		/**
		 * Verifica se o bus está ocupado.
		 *
		 * @return true se o bus está ocupado.
		 */
		volatile bool isBusy();

		/**
		 * Espera até o bus ser liberado.
		 */
		void wait();

	public:
		/**
		 * Ativa o bus como o bus principal
		 */
		void enable();

		/**
		 * Desativa o bus como o bus principal
		 */
		void disable();

		/**
		 * Verifica se o bus é o bus ativo
		 *
		 * @return true se o bus é o bus ativo atualmente
		 */
		bool isActive();

		/**
		 * Retorna o bus I2C master compartilhado
		 */
		static I2CMaster& activeBus();

	private:
		/**
	 	 * Executa rotinas de limpeza no buffer de transmissão
	 	 */
		void cleanupTransmitBuffer();

		/**
		 * Executa rotinas de limpeza no buffer de recepção
		 */
		void cleanupReceiveBuffer();

	private:
		/**
		 * Define a ISR como friend da classe
		 */
		friend void USCIAB0TX_ISR();

		/**
		 * Define a ISR como friend da classe
		 */
		friend void USCIAB0RX_ISR();

	private:
		/**
		 * Evento chamado quando uma interrupção de transmissão ocorrer
		 */
		void onTransmit();

		/**
		 * Evento chamado quando uma interrupção de recepção ocorrer
		 */
		void onReceive();
	};
}