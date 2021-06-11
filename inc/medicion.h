/*
 * medicion.h
 *
 *  Created on: 3 jun. 2021
 *      Author: nicolas
 */

#ifndef CODIGOSNICO_TPFINAL_PCSE_SD_INC_MEDICION_H_
#define CODIGOSNICO_TPFINAL_PCSE_SD_INC_MEDICION_H_

//************* includes *************
#include "main.h"
#include "medicion.h"
#include "sapi.h"     // <= sAPI header
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"      // API de bajo nivel para unidad "SDC:" en FAT FS

//************* variables y defines *************
uint8_t receivedByteUART_232;//variable que recibe el byte enviado desde la GUI.
bool_t modoElegido;	//modoElegido = 0 (MODO BIPOLAR) || modoElegido = 1 (MODO TETRAPOLAR)
bool_t calibracion;	//calibracion = 0 (sistema NO calibrado) || calibracion = 1 (sistema calibrado)
bool_t medicion;//medicion = 0 (medicion no realizada) || medicion = 1 (medicion realizada)
#define	 datoGUI	receivedByteUART_232

uint32_t valFrecuencia[100];
uint32_t valModulo[100];
uint32_t valFase[100];
uint32_t valRealZ[100];
uint32_t valImgZ[100];
FATFS fs;           // <-- FatFs work area needed for each volume
FIL fp;             // <-- File object needed for each open file
rtc_t rtc;

TCHAR filename[100]; // vector con nombre del archivo a almacenar en memoria SD
char buffer[100];// vector buffer para unir valores antes de gurdarlos en el archivo
UINT nbytes;
//uint8_t	 receivedByteUART_232;

//************* prototipos de funciones *************

void actualizaSistema(void);
void initVectores(uint32_t * vector, uint32_t * vector2, uint32_t * vector3,
		uint32_t * vector4, uint32_t * vector5);
void setModoBipolar(void);
void setModoTetrapolar(void);
void finalizaSesion(void);
void calMedicion(void);
void guardaDatosSD(void);
void ejecutaMedicion(void);
void enviarDatosGUI(uint32_t * vector2, uint32_t * vector3);

#endif /* CODIGOSNICO_TPFINAL_PCSE_SD_INC_MEDICION_H_ */
