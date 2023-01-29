#pragma once

class Status {
public:
	USHORT Type, Stacks;
	FLOAT Timer; UINT CastId;
public:
	enum StatusID : USHORT {
		// MRD
		Storms_Eye = 90,
		// PGL
		Twin_Snakes = 101,
		Opo_Form = 107,
		Raptor_Form = 108,
		Coeurl_Form = 109,
		// LNC
		Life_Surge = 116,
		Chaos_Thrust = 118,
		Power_Surge = 120,
		// RNG
		StraightShotReady = 122,
		VenomousBite = 124,
		Windbite = 129,
		// CNJ
		Aero = 143,
		AeroII = 144,
		MedicaII = 150,
		Freecure = 155,
		Regen = 158,
		// BLM
		Firestarter = 165,
		Swiftcast = 167,
		// SMN
		Bio = 179,
		Miasma = 180,
		Miasma_II = 188,
		Bio_II = 189,
		// AST
		Aspected_Benefic = 835,
		Aspected_Helios = 836,
		Combust = 838,
		Dinural_Sect = 839,
		Nocturnal_Sect = 840,
		CombustII = 843,
		// RDM?
		VerfireReady = 1234,
		VerstoneReady = 1235,
		Dualcast = 1249,
		// Samurai
		Jinpu = 1298,
		Shifu = 1299,
		// DNC
		FlourishingCascade = 1814,
		FlourishingFountain = 1815,
		FlourishingWindmill = 1816,
		FlourishingShower = 1817,
		// Changes
		Disembowel = 1914,
	};
};
